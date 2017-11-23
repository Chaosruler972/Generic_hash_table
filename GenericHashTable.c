
#include "GenericHashTable.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Table* createTable(int size, int dType, int listLength)
{
	if(size<1 || listLength < 1)
		return NULL;
	if(dType!=INT_TYPE && dType!=STR_TYPE)
	{
		return NULL;
	}
	Table* table = (Table*)malloc( sizeof(Table) );
	if(!table)
	{
		return NULL;
	}
	table->type = dType;
	table->origSize= size;
	table->rows = size;
	table->length = listLength;
	table->multi = 1; // size / originalsize = 1
	table->arr = (Object**)malloc(sizeof(Object*) * (table->rows));
	if(!table->arr)
	{
		free(table); 
		return NULL;
	}
	int i;
	for(i=0; i<table->rows; i++) // verifies all heads are nul
	{
		table->arr[i] = NULL;
	}
	return table;
}

void freeTable(Table* table)
{
	if(table == NULL)
		return;
	int i;
	for(i=0; i<( table->rows);i++)
	{
		freeList(table->arr[i], table->type);
	}
	free(table->arr);
	table->arr = NULL;
	free(table);
	table = NULL;
	return;
}

int add(Table* table, void* data)
{
	if(!table || !data)
		return -1;
	int data_type = table->type;
	int ind;
	int i;
	if(data_type == INT_TYPE)
	{
		if(*(int*) data < 0)
		{
			return -1;
		}
		int* int_ptr = data;
		ind = intHashFun(int_ptr, table->origSize);
	}
	else if(data_type == STR_TYPE)
	{
		char* char_ptr = (char*) data;
		ind = strHashFun(char_ptr, table->origSize);
	}
	else
	{
		return -1;
	}
	if(ind == -1)
			return -1;
	Object* obj; // the object to add...
	if(data_type == INT_TYPE)
	{
		obj = createObject(data);
	}
	else // STR_TYPE -> allocates dynamicilly in that csae
	{
		char* old_data = (char*) data;
		char* new_char_ptr = (char*) malloc (sizeof(strlen(old_data)) * sizeof(char) ); // allocates a new string, verifies data
		if(!new_char_ptr || !strlen(old_data))
			return  -1;
		strcpy(new_char_ptr,old_data);
		obj = createObject((void*) new_char_ptr);
	}
	int d = table->multi;
	for(i=ind * d; i<(ind+1)* d; i++) // searches for a relevent head to 
	{
		if(!table->arr[i]) // if head is empty, add to it
		{
			table->arr[i] = obj;
			obj->index = 0;
			return i;
		}
		else
		{

			if(addToObject(table->arr[i], obj, table->length)) // find an empty place at the head link list
			{
				return i;
			}
	
		}
	}
	DoubleList(table); 
	table->arr[((ind+1) * table->multi) - 1] = obj; // get the rightest object at the main index
	obj->index = 0;
	return ((ind+1) * table->multi) - 1;
}

int removeObj(Table* table, void* data)
{
	if(!data || !table)
		return -1;
	int i;
	int index;
	Object* obj;
	if(table->type == INT_TYPE)
	{
		index = intHashFun( (int*) data, table->origSize);
	}
	else if(table->type == STR_TYPE)
	{
		index = strHashFun( (char*) data, table->origSize);
	}
	else
	{
		return -1;
	}
	if(index == -1)
			return -1;
	for(i=index*table->multi; i<index*table->multi+table->multi; i++)
	{
		obj = FindObject(table->arr[i],data,table->type);
		if(obj)
		{
			int object_index = obj->index;
			if(!object_index)
			{
				table->arr[i] = obj->next;
			}
			else
			{
				Object* prev = table->arr[i];
				int j=0;
				for(j=0; j<object_index-1; j++)
				{
					prev = prev->next;
				}
				prev->next = obj->next;
			}
			decreaseIndex(obj->next);
			freeObject(obj, table->type);
			return i;
		}
	}
	return -1;
}

Object* search(Table* table, void* data)
{
	if(!data || !table)
		return NULL;
	int i;
	int index;
	Object* obj;
	if(table->type == INT_TYPE )
	{
		index = intHashFun((int*) data, table->origSize);
	}
	else if(table->type == STR_TYPE)
	{
		index = strHashFun( (char*) data, table->origSize);
	}
	else
	{
		return NULL;
	}
	if(index == -1)
			return NULL;
	for(i=index*table->multi; i<index*table->multi+table->multi; i++)
	{
		obj = FindObject(table->arr[i], data, table->type);
		if(obj)
		{
			printf("%d\t%d\n", i, (obj->index)+1);
			return obj;
		}
	}
	return NULL;
}


void printTable(Table* table)
{
	if(!table)
		return;
	int i;
	for(i=0; i<table->rows;i++)
	{
		printf("[%d]",i);
		PrintList(table->arr[i],table->type);
		printf("\n");
	}
}

Object* createObject(void* data)
{
	if(!data)
		return NULL;
	Object* obj = (Object*) malloc (sizeof(Object));
	if(!obj)
	{
		return NULL;
	}
	obj->data = data;
	obj->next = NULL;
	obj->index = 0;
	return obj;
}

void freeObject(Object* obj, int type)
{
	if(!obj)
		return;
	if(type == INT_TYPE)
	{
		free(obj);
	}
	else if(type == STR_TYPE)
	{
		free(obj->data); 
		free(obj);
	}
}

int isEqual(int type, void* data1, void* data2)
{
	if(!data1 || !data2)
		return -1;
	if(type == INT_TYPE)
	{
		if(*(int*) data1 == *(int*) data2)
		{
			//printf("Object 1 is %d, object 2 is %d EQUAL\n",*(int*) data1, *(int*) data2);
			return 0;
		}
		else 
		{
			//printf("Object 1 is %d, object 2 is %d UNEQUAL\n",*(int*) data1, *(int*) data2);
			return 1;
		}
	}
	else if(type == STR_TYPE)
	{
		if(!data1 || !data2)
			return -1;
		char* d1 = data1;
		char* d2 = data2;
		if(strcmp(d1,d2)==0)
			return 0;
		else
			return 1;
	}
	else
	{
		return -1;
	}
}

int intHashFun(int* key, int origSize)
{
	if(!key)
		return -1;
	return HashFun(*key,origSize);
}

int strHashFun(char* key, int origSize)
{
	if(!key)
		return -1;
	if(!strlen(key))
		return -1;
	int number = 0;
	int i;
	for(i=0 ; i<strlen(key) ; i++)
	{
		number+=(int) key[i];
	}
	return HashFun(number,origSize);
}

int HashFun(int number, int origSize)
{
	if(number<0)
		return -1;
	return number%origSize;
}


void PrintObj(Object* obj, int type)
{
	if(!obj)
		return;
	if(type == INT_TYPE)
	{
		printf("%d", *(int*) obj->data);
	}
	else if(type == STR_TYPE)
	{
		char* char_ptr = obj->data;
		printf("%s", char_ptr);
	}
}

void freeList(Object* obj, int type)
{
	if(!obj)
		return;
	freeList(obj->next,type);
	freeObject(obj,type);
	obj = NULL;
}

void PrintList(Object* obj,int type)
{
	if(!obj)
		return;
	printf("\t");
	if(obj->index)
	{
		printf("->\t");
	}
	PrintObj(obj,type);
	PrintList(obj->next,type);
}

int addToObject(Object* source, Object* ptr, int length)
{
	if(!source || !ptr)
		return -1;
	if(!source->next)
	{
		if(source->index + 1 != length) // there's a place here
		{
			source->next = ptr;
			ptr->index = source->index + 1;
			return 1;
		}
		else // too big of a linked list
		{
			return 0;
		}
	}
	else
		return addToObject(source->next,ptr,length); 
}

void DoubleList(Table* table)
{
	if(!table)
		return;
	Object** newarr = (Object**) malloc( sizeof(Object*) * table->rows*2);
	if(!newarr)
		return;
	int i;
	for(i=0; i<table->rows*2; i++) // callocs
	{
		newarr[i] = NULL;
	}
	for(i=0; i< table->rows; i++) // copies heads
	{
		newarr[i*2] = table->arr[i];
	}
	table->rows *=2; 
	table->multi*=2;
	free(table->arr);
	table->arr = newarr;
}

Object* FindObject(Object* head, void* data, int type)
{
	if(!head || !data)
		return NULL;
	if(!isEqual(type,data,head->data))
		return head;
	return FindObject(head->next, data, type);
	
}


void decreaseIndex(Object* head)
{
	if(!head)
		return;
	head->index--;
	decreaseIndex(head->next);
}
