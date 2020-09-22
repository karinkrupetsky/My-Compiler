
#include "Parser.h"

SymbolTable* currTable = NULL;
SymbolTableItem* entryID = NULL;
eTOKENS currTokenType;
eTOKENS retTokenType;
int fullFuncFlag = 0;
int retFlag = 1;

#define VAR 0
#define ARR 1
#define FUNC 2


void parse()
{
	parse_PROG();
	match(EOF_Token);
}

void parse_PROG()
{
	fprintf(yyoutParser, "Rule (PROG -> GLOBAL_VARS FUNC_PREDEFS FUNC_FULL_DEFS)\n");
	currTable = makeTable(currTable);
	fullFuncFlag = 0;
	int retFlag = 1;
	parse_GLOBAL_VARS();
	parse_FUNC_PREDEFS();
	parse_FUNC_FULL_DEFS();
	SymbolTableItem* ptr = (SymbolTableItem*)(malloc(sizeof(SymbolTableItem)));
	ptr = currTable->top;
	while (ptr != NULL)
	{
		if (ptr->flag == 0)
		{
			if (ptr->typeOfID == VAR)
				fprintf(yySemanticOut, "Note- The global variable %s not used, even it was declared in line %d. \n", ptr->name, ptr->IsDeclareted);
			else if (ptr->typeOfID == ARR)
				fprintf(yySemanticOut, "Note- The global array %s not used, even it was declared in line %d. \n", ptr->name, ptr->IsDeclareted);
			else if (ptr->typeOfID == FUNC)
				fprintf(yySemanticOut, "Note- The function %s not used, even it was declared in line %d. \n", ptr->name, ptr->IsDeclareted);
		}
		ptr = ptr->next;
	}
	currTable = popTable(currTable);
}

void parse_GLOBAL_VARS()
{
	fprintf(yyoutParser, "Rule (GLOBAL_VARS -> VAR_DEC GLOBAL_VARS’)\n");
	parse_VAR_DEC();
	parse_GLOBAL_VARS_TAG();
}

void parse_GLOBAL_VARS_TAG()
{
	curr_token = next_token();
	// To know which token is the next, we have to check the next 3 tokens(lookahed).
	curr_token = next_token();
	curr_token = next_token();
	if (curr_token->kind == leftParentheses_Token) // if we got ( we declareing FUNC_PROTOTYPE
	{
		fprintf(yyoutParser, "Rule (GLOBAL_VARS' -> epsilon)\n");
		curr_token = back_token();
		curr_token = back_token();
		curr_token = back_token();
		return;
	}
	else
	{
		// We go back to the 'curr_token' before lookahed.
		curr_token = back_token();
		curr_token = back_token();
		switch (curr_token->kind)
		{
		case (floatKW_Token):
		case (intKW_Token):
			fprintf(yyoutParser, "Rule (GLOBAL_VARS -> VAR_DEC GLOBAL_VARS')\n");
			curr_token = back_token();
			parse_VAR_DEC();
			parse_GLOBAL_VARS_TAG();
			break;
		case(voidKW_Token):
			fprintf(yyoutParser, "Rule (GLOBAL_VARS -> EPSILON )\n");
			curr_token = back_token();
			break;
		default:
			printUnexpectedToken("intKW_Token/floatKW_Token/voidKW_Token");
			while (curr_token->kind != intKW_Token &&
				curr_token->kind != floatKW_Token &&
				curr_token->kind != voidKW_Token &&
				curr_token->kind != EOF_Token)
			{
				curr_token = next_token();
			}
			if (curr_token->kind == EOF_Token) {
				return;
			}
			curr_token = back_token();
			break;
		}
	}
}

void parse_VAR_DEC()
{
	fprintf(yyoutParser, "Rule (VAR_DEC -> TYPE id VAR_DEC’)\n");
	parse_TYPE();
	if(match(ID_Token))
	{
		if ((entryID = insert(currTable, curr_token->lexeme)) != NULL)
		{
			if (entryID != NULL)
			{
				entryID->tokenName = currTokenType;
			}


			if (entryID != NULL)
			{
				entryID->IsDeclareted = curr_token->lineNumber;
			}
			
			entryID->flag = 0;
		}
		else
			fprintf(yySemanticOut, "Error- Duplicated definition of %s at line %d (same scope).\n", curr_token->lexeme, curr_token->lineNumber);
	}
	parse_VAR_DEC_TAG();
}

void parse_VAR_DEC_TAG()
{
	DimSizeList* listSize;
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case semiColon_Token:
	{
		if (entryID != NULL)
		{
			if (entryID != NULL)
			{
				entryID->typeOfID = VAR;

			}
		}
		fprintf(yyoutParser, "Rule (VAR_DEC’ -> ;)\n");
		break;
	}
	case leftBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (VAR_DEC’ -> [DIM_SIZES];)\n");
		if (entryID != NULL)
		{
			if (entryID != NULL)
			{
				entryID->typeOfID = ARR;

			}
			listSize = parse_DIM_SIZES();
			entryID->dimSizeList = listSize;
		}
		else
		{
			listSize = parse_DIM_SIZES();
		}
		match(rightBrackets_Token);
		match(semiColon_Token);
		break;
	}
	default:
	{
		printUnexpectedToken("semiColon_Token/leftBrackets_Token");
		while (curr_token->kind != leftCurlyBraces_Token &&
			curr_token->kind != ifKW_Token &&
			curr_token->kind != ID_Token &&
			curr_token->kind != returnKW_Token &&
			curr_token->kind != intKW_Token &&
			curr_token->kind != floatKW_Token &&
			curr_token->kind != voidKW_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_TYPE()
{
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case intKW_Token:
	{
		fprintf(yyoutParser, "Rule (TYPE -> int )\n");
		currTokenType = curr_token->kind;
		break;
	}
	case floatKW_Token:
	{
		fprintf(yyoutParser, "Rule (TYPE -> float )\n");
		currTokenType = curr_token->kind;
		break;
	}
	default:
	{
		printUnexpectedToken("intKW_Token/floatKW_Token");
		while (curr_token->kind != ID_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

DimSizeList* parse_DIM_SIZES()
{
	DimSizeList* listSize = (DimSizeList*)(malloc(sizeof(DimSizeList)));
	DimSizeList* nodeSize = (DimSizeList*)(malloc(sizeof(DimSizeList)));
	curr_token = next_token();
	if (curr_token->kind == intNum_Token)
	{
		listSize->top = (dimSizeItem*)(malloc(sizeof(dimSizeItem)));
		listSize->top->size = atoi(curr_token->lexeme);
		fprintf(yyoutParser, "Rule (DIM_SIZES -> int_num DIM_SIZES')\n");
		nodeSize = parse_DIM_SIZES_TAG();
		if (nodeSize != NULL)
		{
			listSize->top->next = (dimSizeItem*)(malloc(sizeof(dimSizeItem)));
			listSize->top->next = nodeSize->top;
		}
		else
		{
			listSize->top->next = NULL;
		}
	}
	else
	{
		fprintf(yySemanticOut, "Error- Wrong amount of dimensions in line %d, array dimensions must be a natural number. \n", curr_token->lineNumber);
		printUnexpectedToken("intNum_Token/rightBrackets_Token");
		while (curr_token->kind != rightBrackets_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
	}
	return listSize;
}

DimSizeList* parse_DIM_SIZES_TAG()
{
	DimSizeList* listSize = (DimSizeList*)(malloc(sizeof(DimSizeList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case comma_Token:
	{
		fprintf(yyoutParser, "Rule (DIM_SIZES’ -> , DIM_SIZES)\n");
		listSize = parse_DIM_SIZES();
		break;
	}
	case rightBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (DIM_SIZES’ -> EPSILON)\n");
		curr_token = back_token();
		listSize = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("comma_Token/rightBrackets_Token");
		while (curr_token->kind != rightBrackets_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	return listSize;
	}
}

void parse_FUNC_PREDEFS()
{
	fprintf(yyoutParser, "Rule (FUNC_PREDEFS -> FUNC_PROTOTYPE; FUNC_PREDEFS’ )\n");
	parse_FUNC_PROTOTYPE();
	match(semiColon_Token);
	parse_FUNC_PREDEFS_TAG();
}

//lookahed - we try to look if ti is a func with body or func decleration.
void parse_FUNC_PREDEFS_TAG()
{
	curr_token = next_token();
	int index = 0;
	back_token();
	switch (curr_token->kind)
	{
	case intKW_Token:
	case floatKW_Token:
	case voidKW_Token:
		while (curr_token->kind != semiColon_Token && curr_token->kind != leftCurlyBraces_Token && curr_token->kind != EOF_Token)
		{
			index++;
			curr_token = next_token();
		}

		for (int i = 0; i < index; i++)
		{
			back_token();
		}
		if (curr_token->kind == semiColon_Token)
		{
			fprintf(yyoutParser, "Rule (FUNC_PREDEFS' -> FUNC_PROTOTYPE ; FUNC_PREDEFS' )\n");
			parse_FUNC_PROTOTYPE();
			match(semiColon_Token);
			parse_FUNC_PREDEFS_TAG();
			break;
		}
		else if (curr_token->kind == leftCurlyBraces_Token)
		{
			fprintf(yyoutParser, "Rule (FUNC_PREDEFS' -> EPSILON )\n");
			break;
		}
		break;
	default:
	{
		printUnexpectedToken("intKW_Token/floatKW_Token/voidKW_Token");
		while (curr_token->kind != intKW_Token &&
			curr_token->kind != floatKW_Token &&
			curr_token->kind != voidKW_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_FUNC_PROTOTYPE()
{
	fprintf(yyoutParser, "Rule (FUNC_PROTOTYPE -> RETURNED_TYPE id (PARAMS) )\n");
	ParamList* headOfParams = (ParamList*)(malloc(sizeof(ParamList)));
	parse_RETURNED_TYPE();
	if (match(ID_Token))
	{
		if (fullFuncFlag) 
		{
			entryID = lookup(currTable, curr_token->lexeme);

			if (entryID == NULL) 
			{
				entryID = insert(currTable, curr_token->lexeme);
				match(leftParentheses_Token);
				if (entryID != NULL)
				{
					entryID->tokenName = currTokenType;
				}
				headOfParams = parse_PARAMS();
				if (entryID != NULL)
				{
					entryID->typeOfID = FUNC;

				}
				if (entryID != NULL)
				{
					entryID->IsDeclareted = curr_token->lineNumber;
				}
				if (entryID != NULL)
				{
					entryID->listParameterTypes = (ParamList*)(malloc(sizeof(ParamList)));
					entryID->listParameterTypes = headOfParams;
				}
				entryID->flag = 1;
			}
			else if (entryID->typeOfID != FUNC) 
			{
				fprintf(yySemanticOut, "Error- The function %s in line %d was not declared. \n", entryID->name, curr_token->lineNumber);
				match(leftParentheses_Token);
				headOfParams = parse_PARAMS();
			}
			else
			{
				if (strcmp(enumStringTranslator(currTokenType), enumStringTranslator(entryID->tokenName)) != 0) 
				{
					fprintf(yySemanticOut, "Error- The function %s was not declared in line %d  because it has different return value. \n", entryID->name, curr_token->lineNumber);
					entryID->tokenName = currTokenType; 
				}
				match(leftParentheses_Token);
				headOfParams = parse_PARAMS();
				if (!(isEqualParamAmount(headOfParams, entryID->listParameterTypes)))
				{
					fprintf(yySemanticOut, "Error- The function %s was not declared in line %d because it has different parameter list. \n", entryID->name, curr_token->lineNumber);
					if (entryID != NULL)
					{
						entryID->listParameterTypes = (ParamList*)(malloc(sizeof(ParamList)));
						entryID->listParameterTypes = headOfParams;
					}
				}
				entryID->flag = 1;
				if (entryID != NULL)
				{
					entryID->IsDeclareted = curr_token->lineNumber;
				}
			}
		}
		else
		{
			if ((entryID = insert(currTable, curr_token->lexeme)) != NULL)
			{
				match(leftParentheses_Token);
				if (entryID != NULL)
				{
					entryID->tokenName = currTokenType;
				}
				headOfParams = parse_PARAMS();
				if (entryID != NULL)
				{
					entryID->typeOfID = FUNC;
				}
				if (entryID != NULL)
				{
					entryID->IsDeclareted = curr_token->lineNumber;
				}
				if (entryID != NULL)
				{
					entryID->listParameterTypes = (ParamList*)(malloc(sizeof(ParamList)));
					entryID->listParameterTypes = headOfParams;
				}
				entryID->flag = 0;
			}
			else
			{
				fprintf(yySemanticOut, "Error- Duplicated definition of %s in line %d (same scope).\n", curr_token->lexeme, curr_token->lineNumber);
				match(leftParentheses_Token);
				headOfParams = parse_PARAMS();
			}
		}
	}
	match(rightParentheses_Token);
}

void parse_FUNC_FULL_DEFS()
{
	fprintf(yyoutParser, "Rule (FUNC_FULL_DEFS -> FUNC_WITH_BODY FUNC_FULL_DEFS’ )\n");
	fullFuncFlag = 1;
	parse_FUNC_WITH_BODY();
	parse_FUNC_FULL_DEFS_TAG();
}

void parse_FUNC_FULL_DEFS_TAG()
{
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case intKW_Token:
	case floatKW_Token:
	case voidKW_Token:
	{
		fprintf(yyoutParser, "Rule (FUNC_FULL_DEFS’ -> FUNC_FULL_DEFS)\n");
		curr_token = back_token();
		parse_FUNC_FULL_DEFS();
		break;
	}
	case EOF_Token:
	{
		fprintf(yyoutParser, "Rule (FUNC_FULL_DEFS’ -> EPSILON)\n");
		curr_token = back_token();
		break;
	}
	default:
	{
		printUnexpectedToken("intKW_Token/floatKW_Token/voidKW_Token/EOF_Token");
		while (curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_FUNC_WITH_BODY()
{
	fprintf(yyoutParser, "Rule (FUNC_WITH_BODY -> FUNC_PROTOTYPE COMP _STMT)\n");
	parse_FUNC_PROTOTYPE();
	parse_COMP_STMT();
}

void parse_RETURNED_TYPE()
{
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case intKW_Token:
	case floatKW_Token:
	{
		fprintf(yyoutParser, "Rule (RETURNED_TYPE -> TYPE )\n");
		curr_token = back_token();
		parse_TYPE();
		break;
	}
	case voidKW_Token:
	{
		fprintf(yyoutParser, "Rule (RETURNED_TYPE -> void )\n");
		currTokenType = voidKW_Token;
		break;
	}
	default:
	{
		printUnexpectedToken("intKW_Token/floatKW_Token/voidKW_Token");
		while (curr_token->kind != ID_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

ParamList* parse_PARAMS()
{
	ParamList* param_head;
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case intKW_Token:
	case floatKW_Token:
	{
		fprintf(yyoutParser, "Rule (PARAMS -> PARAM_LIST)\n");
		curr_token = back_token();
		param_head = parse_PARAM_LIST();
		break;
	}
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (PARAMS -> EPSILON)\n");
		curr_token = back_token();
		param_head = NULL;
		break;
	}
	default:
	{
		param_head = NULL;
		printUnexpectedToken("intKW_Token/floatKW_Token/rightParentheses_Token");
		while (curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return param_head;
}

ParamList* parse_PARAM_LIST()
{
	fprintf(yyoutParser, "Rule (PARAM_LIST ->PARAM PARAM_LIST’ )\n");
	ParamList* param_head = (ParamList*)(malloc(sizeof(ParamList)));
	param_head->top = parse_PARAM();
	param_head->top->next = NULL;
	param_head->top->prev = NULL;
	param_head->top->next = parse_PARAM_LIST_TAG()->top;
	if (isExistsParam(param_head, param_head->top) == 0) 
		fprintf(yySemanticOut, "Error- Parameter name was declared before in line %d (dup).\n", curr_token->lineNumber);
	return param_head;
}

ParamList* parse_PARAM_LIST_TAG()
{
	ParamList* param = (ParamList*)(malloc(sizeof(ParamList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case comma_Token:
	{
		fprintf(yyoutParser, "Rule (PARAM_LIST’ -> , PARAM PARAM_LIST’)\n");
		param->top = parse_PARAM();
		param->top->next = NULL;
		param->top->next = parse_PARAM_LIST_TAG()->top;
		if (isExistsParam(param, param->top) == 0)
		{
			fprintf(yySemanticOut, "Error- Parameter name was declared before in line %d (dup). \n", curr_token->lineNumber);
		}
		break;
	}
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (PARAM_LIST’ -> EPSILON)\n");
		curr_token = back_token();
		param->top = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("comma_Token/rightParentheses_Token");
		while (curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return param;
}

ParamItem* parse_PARAM()
{
	fprintf(yyoutParser, "Rule (PARAM -> TYPE id PARAM’ )\n");
	ParamItem* param_node = (ParamItem*)(malloc(sizeof(ParamItem)));
	DimSizeList* listSize;
	parse_TYPE();
	if (match(ID_Token))
	{
		curr_token = back_token();
		param_node->tokenName = curr_token->kind;
		curr_token = next_token();
		param_node->name = curr_token->lexeme;
	}
	listSize = parse_PARAM_TAG();
	param_node->dimSize = listSize;
	if (listSize == NULL)
		param_node->typeOfID = 0;
	else
		param_node->typeOfID = 1;
	return param_node;
}

DimSizeList* parse_PARAM_TAG()
{
	DimSizeList* listSize = (DimSizeList*)(malloc(sizeof(DimSizeList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case leftBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (PARAM’ -> [DIM_SIZES])\n");
		listSize = parse_DIM_SIZES();
		match(rightBrackets_Token);
		break;
	}
	case comma_Token:
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (PARAM’ -> EPSILON)\n");
		curr_token = back_token();
		listSize = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("leftBrackets_Token/comma_Token/rightParentheses_Token");
		while (curr_token->kind != comma_Token &&
			curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return listSize;
}

void parse_COMP_STMT()
{
	SymbolTableItem* ptr = (SymbolTableItem*)(malloc(sizeof(SymbolTableItem)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case leftCurlyBraces_Token:
	{
		fprintf(yyoutParser, "Rule (COMP_STMT -> { VAR_DEC_LIST STMT_LIST })\n");
		currTable = makeTable(currTable);
		if (entryID != NULL)
		{
			if (entryID->typeOfID == FUNC)
			{
				currTable->typeOfID = FUNC;
				retTokenType = entryID->tokenName;
				if (strcmp(enumStringTranslator(retTokenType), enumStringTranslator(voidKW_Token)) != 0)
					retFlag = 0;
				int line = entryID->IsDeclareted;
				if (entryID->listParameterTypes != NULL)
				{
					ParamItem* listParamter = entryID->listParameterTypes->top;
					while (listParamter != NULL)
					{
						if ((entryID = insert(currTable, listParamter->name)) != NULL)
						{
							if (entryID != NULL)
							{
								entryID->tokenName = listParamter->tokenName;
							}
							if (listParamter->typeOfID == 0)
								if (entryID != NULL)
								{
									entryID->typeOfID = VAR;

								}
							else
							{
									if (entryID != NULL)
									{
										entryID->typeOfID = ARR;

									}
								entryID->dimSizeList = listParamter->dimSize;
							}
							entryID->IsDeclareted = line;
						}
						listParamter = listParamter->next;
					}
				}
			}
			else
			{
				currTable->typeOfID = 0;
			}
		}
		else
		{
			currTable->typeOfID = 0;
		}
		parse_VAR_DEC_LIST();
		parse_STMT_LIST();

		match(rightCurlyBraces_Token);

		if (currTable->typeOfID == 2)
		{
			if (retFlag == 0)
			{
				fprintf(yySemanticOut, "Error- The function must return a value at line %d. \n", curr_token->lineNumber);
			}
		}
		retFlag = 1;
		ptr = currTable->top;
		while (ptr != NULL)
		{
			if (ptr->flag == 0)
			{
				if (ptr->typeOfID == VAR)
					fprintf(yySemanticOut, "Note- The variable %s not used, it was declared in line %d. \n", ptr->name, ptr->IsDeclareted);
				else if (ptr->typeOfID == ARR)
					fprintf(yySemanticOut, "Note- The array %s not used, it was declared in line %d. \n", ptr->name, ptr->IsDeclareted);
			}
			ptr = ptr->next;
		}
		currTable = popTable(currTable);
		break;
	}
	default:
	{
		printUnexpectedToken("leftCurlyBraces_Token");
		while (curr_token->kind != leftCurlyBraces_Token &&
			curr_token->kind != semiColon_Token &&
			curr_token->kind != floatKW_Token &&
			curr_token->kind != intKW_Token &&
			curr_token->kind != voidKW_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_VAR_DEC_LIST()
{
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case intKW_Token:
	case floatKW_Token:
	{
		fprintf(yyoutParser, "Rule (VAR_DEC_LIST -> VAR_DEC VAR_DEC_LIST)\n");
		curr_token = back_token();
		parse_VAR_DEC();
		parse_VAR_DEC_LIST();
		break;
	}
	case leftCurlyBraces_Token:
	case ifKW_Token:
	case ID_Token:
	case returnKW_Token:
	{
		fprintf(yyoutParser, "Rule (VAR_DEC_LIST -> EPSILON)\n");
		curr_token = back_token();
		break;
	}
	default:
	{
		printUnexpectedToken("intKW_Token/floatKW_Token/leftCurlyBraces_Token/ifKW_Token/ID_Token/returnKW_Token");
		while (curr_token->kind != leftCurlyBraces_Token &&
			curr_token->kind != ifKW_Token &&
			curr_token->kind != ID_Token &&
			curr_token->kind != returnKW_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_STMT_LIST()
{
	fprintf(yyoutParser, "Rule (STMT_LIST -> STMT STMT_LIST’)\n");
	parse_STMT();
	parse_STMT_LIST_TAG();
}

void parse_STMT_LIST_TAG()
{
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case semiColon_Token:
	{
		fprintf(yyoutParser, "Rule (STMT_LIST’ -> ; STMT STMT_LIST’)\n");
		parse_STMT();
		parse_STMT_LIST_TAG();
		break;
	}
	case rightCurlyBraces_Token:
	{
		fprintf(yyoutParser, "Rule (STMT_LIST’ -> EPSILON)\n");
		curr_token = back_token();
		break;
	}
	default:
	{
		printUnexpectedToken("semiColon_Token/rightCurlyBraces_Token");
		while (curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_STMT()
{
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case ID_Token:
	{
		fprintf(yyoutParser, "Rule (STMT -> id STMT’)\n");
		entryID = find(currTable, curr_token->lexeme);
		if (entryID != NULL)
		{
			if (entryID != NULL)
			{
				entryID->flag = 1;

			}
		}
		else
		{
			fprintf(yySemanticOut, "Error- The ID %s at line %d was not declared. \n", curr_token->lexeme, curr_token->lineNumber);
		}
		parse_STMT_TAG();
		break;
	}
	case leftCurlyBraces_Token:
	{
		fprintf(yyoutParser, "Rule (STMT -> COMP_STMT)\n");
		curr_token = back_token();
		parse_COMP_STMT();
		break;
	}
	case ifKW_Token:
	{
		fprintf(yyoutParser, "Rule (STMT -> IF_STMT)\n");
		curr_token = back_token();
		parse_IF_STMT();
		break;
	}
	case returnKW_Token:
	{
		fprintf(yyoutParser, "Rule (STMT -> RETURN_STMT)\n");
		curr_token = back_token();
		parse_RETURN_STMT();
		break;
	}
	default:
	{
		printUnexpectedToken("leftCurlyBraces_Token/ID_Token/ifKW_Token/returnKW_Token");
		while (curr_token->kind != semiColon_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_STMT_TAG()
{
	ParamItem* paramItem1, * paramItem2;
	paramItem1 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem2 = (ParamItem*)(malloc(sizeof(ParamItem)));
	ParamList* paramListHead = (ParamList*)(malloc(sizeof(ParamList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case assignOp_Token:
	case leftBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (STMT’ -> VAR’ = EXPR)\n");
		curr_token = back_token();
		paramItem1 = parse_VAR_TAG();
		match(assignOp_Token);
		paramItem2 = parse_EXPR();
		if (paramItem1 == NULL || paramItem2 == NULL)
			fprintf(yySemanticOut, "Error- The assignment in line %d can't be done.\n", curr_token->lineNumber);
		else
		{
			if ((strcmp(enumStringTranslator(paramItem1->tokenName), enumStringTranslator(intKW_Token)) == 0) && (strcmp(enumStringTranslator(paramItem2->tokenName), enumStringTranslator(floatKW_Token)) == 0))
			{
				fprintf(yySemanticOut, "Error- The assigniment float to int variable in line %d is not valid. \n", curr_token->lineNumber);
			}
			else
			{
				paramItem1->val = paramItem2->val;
				entryID = find(currTable, paramItem1->name);
				if (entryID != NULL)
				{
					entryID->val = paramItem1->val;
				}
			}
		}
		break;
	}
	case leftParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (STMT’ -> ( ARGS ) )\n");
		paramListHead = parse_ARGS();
		match(rightParentheses_Token);
		if (entryID != NULL)
		{
			if (entryID->typeOfID != FUNC)
				fprintf(yySemanticOut, "Error- The assignment into variable/array %s at line %d is not valid. \n", entryID->name, curr_token->lineNumber);
			else
			{
				if (entryID->flag == VAR)
				{
					fprintf(yySemanticOut, "Error- There is no implementation of the function %s at line %d. \n", entryID->name, curr_token->lineNumber);
				}
				else if (isSameParamsCount(paramListHead, entryID->listParameterTypes) == 0)
				{
					fprintf(yySemanticOut, "Error- Wrong amount of parameters assigned to function %s at line %d. \n", entryID->name, curr_token->lineNumber);
				}
				else if (isEqualParamAmount(paramListHead, entryID->listParameterTypes) == 0)
				{
					fprintf(yySemanticOut, "Error- There is at least one unmatch parameter type in the function %s in line %d.  \n", entryID->name, curr_token->lineNumber);
				}
			}
		}
		break;
	}
	default:
	{
		printUnexpectedToken("leftBrackets_Token/assignOp_Token/leftParentheses_Token");
		while (curr_token->kind != semiColon_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token(); 
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

void parse_IF_STMT()
{
	fprintf(yyoutParser, "Rule (IF_STMT -> if (CONDITION) STMT )\n");
	match(ifKW_Token);
	match(leftParentheses_Token);
	parse_CONDITION();
	match(rightParentheses_Token);
	parse_STMT();
}

void parse_CALL()
{
	fprintf(yyoutParser, "Rule (CALL -> id ( ARGS ) )\n");
	match(ID_Token);
	match(leftParentheses_Token);
	parse_ARGS();
	match(rightParentheses_Token);
}

ParamList* parse_ARGS()
{
	ParamList* paramListHead = (ParamList*)(malloc(sizeof(ParamList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case ID_Token:
	case intNum_Token:
	case floatNum_Token:
	case leftParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (ARGS -> ARG_LIST )\n");
		curr_token = back_token();
		paramListHead = parse_ARG_LIST();
		break;
	}
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (ARGS -> EPSILON )\n");
		curr_token = back_token();
		paramListHead = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("ID_Token/intNum_Token/floatNum_Token/leftParentheses_Token/rightParentheses_Token");
		while (curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramListHead;
}

ParamList* parse_ARG_LIST()
{
	fprintf(yyoutParser, "Rule (ARG_LIST -> EXPR ARG_LIST’ )\n");
	ParamList* paramListHead = (ParamList*)(malloc(sizeof(ParamList)));
	paramListHead->top = parse_EXPR();
	if (paramListHead->top != NULL)
	{
		paramListHead->top->next = NULL;
		paramListHead->top->prev = NULL;
		paramListHead->top->next = parse_ARG_LIST_TAG()->top;
	}
	else
	{
		parse_ARG_LIST_TAG();
		if (paramListHead->top != NULL)
			paramListHead->top->next = NULL;
	}
	return paramListHead;
}

ParamList* parse_ARG_LIST_TAG()
{
	ParamList* paramListHead = (ParamList*)(malloc(sizeof(ParamList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case comma_Token:
	{
		fprintf(yyoutParser, "Rule (ARG_LIST’ -> , EXPR ARG_LIST’)\n");
		paramListHead->top = parse_EXPR();
		if (paramListHead->top != NULL)
		{
			paramListHead->top->next = NULL;
			paramListHead->top->next = parse_ARG_LIST_TAG()->top;
		}
		else
		{
			parse_ARG_LIST_TAG();
			if (paramListHead->top != NULL)
				paramListHead->top->next = NULL;
		}
		break;
	}
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (ARG_LIST’ -> EPSILON )\n");
		curr_token = back_token();
		paramListHead->top = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("comma_Token/rightParentheses_Token");
		while (curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramListHead;
}

void parse_RETURN_STMT()
{
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case returnKW_Token:
	{
		fprintf(yyoutParser, "Rule (RETURN_STMT -> return RETURN_STMT')\n");
		retFlag = 1;
		paramItem = parse_RETURN_STMT_TAG();
		if (paramItem == NULL)
		{
			if ((strcmp(enumStringTranslator(retTokenType),
				enumStringTranslator(voidKW_Token)) == 0))
			{
			}
			else if (((strcmp(enumStringTranslator(retTokenType),
				enumStringTranslator(intKW_Token)) == 0) || (strcmp(enumStringTranslator(retTokenType),
					enumStringTranslator(floatKW_Token)) == 0)))
			{
				fprintf(yySemanticOut, "Error- A value must be returned at line %d. \n", curr_token->lineNumber);
			}
		}
		else
		{
			if (strcmp(enumStringTranslator(paramItem->tokenName),
				enumStringTranslator(retTokenType)) == 0)
			{
			}
			else if (((strcmp(enumStringTranslator(paramItem->tokenName),
				enumStringTranslator(intKW_Token)) == 0)) && (strcmp(enumStringTranslator(retTokenType),
					enumStringTranslator(intKW_Token)) == 0))
			{
				fprintf(yySemanticOut, "Error- A float value must be returned at line %d. \n", curr_token->lineNumber);
			}
			else if ((strcmp(enumStringTranslator(paramItem->tokenName),
				enumStringTranslator(floatKW_Token) == 0)) && (strcmp(enumStringTranslator(retTokenType),
					enumStringTranslator(intKW_Token)) == 0))
			{
				fprintf(yySemanticOut, "Error- An int value must be returned at line %d. \n", curr_token->lineNumber);
			}
			else if (((strcmp(enumStringTranslator(paramItem->tokenName),
				enumStringTranslator(floatKW_Token)) == 0) || (strcmp(enumStringTranslator(paramItem->tokenName),
					enumStringTranslator(intKW_Token)) == 0)) && (strcmp(enumStringTranslator(retTokenType),
						enumStringTranslator(voidKW_Token)) == 0))
			{
				fprintf(yySemanticOut, "Error- Cannot return a value at line %d. \n", curr_token->lineNumber);
			}
		}
		break;
	}
	default:
	{
		printUnexpectedToken("returnKW_Token");
		while (curr_token->kind != semiColon_Token &&
			curr_token->kind != returnKW_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
}

ParamItem* parse_RETURN_STMT_TAG()
{
	curr_token = next_token();
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	switch (curr_token->kind)
	{
	case ID_Token:
	case intNum_Token:
	case floatNum_Token:
	case leftParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (RETURN_STMT’ -> EXPR )\n");
		curr_token = back_token();
		paramItem = parse_EXPR();
		break;
	}
	case semiColon_Token:
	case rightCurlyBraces_Token:
	{
		fprintf(yyoutParser, "Rule (RETURN_STMT’ -> EPSILON )\n");
		curr_token = back_token();
		paramItem = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("ID_Token/intNum_Token/floatNum_Token/leftParentheses_Token/semiColon_Token/rightCurlyBraces_Token");
		while (curr_token->kind != semiColon_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramItem;
}

ParamItem* parse_VAR_TAG()
{
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	DimSizeList* sizesList;
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case leftBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (VAR’ -> [ EXPR_LIST] )\n");
		sizesList = parse_EXPR_LIST();
		if (entryID != NULL)
		{
			if (entryID->typeOfID != ARR) 
			{
				fprintf(yySemanticOut, "Error- The assignment into variable/function %s at line %d is not valid. \n", entryID->name, curr_token->lineNumber);
				paramItem = NULL;
			}
			else
			{
				if (isSameSize(entryID->dimSizeList, sizesList) == 0)
				{
					fprintf(yySemanticOut, "Error- Array %s out of bounds at line %d. \n", entryID->name, curr_token->lineNumber);
					paramItem = NULL;
				}
				else if (isSameSize(entryID->dimSizeList, sizesList) == 2)
				{
					fprintf(yySemanticOut, "Error- Wrong amount of dimensions of the array %s at line %d. \n", entryID->name, curr_token->lineNumber);
					paramItem = NULL;
				}
				else
				{
					paramItem->tokenName = entryID->tokenName;
					paramItem->name = entryID->name;
				}
			}
		}
		else
		{
			paramItem = NULL;
		}
		match(rightBrackets_Token);
		break;
	}
	case arOpMul_Token:
	case arOpAdd_Token:
	case comma_Token:
	case relOpBiggerEqualThan_Token:
	case relOpBiggerThan_Token:
	case relOpEqual_Token:
	case relOpNotEqual_Token:
	case relOpSmallerEqualThan_Token:
	case relOpSmallerThan_Token:
	case semiColon_Token:
	case rightCurlyBraces_Token:
	case rightBrackets_Token:
	case rightParentheses_Token:
	case assignOp_Token:
	{
		fprintf(yyoutParser, "Rule (VAR’ -> EPSILON )\n");
		if (entryID != NULL)
		{
			if (entryID->typeOfID != VAR) 
			{
				fprintf(yySemanticOut, "Error- The assignment into function/array %s at line %d is not valid. \n", entryID->name, curr_token->lineNumber);
				paramItem = NULL;
			}
			else
			{
				paramItem->name = (char*)(calloc(strlen(entryID->name), sizeof(char)));
				paramItem->name = entryID->name;
				paramItem->tokenName = entryID->tokenName;
				paramItem->val = entryID->val;
			}
		}
		else
		{
			paramItem = NULL;
		}
		curr_token = back_token();
		break;
	}
	default:
	{
		printUnexpectedToken("leftBrackets_Token/comma_Token/arOpMul_Token/arOpAdd_Token/relOpBiggerEqualThan_Token/relOpBiggerThan_Token/relOpEqual_Token/relOpNotEqual_Token/relOpSmallerEqualThan_Token/relOpSmallerThan_Token/semiColon_Token/rightCurlyBraces_Token/rightBrackets_Token/rightParentheses_Token/assignOp_Token");
		while (curr_token->kind != arOpMul_Token &&
			curr_token->kind != arOpAdd_Token &&
			curr_token->kind != relOpBiggerEqualThan_Token &&
			curr_token->kind != relOpBiggerThan_Token &&
			curr_token->kind != relOpNotEqual_Token &&
			curr_token->kind != comma_Token &&
			curr_token->kind != relOpSmallerEqualThan_Token &&
			curr_token->kind != relOpSmallerThan_Token &&
			curr_token->kind != semiColon_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != assignOp_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramItem;
}

DimSizeList* parse_EXPR_LIST()
{
	fprintf(yyoutParser, "Rule (EXPR_LIST -> EXPR EXPR_LIST’ )\n");
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	DimSizeList* sizesList = (DimSizeList*)(malloc(sizeof(DimSizeList)));
	sizesList->top = NULL;
	paramItem = parse_EXPR();
	if (paramItem != NULL)
	{
		if (strcmp(enumStringTranslator(paramItem->tokenName),
			enumStringTranslator(floatKW_Token)) == 0)
		{
			if (entryID != NULL)
			{
				fprintf(yySemanticOut, "Error- Size must be a natural number to array %s at line %d. \n", entryID->name, curr_token->lineNumber);
			}
			else
			{
				fprintf(yySemanticOut, "Error- The array size must be a natural number at line %d. \n", curr_token->lineNumber);
			}
			sizesList = parse_EXPR_LIST_TAG();
		}
		else
		{
			insertDimSizeList(sizesList, (int)(paramItem->val));
			sizesList->top->next = parse_EXPR_LIST_TAG()->top;
		}
	}
	else
	{
		parse_EXPR_LIST_TAG();
		sizesList = NULL;
	}
	return sizesList;
}

DimSizeList* parse_EXPR_LIST_TAG()
{
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	DimSizeList* sizesList = (DimSizeList*)(malloc(sizeof(DimSizeList)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case comma_Token:
	{
		fprintf(yyoutParser, "Rule (EXPR_LIST’ -> , EXPR EXPR_LIST’ )\n");
		paramItem = parse_EXPR();
		if (strcmp(enumStringTranslator(paramItem->tokenName),
			enumStringTranslator(floatKW_Token)) == 0)
		{
			if (entryID != NULL)
			{
				fprintf(yySemanticOut, "Error- The array %s size must be a natural number at line %d. \n", entryID->name, curr_token->lineNumber);
			}
			else
			{
				fprintf(yySemanticOut, "Error- The array size must be a natural number at line %d. \n", curr_token->lineNumber);
			}
			sizesList = parse_EXPR_LIST_TAG();
		}
		else
		{
			insertDimSizeList(sizesList, (int)(paramItem->val));
			sizesList->top->next = parse_EXPR_LIST_TAG()->top;
		}
		return sizesList;
		break;
	}
	case rightBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (EXPR_LIST’ -> EPSILON )\n");
		curr_token = back_token();
		sizesList->top = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("comma_Token/rightBrackets_Token");
		while (curr_token->kind != rightBrackets_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return sizesList;
}

void parse_CONDITION()
{
	fprintf(yyoutParser, "Rule (CONDITION -> EXPR rel_op EXPR )\n");
	parse_EXPR();
	curr_token = next_token();
	if (curr_token->kind == relOpBiggerEqualThan_Token || curr_token->kind == relOpBiggerThan_Token || curr_token->kind == relOpEqual_Token || curr_token->kind == relOpNotEqual_Token
		|| curr_token->kind == relOpSmallerEqualThan_Token || curr_token->kind == relOpSmallerThan_Token)
	{
		parse_EXPR();
	}
	else
	{
		printUnexpectedToken("relOpBiggerEqualThan_Token/relOpBiggerThan_Token/relOpEqual_Token/relOpNotEqual_Token/relOpSmallerEqualThan_Token/relOpSmallerThan_Token");
		while (curr_token->kind != relOpBiggerEqualThan_Token &&
			curr_token->kind != relOpBiggerThan_Token &&
			curr_token->kind != relOpEqual_Token &&
			curr_token->kind != relOpNotEqual_Token &&
			curr_token->kind != relOpSmallerEqualThan_Token &&
			curr_token->kind != relOpSmallerThan_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
	}
}

ParamItem* parse_EXPR()
{
	fprintf(yyoutParser, "Rule (EXPR -> TERM EXPR’ )\n");
	ParamItem* paramItem1, * paramItem2;
	paramItem1 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem2 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem1 = parse_TERM();
	paramItem2 = parse_EXPR_TAG();
	if (paramItem1 != NULL)
	{
		if (paramItem2 != NULL)
		{
			if (strcmp(enumStringTranslator(paramItem2->tokenName),
				enumStringTranslator(floatKW_Token)) == 0)
			{
				paramItem1->tokenName = floatKW_Token;
			}
			paramItem1->val += paramItem2->val;
		}
	}
	return paramItem1;
}

ParamItem* parse_EXPR_TAG()
{
	ParamItem* paramItem1, * paramItem2;
	paramItem1 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem2 = (ParamItem*)(malloc(sizeof(ParamItem)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case arOpAdd_Token:
	{
		fprintf(yyoutParser, "Rule (EXPR’ -> + TERM EXPR’ )\n");
		paramItem1 = parse_TERM();
		paramItem2 = parse_EXPR_TAG();
		if (paramItem1 != NULL)
		{
			if (paramItem2 != NULL)
			{
				if (strcmp(enumStringTranslator(paramItem2->tokenName),
					enumStringTranslator(floatKW_Token)) == 0)
				{
					paramItem1->tokenName = floatKW_Token;
				}
				paramItem1->val += paramItem2->val;
			}
		}
		break;
	}
	case comma_Token:
	case relOpBiggerEqualThan_Token:
	case relOpBiggerThan_Token:
	case relOpEqual_Token:
	case relOpNotEqual_Token:
	case relOpSmallerEqualThan_Token:
	case relOpSmallerThan_Token:
	case semiColon_Token:
	case rightCurlyBraces_Token:
	case rightBrackets_Token:
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (EXPR' -> EPSILON )\n");
		curr_token = back_token();
		paramItem1 = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("arOpAdd_Token/comma_Token/relOpBiggerEqualThan_Token/relOpBiggerThan_Token/relOpEqual_Token/relOpNotEqual_Token/relOpSmallerEqualThan_Token/relOpSmallerThan_Token/semiColon_Token/rightCurlyBraces_Token/rightBrackets_Token/rightParentheses_Token");
		while (curr_token->kind != comma_Token &&
			curr_token->kind != relOpBiggerEqualThan_Token &&
			curr_token->kind != relOpBiggerThan_Token &&
			curr_token->kind != relOpEqual_Token &&
			curr_token->kind != relOpNotEqual_Token &&
			curr_token->kind != relOpSmallerEqualThan_Token &&
			curr_token->kind != relOpSmallerThan_Token &&
			curr_token->kind != semiColon_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != rightBrackets_Token &&
			curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramItem1;
}

ParamItem* parse_TERM()
{
	fprintf(yyoutParser, "Rule (TERM -> FACTOR TERM’ )\n");
	ParamItem* paramItem1, * paramItem2;
	paramItem1 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem2 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem1 = parse_FACTOR();
	paramItem2 = parse_TERM_TAG();
	if (paramItem1 != NULL)
	{
		if (paramItem2 != NULL)
		{
			if (strcmp(enumStringTranslator(paramItem2->tokenName),
				enumStringTranslator(floatKW_Token)) == 0)
			{
				paramItem1->tokenName = floatKW_Token;
			}
			paramItem1->val *= paramItem2->val;
		}
	}
	return paramItem1;
}

ParamItem* parse_TERM_TAG()
{
	ParamItem* paramItem1, * paramItem2;
	paramItem1 = (ParamItem*)(malloc(sizeof(ParamItem)));
	paramItem2 = (ParamItem*)(malloc(sizeof(ParamItem)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case arOpMul_Token:
	{
		fprintf(yyoutParser, "Rule (TERM’ -> * FACTOR TERM’ )\n");
		paramItem1 = parse_FACTOR();
		paramItem2 = parse_TERM_TAG();
		if (paramItem1 != NULL)
		{
			if (paramItem2 != NULL)
			{
				if (strcmp(enumStringTranslator(paramItem2->tokenName),
					enumStringTranslator(floatKW_Token)) == 0)
				{
					paramItem1->tokenName = floatKW_Token;
				}
				paramItem1->val *= paramItem2->val;
			}
		}
		break;
	}
	case arOpAdd_Token:
	case comma_Token:
	case relOpBiggerEqualThan_Token:
	case relOpBiggerThan_Token:
	case relOpEqual_Token:
	case relOpNotEqual_Token:
	case relOpSmallerEqualThan_Token:
	case relOpSmallerThan_Token:
	case semiColon_Token:
	case rightParentheses_Token:
	case rightCurlyBraces_Token:
	case rightBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (TERM’ -> EPSILON )\n");
		curr_token = back_token();
		paramItem1 = NULL;
		break;
	}
	default:
	{
		printUnexpectedToken("arOpMul_Token/arOpAdd_Token/comma_Token/relOpBiggerEqualThan_Token/relOpBiggerThan_Token/relOpEqual_Token/relOpNotEqual_Token/relOpSmallerEqualThan_Token/ relOpSmallerThan_Token/ semiColon_Token/ rightCurlyBraces_Token/ rightBrackets_Token/ rightParentheses_Token");
		while (curr_token->kind != arOpAdd_Token &&
			curr_token->kind != comma_Token &&
			curr_token->kind != relOpBiggerEqualThan_Token &&
			curr_token->kind != relOpBiggerThan_Token &&
			curr_token->kind != relOpEqual_Token &&
			curr_token->kind != relOpNotEqual_Token &&
			curr_token->kind != relOpSmallerEqualThan_Token &&
			curr_token->kind != relOpSmallerThan_Token &&
			curr_token->kind != semiColon_Token &&
			curr_token->kind != rightParentheses_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != rightBrackets_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramItem1;
}

ParamItem* parse_FACTOR()
{
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case ID_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR -> id FACTOR’ )\n");
		entryID = find(currTable, curr_token->lexeme);
		if (entryID == NULL)
		{
			fprintf(yySemanticOut, "Error- The ID %s at line %d was not declared. \n", curr_token->lexeme, curr_token->lineNumber);
			paramItem = NULL;
			parse_FACTOR_TAG();
		}
		else
		{
			if (entryID != NULL)
			{
				entryID->flag = 1;

			}
			paramItem = parse_FACTOR_TAG();
		}
		break;
	}
	case intNum_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR -> int_num )\n");
		paramItem->tokenName = intKW_Token;
		paramItem->val = atof(curr_token->lexeme);
		break;
	}
	case floatNum_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR -> float_num )\n");
		paramItem->tokenName = floatKW_Token;
		paramItem->val = atof(curr_token->lexeme);
		break;
	}
	case leftParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR -> (EXPR) )\n");
		paramItem = parse_EXPR();
		match(rightParentheses_Token);
		break;
	}
	default:
	{
		printUnexpectedToken("ID_Token/intNum_Token/floatNum_Token/leftParentheses_Token/arOpMul_Token/arOpAdd_Token/relOpBiggerEqualThan_Token/relOpBiggerThan_Token/relOpEqual_Token/relOpNotEqual_Token/relOpSmallerEqualThan_Token/relOpSmallerThan_Token/semiColon_Token/comma_Token/rightCurlyBraces_Token/rightBrackets_Token/rightParentheses_Token");
		while (curr_token->kind != arOpMul_Token &&
			curr_token->kind != arOpAdd_Token &&
			curr_token->kind != relOpBiggerEqualThan_Token &&
			curr_token->kind != relOpBiggerThan_Token &&
			curr_token->kind != relOpEqual_Token &&
			curr_token->kind != relOpNotEqual_Token &&
			curr_token->kind != relOpSmallerEqualThan_Token &&
			curr_token->kind != relOpSmallerThan_Token &&
			curr_token->kind != semiColon_Token &&
			curr_token->kind != comma_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != rightBrackets_Token &&
			curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}

		if (curr_token->kind == EOF_Token) {
			return;
		}

		curr_token = back_token();
		break;
	}
	}
	
	if (paramItem != NULL)
		paramItem->typeOfID = 0; 
	return paramItem;
}

ParamItem* parse_FACTOR_TAG()
{
	ParamItem* paramItem = (ParamItem*)(malloc(sizeof(ParamItem)));
	SymbolTableItem* tableItemEntry;
	ParamList* paramList;
	DimSizeList* sizesList;
	curr_token = next_token();
	switch (curr_token->kind)
	{
	case leftBrackets_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR' -> [ EXPR_LIST ])\n");
		sizesList = parse_EXPR_LIST();
		if (entryID != NULL)
		{
			if (entryID->typeOfID != ARR)
			{
				fprintf(yySemanticOut, "Error: The assignment into variable/array %s at line %d is not valid. \n", entryID->name, curr_token->lineNumber);
				paramItem = NULL;
			}
			else
			{
				if (isSameSize(entryID->dimSizeList, sizesList) == 0)
				{
					fprintf(yySemanticOut, "Error- Array %s out of bounds at line %d. \n", entryID->name, curr_token->lineNumber);
					paramItem = NULL;
				}
				else if (isSameSize(entryID->dimSizeList, sizesList) == 2)
				{
					fprintf(yySemanticOut, "Error- Access to wrong dimension of the array %s in line %d. \n", entryID->name, curr_token->lineNumber);
					paramItem = NULL;
				}
				else
				{
					paramItem->tokenName = entryID->tokenName;
					paramItem->name = entryID->name;
				}
			}
		}
		else
		{
			paramItem = NULL;
		}
		match(rightBrackets_Token);
		break;
	}
	case leftParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR’ -> (ARGS) )\n");
		if (entryID != NULL)
		{
			tableItemEntry = entryID;
			if (entryID->typeOfID != FUNC) 
			{
				fprintf(yySemanticOut, "Error- The assignment into variable/array %s at line %d is not valid. \n", entryID->name, curr_token->lineNumber);
				parse_ARGS();
			}
			else
			{
				paramItem->tokenName = entryID->tokenName;
				paramList = parse_ARGS();
				if (isEqualParamAmount(tableItemEntry->listParameterTypes, paramList) == 0)
					fprintf(yySemanticOut, "Error- Wrong call to function %s in line %d. \n", tableItemEntry->name, curr_token->lineNumber);
			}
		}
		else
		{
			parse_ARGS();
		}
		match(rightParentheses_Token);
		break;
	}
	case arOpMul_Token:
	case arOpAdd_Token:
	case relOpBiggerEqualThan_Token:
	case relOpBiggerThan_Token:
	case relOpEqual_Token:
	case relOpNotEqual_Token:
	case relOpSmallerEqualThan_Token:
	case relOpSmallerThan_Token:
	case semiColon_Token:
	case comma_Token:
	case rightCurlyBraces_Token:
	case rightBrackets_Token:
	case rightParentheses_Token:
	{
		fprintf(yyoutParser, "Rule (FACTOR’ -> EPSILON )\n");
		if (entryID != NULL)
		{
			if (entryID->typeOfID != VAR) 
			{
				fprintf(yySemanticOut, "Error- The assignment into function/array %s at line %d is not valid. \n", entryID->name, curr_token->lineNumber);
				paramItem = NULL;
			}
			else
			{
				paramItem->name = (char*)(calloc(strlen(entryID->name), sizeof(char)));
				paramItem->name = entryID->name;
				paramItem->tokenName = entryID->tokenName;
				paramItem->val = entryID->val;
			}
		}
		else
		{
			paramItem = NULL;
		}
		curr_token = back_token();
		break;
	}
	default:
	{
		printUnexpectedToken("leftBrackets_Token/leftParentheses_Token/arOpMul_Token/arOpAdd_Token/relOpBiggerEqualThan_Token/relOpBiggerThan_Token/relOpEqual_Token/comma_Token/relOpNotEqual_Token/relOpSmallerEqualThan_Token/relOpSmallerThan_Token/semiColon_Token/rightCurlyBraces_Token/rightBrackets_Token/rightParentheses_Token");
		while (curr_token->kind != arOpMul_Token &&
			curr_token->kind != arOpAdd_Token &&
			curr_token->kind != relOpBiggerEqualThan_Token &&
			curr_token->kind != relOpBiggerThan_Token &&
			curr_token->kind != relOpEqual_Token &&
			curr_token->kind != relOpNotEqual_Token &&
			curr_token->kind != relOpSmallerEqualThan_Token &&
			curr_token->kind != relOpSmallerThan_Token &&
			curr_token->kind != semiColon_Token &&
			curr_token->kind != comma_Token &&
			curr_token->kind != rightCurlyBraces_Token &&
			curr_token->kind != rightBrackets_Token &&
			curr_token->kind != rightParentheses_Token &&
			curr_token->kind != EOF_Token)
		{
			curr_token = next_token();
		}
		if (curr_token->kind == EOF_Token) {
			return;
		}
		curr_token = back_token();
		break;
	}
	}
	return paramItem;
}

//check expected token kind with the actual token kind
//return true if equals, false otherwise
int match(eTOKENS kind)
{
	curr_token = next_token();
	if (curr_token->kind != kind) {
		printUnexpectedToken(enumStringTranslator(kind));
		return 0;
	}
	return 1;
}
//function for print errors
void printUnexpectedToken(char* expectedTokens)
{
	fprintf(yyoutParser, "Expected token of type '%s' at line: '%d', Actual token of type '%s', lexeme: '%s'.\n",
		expectedTokens,
		curr_token->lineNumber,
		enumStringTranslator(curr_token->kind),
		curr_token->lexeme);
}
