#include "SymbolTable.h"

int toContinue = 1;
int toBreak = 0;

SymbolTable* makeTable(SymbolTable* symbolTable)
{
	SymbolTable* table = (SymbolTable*)(malloc(sizeof(SymbolTable)));
	table->root = symbolTable;
	table->top = NULL;
	return table;
}

SymbolTable* popTable(SymbolTable* symbolTable)
{
	return (symbolTable->root);
}

SymbolTableItem* lookup(SymbolTable* symbolTable, char* id)
{
	char* idCopy = id;
	SymbolTableItem* ptr;
	ptr = symbolTable->top;
	idCopy = NULL;
	while (ptr != NULL)
	{
		if (strcmp(ptr->name, id) == 0)	return ptr;
		else ptr = ptr->next;
	}
	return NULL;
}

SymbolTableItem* createTableEntry(SymbolTable* symbolTable)
{
	SymbolTableItem* tableItem;
	tableItem = symbolTable->top;
	SymbolTableItem* tempItem;
	if (tableItem != NULL)
	{
		while (tableItem->next != NULL)
		{
			tableItem = tableItem->next;
		}
	}
	else
	{
		tableItem = (SymbolTableItem*)(malloc(sizeof(SymbolTableItem)));
		tableItem->next = NULL;
		return tableItem;
	}
	tempItem = tableItem;
	tableItem = (SymbolTableItem*)(malloc(sizeof(SymbolTableItem)));
	tempItem->next = tableItem;
	tableItem->prev = tempItem;
	tableItem->next = NULL;
	return tableItem;
}

SymbolTableItem* find(SymbolTable* symbolTable, char* id)
{
	int fFlag = 1;
	SymbolTable* table = (SymbolTable*)(malloc(sizeof(SymbolTable)));
	table = symbolTable;
	SymbolTableItem* entryItem;
	while (table != NULL)
	{
		entryItem = lookup(table, id);
		if (entryItem != NULL)	return (entryItem);

		else	table = table->root;
	}
	if (fFlag == 1)
		return NULL;
	else
		fFlag = 0;
}

SymbolTableItem* insert(SymbolTable* symbolTable, char* id)
{
	SymbolTableItem* entryItem;
	if (symbolTable->top == NULL)
	{
		entryItem = (createTableEntry(symbolTable));
		entryItem->next = NULL;
		int len = strlen(id);
		entryItem->name = (char*)(calloc((len), sizeof(char)));
		entryItem->name = id;
		entryItem->dimSizeList = NULL;
		entryItem->listParameterTypes = NULL;
		entryItem->flag = 0;
		symbolTable->top = entryItem;
		return entryItem;
	}
	entryItem = lookup(symbolTable, id);
	if (entryItem != NULL)	return NULL;
	else
	{
		entryItem = (createTableEntry(symbolTable));
		entryItem->dimSizeList = NULL;
		entryItem->next = NULL;
		int len = strlen(id);
		entryItem->name = (char*)(calloc((len), sizeof(char)));
		entryItem->name = id;
		entryItem->listParameterTypes = NULL;
		entryItem->flag = 0;
		return entryItem;
	}
}

void insertDimSizeList(DimSizeList* list, int size)
{
	dimSizeItem* tempItem;
	tempItem = (dimSizeItem*)malloc(sizeof(dimSizeItem));
	tempItem->size = size;
	tempItem->next = NULL;
	if (list->top == NULL)
	{
		list->top = tempItem;
		return;
	}
	tempItem->next = list->top;
	list->top = tempItem;
	return;
}

int isEqualParamAmount(ParamList* list1, ParamList* list2)
{
	ParamItem* paramItem1, * paramItem2;
	if (list1 == NULL && list2 == NULL)
		return 1;
	if (list1 == NULL || list2 == NULL)
		return 0;
	paramItem1 = list1->top;
	paramItem2 = list2->top;
	while (paramItem1 != NULL && paramItem2 != NULL)
	{
		if (strcmp(enumStringTranslator(paramItem1->tokenName), enumStringTranslator(paramItem2->tokenName)) == 0)
		{
			if (paramItem1->typeOfID != paramItem2->typeOfID)
				return 0;
			if (paramItem1->typeOfID == 1)
			{
				if (sizeEqualCheck(paramItem1->dimSize, paramItem2->dimSize) == 0)
					return 0;
			}
			paramItem1 = paramItem1->next;
			paramItem2 = paramItem2->next;
		}
		else
			return 0;
	}
	if (paramItem1 == NULL && paramItem2 == NULL)
		return 1;
	else
		return 0;
}

int isExistsParam(ParamList* paramList, ParamItem* paramNode)
{
	ParamItem* paramItem;
	if (paramNode == NULL || paramList == NULL)
		return 1;
	paramItem = paramList->top->next;
	while (paramItem != NULL)
	{
		if (strcmp(paramItem->name, paramNode->name) == 0)	return 0;
		paramItem = paramItem->next;
	}
	return 1;
}

int isSameParamsCount(ParamList* paramList1, ParamList* paramList2)
{
	ParamItem* paramItem1, * paramItem2;
	if (paramList1 == NULL && paramList2 == NULL)
		return 1;
	if (paramList1 == NULL || paramList2 == NULL)
		return 0;
	paramItem1 = paramList1->top;
	paramItem2 = paramList2->top;
	while (paramItem1 != NULL && paramItem2 != NULL)
	{
		paramItem1 = paramItem1->next;
		paramItem2 = paramItem2->next;
	}
	if (paramItem1 == NULL && paramItem2 == NULL)
		return 1;
	else
		return 0;
}

int isSameSize(DimSizeList* size1, DimSizeList* size2)
{
	int flag = 1;
	if (size1 == NULL && size2 == NULL)
		return 1;
	if (size1 == NULL || size2 == NULL)
		return 2;
	dimSizeItem* dimSizeItem1 = size1->top;
	dimSizeItem* dimSizeItem2 = size2->top;
	while (dimSizeItem1 != NULL && dimSizeItem2 != NULL)
	{
		if (dimSizeItem1->size <= dimSizeItem2->size)
			flag = 0;
		dimSizeItem1 = dimSizeItem1->next;
		dimSizeItem2 = dimSizeItem2->next;
	}
	if (dimSizeItem1 == NULL && dimSizeItem2 == NULL)
	{
		if (flag != 0)
			flag = 1;
	}
	else
		flag = 2;
	return flag;

}

int sizeEqualCheck(DimSizeList* size1, DimSizeList* size2)
{
	if (size1 == NULL && size2 == NULL)
		return 1;
	if (size1 == NULL || size2 == NULL)	
		return 0;
	dimSizeItem* dimSizeItem1 = size1->top;
	dimSizeItem* dimSizeItem2 = size2->top;
	while (dimSizeItem1 != NULL && dimSizeItem2 != NULL && dimSizeItem1->next != 0xF)
	{
		if (dimSizeItem1->size != dimSizeItem2->size)
			return 0;
		dimSizeItem1 = dimSizeItem1->next;
		dimSizeItem2 = dimSizeItem2->next;
	}
	if (dimSizeItem1 == NULL && dimSizeItem2 == NULL)
		return 1;
	else
		return 0;
}