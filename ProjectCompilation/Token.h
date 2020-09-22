#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern FILE* yyin, * yyout;

typedef enum eTOKENS
{
	arOpAdd_Token,
	arOpMul_Token,
	assignOp_Token,
	relOpSmallerThan_Token,
	relOpSmallerEqualThan_Token,
	relOpBiggerThan_Token,
	relOpBiggerEqualThan_Token,
	relOpEqual_Token,
	relOpNotEqual_Token,
	intKW_Token,
	floatKW_Token,
	voidKW_Token,
	ifKW_Token,
	returnKW_Token,
	ID_Token,
	intNum_Token,
	floatNum_Token,
	comma_Token,
	colon_Token,
	semiColon_Token,
	leftParentheses_Token,
	rightParentheses_Token,
	leftBrackets_Token,
	rightBrackets_Token,
	leftCurlyBraces_Token,
	rightCurlyBraces_Token,
	comments_Token,
	EOF_Token
}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char* lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token* tokensArray;
	struct Node* prev;
	struct Node* next;
}Node;

void create_and_store_token(eTOKENS kind, char* lexeme, int numOfLine);
Token* next_token();
Token* back_token();
char* enumStringTranslator(eTOKENS kind);
void repoint_to_flag();
void freeNode();
