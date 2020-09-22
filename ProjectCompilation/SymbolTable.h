#pragma once
#include "Token.h"
#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H



typedef struct dimSizeItem {
	int size;
	struct dimSizeItem* next;
}dimSizeItem;


typedef struct DimSizeList {
	struct dimSizeItem* top;
}DimSizeList;


typedef struct LocalVar {
	char* name;
	int countOfLocal;
}LocalVar;


typedef struct GlobalVar {
	char* name;
	int countOfGlobal;
}GlobalVar;



typedef struct ParamItem
{
	char* name;
	eTOKENS tokenName;
	float val;
	struct DimSizeList* dimSize;
	int typeOfID;
	struct ParamItem* next;
	struct ParamItem* prev;
}ParamItem;

typedef struct ParamList
{
	struct ParamItem* top;
}ParamList;

typedef struct SymbolTableItem
{
	struct SymbolTableItem* next;
	struct SymbolTableItem* prev;
	char* name;
	eTOKENS tokenName;
	DimSizeList* dimSizeList;
	struct ParamList* listParameterTypes;
	int flag;
	int typeOfID;
	int IsDeclareted;
	float val;
}SymbolTableItem;

typedef struct SymbolTable
{
	struct SymbolTable* root;
	struct SymbolTableItem* top;
	int typeOfID;
}SymbolTable;



SymbolTable* makeTable(SymbolTable* current_table);
SymbolTable* popTable(SymbolTable* current_table);
SymbolTableItem* lookup(SymbolTable* current_table, char* id_name);
SymbolTableItem* createTableEntry(SymbolTable* current_table);
SymbolTableItem* find(SymbolTable* current_table, char* id_name);
SymbolTableItem* insert(SymbolTable* current_table, char* id_name);
void insertDimSizeList(DimSizeList* list, int size);
int isEqualParamAmount(ParamList* list1, ParamList* list2);
int isExistsParam(ParamList* paramList, ParamItem* paramNode); int isSameParamsCount(ParamList* list1, ParamList* list2);
int isSameParamsCount(ParamList* list1, ParamList* list2);
int isSameSize(DimSizeList* size, DimSizeList* sizeSmall);
int sizeEqualCheck(DimSizeList* size1, DimSizeList* size2);


#endif