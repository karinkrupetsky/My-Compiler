#pragma once

#include"SymbolTable.h"

//////////////////////////////////////////////////////////////
FILE* yyoutParser;
Token* curr_token;
FILE* yySemanticOut;


//////////////////////////////////////////////////////////////

//enum of variables of the grammar
typedef enum eVariables
{
	PROG,
	GLOBAL_VARS,
	GLOBAL_VARS_TAG,
	VAR_DEC,
	VAR_DEC_TAG,
	TYPE,
	DIM_SIZES,
	DIM_SIZES_TAG,
	FUNC_PREDEFS,
	FUNC_PREDEFS_TAG,
	FUNC_PROTOTYPE,
	FUNC_FULL_DEFS,
	FUNC_FULL_DEFS_TAG,
	FUNC_WITH_BODY,
	RETURNED_TYPE,
	PARAMS,
	PARAM_LIST,
	PARAM_LIST_TAG,
	PARAM,
	PARAM_TAG,
	COMP_STMT,
	VAR_DEC_LIST,
	STMT_LIST,
	STMT_LIST_TAG,
	STMT,
	STMT_TAG,
	IF_STMT,
	CALL,
	ARGS,
	ARG_LIST,
	ARG_LIST_TAG,
	RETURN_STMT,
	RETURN_STMT_TAG,
	VAR,
	VAR_TAG,
	EXPR_LIST,
	EXPR_LIST_TAG,
	CONDITION,
	EXPR,
	EXPR_TAG,
	TERM,
	TERM_TAG,
	FACTOR,
	FACTOR_TAG
}eVariables;

//////////////////////////////////////////////////////////////

//Parse functions for each variable
void parse();
void parse_PROG();
void parse_GLOBAL_VARS();
void parse_GLOBAL_VARS_TAG();
void parse_VAR_DEC();
void parse_VAR_DEC_TAG();
void parse_TYPE();
DimSizeList* parse_DIM_SIZES();
DimSizeList* parse_DIM_SIZES_TAG();
void parse_FUNC_PREDEFS();
void parse_FUNC_PREDEFS_TAG();
void parse_FUNC_PROTOTYPE();
void parse_FUNC_FULL_DEFS();
void parse_FUNC_FULL_DEFS_TAG();
void parse_FUNC_WITH_BODY();
void parse_RETURNED_TYPE();
ParamList* parse_PARAMS();
ParamList* parse_PARAM_LIST();
ParamList* parse_PARAM_LIST_TAG();
ParamItem* parse_PARAM();
DimSizeList* parse_PARAM_TAG();
void parse_COMP_STMT();
void parse_VAR_DEC_LIST();
void parse_STMT_LIST();
void parse_STMT_LIST_TAG();
void parse_STMT();
void parse_STMT_TAG();
void parse_IF_STMT();
void parse_CALL();
ParamList* parse_ARGS();
ParamList* parse_ARG_LIST();
ParamList* parse_ARG_LIST_TAG();
void parse_RETURN_STMT();
ParamItem* parse_RETURN_STMT_TAG();
void parse_VAR();
ParamItem* parse_VAR_TAG();
DimSizeList* parse_EXPR_LIST();
DimSizeList* parse_EXPR_LIST_TAG();
void parse_CONDITION();
ParamItem* parse_EXPR();
ParamItem* parse_EXPR_TAG();
ParamItem* parse_TERM();
ParamItem* parse_TERM_TAG();
ParamItem* parse_FACTOR();
ParamItem* parse_FACTOR_TAG();

//////////////////////////////////////////////////////////////

//check expected token kind with the actual token kind
//return true if equals, false otherwise
int match(eTOKENS kind);

//function for print errors
void printUnexpectedToken(char* unexpectedTokens);
