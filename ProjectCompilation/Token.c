#include "Token.h"

/* This package describes the storage of tokens identified in the input text.
* The storage is a bi-directional list of nodes.
* Each node is an array of tokens; the size of this array is defined as TOKEN_ARRAY_SIZE.
* Such data structure supports an efficient way to manipulate tokens.

There are three functions providing an external access to the storage:
- function create_and_store_tokens ; it is called by the lexical analyzer when it identifies a legal token in the input text.
- functions next_token and back_token; they are called by parser during the syntax analysis (the second stage of compilation)
*/

int currentIndex = 0;
Node* currentNode = NULL;
Node* flag = NULL; //For self implementation of back and next token

#define TOKEN_ARRAY_SIZE 1000

/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine)
{
	int length = strlen(lexeme) + 1;

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
		flag = currentNode;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)calloc(sizeof(Token), TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}

	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;

	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char) * length);
#ifdef _WIN32
	strcpy_s(currentNode->tokensArray[currentIndex].lexeme, length, lexeme);
#else
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
#endif		
}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token* back_token() {
	if (currentIndex == -1) {
		return NULL;
	}
	if (currentIndex == 0) {
		if (currentNode == flag) {
			currentIndex--;
			return NULL;
		}
		currentNode = currentNode->prev;
		currentIndex = TOKEN_ARRAY_SIZE;
	}
	currentIndex--;
	return &currentNode->tokensArray[currentIndex];
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/
Token* next_token() {
	if (currentIndex + 1 == TOKEN_ARRAY_SIZE)
	{
		if (currentNode->next != NULL) {
			currentNode = currentNode->next;
			currentIndex = -1;
		}
		else {
			return NULL;
		}
	}
	if (currentNode->tokensArray[currentIndex + 1].lineNumber != 0)
	{
		currentIndex++;
		return  &currentNode->tokensArray[currentIndex];
	}
	return  &currentNode->tokensArray[currentIndex];

}

//Point to the first of the section.
void repoint_to_flag() {
	if (flag != NULL) {
		currentNode = flag;
		currentIndex = -1;
	}
}

// We use this func to clean the previous data structure before we check anothe test file.
void freeNode() {
	free(currentNode);
	currentNode = NULL;
	currentIndex = 0;
}
//For print the unexpected tokens, parse token to string.
char* enumStringTranslator(int kind)
{
	switch (kind)
	{
	case arOpAdd_Token:
		return "arOpAdd_Token";
	case arOpMul_Token:
		return "arOpMul_Token";
	case assignOp_Token:
		return "assignOp_Token";
	case relOpSmallerThan_Token:
		return "relOpSmallerThan_Token";
	case relOpSmallerEqualThan_Token:
		return "relOpSmallerEqualThan_Token";
	case relOpBiggerThan_Token:
		return "relOpBiggerThan_Token";
	case relOpBiggerEqualThan_Token:
		return "relOpBiggerEqualThan_Token";
	case relOpEqual_Token:
		return "relOpEqual_Token";
	case relOpNotEqual_Token:
		return "relOpNotEqual_Token";
	case intKW_Token:
		return "intKW_Token";
	case floatKW_Token:
		return "floatKW_Token";
	case voidKW_Token:
		return "voidKW_Token";
	case ifKW_Token:
		return "ifKW_Token";
	case returnKW_Token:
		return "returnKW_Token";
	case ID_Token:
		return "ID_Token";
	case intNum_Token:
		return "intNum_Token";
	case floatNum_Token:
		return "floatNum_Token";
	case comma_Token:
		return "comma_Token";
	case colon_Token:
		return "colon_Token";
	case semiColon_Token:
		return "semiColon_Token";
	case leftParentheses_Token:
		return "leftParentheses_Token";
	case rightParentheses_Token:
		return "rightParentheses_Token";
	case leftBrackets_Token:
		return "leftBrackets_Token";
	case rightBrackets_Token:
		return "rightBrackets_Token";
	case leftCurlyBraces_Token:
		return "leftCurlyBraces_Token";
	case rightCurlyBraces_Token:
		return "rightCurlyBraces_Token";
	case comments_Token:
		return "comments_Token";
	case EOF_Token:
		return "EOF_Token";
	default:
		return "Unknown Token";
	}
}