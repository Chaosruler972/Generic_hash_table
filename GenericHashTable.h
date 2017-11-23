#ifndef GENERIC_H
#define GENERIC_H
#define INT_TYPE 0
#define STR_TYPE 1

typedef struct Object
{
	void* data; // the data, on STRING it's a pointer to the begining of the String, on int it's a masked pointer that doesn't really point anywhere but holds the data itself
	struct Object* next;
	int index; // the index INSIDE the linked list, not the array index
} Object;

typedef struct Table
{
	Object** arr;
	int rows; // aka size, the size of the array holding the heads of the linked lists, since it's an array I called it rows
	int origSize; // the original size given, used to compute hash and copyin the array
	int length; // the list length, meaning the entire data count is rows * length
	int multi; // simply saying how bigger did our array got since it was first created, multi * origSize = rows
	int type; // the type of data this table consists
}Table;
/**
* The function gets the original size and the type of the data in the table elements.
* it initializes the Table struct members.
* On success, the function returns a pointer to the new created Table, otherwise, it return NULL.
*/
Table* createTable(int size, int dType, int listLength);
/**
* The function release all the allocated members of struct Table.
*/
void freeTable(Table* table);

/**
* The function adds data to the hashtable (as described in the exe definition)
* On success, the function returns the array index of the added data, otherwise, it return -1.
*/
int add(Table* table, void* data);

/**
* The function removes the Object which its data equals to data, if there are more than one, it removes the first one.
* On success, the function returns the array index of the removed data, otherwise, it returns -1.
* -1 is also returned in the case where there is no such object.
*/
int removeObj(Table* table, void* data);

/**
* The function search for an object that its data is equal to data and returns a pointer to that object.
* If there is no such object or in a case of an error, NULL is returned.
*/
Object* search(Table* table, void* data);

/**
* The function print the table (the format is in the exe definition)
*/
void printTable(Table* table);

/**
* This function creates an object and return the pointer to it or NULL if there is some error.
*/
Object* createObject(void* data);

/**
* This function frees an object,  note the in the case of STR_TYPE, the data
* of the object should also be freed.
*/
void freeObject(Object* obj, int type);

/**
* check the equality of the data of two objects. The implementation is different depending the type of the data.
* the function returns 0 if they are equal or some other value if they are not equal.
*/
int isEqual(int type, void* data1, void* data2);

/**
* returns the hash value of an integer, which is key mod origSize
*/
int intHashFun(int* key, int origSize);

/**
* returns the hash value of an string, which is m mod origSize, where m is the sum of the ascii value of all the
* character in key.
*/
int strHashFun(char* key, int origSize);

/**
* returns the hash value of a number, the number is representing the computed value of any number or string
*/
int HashFun(int number, int origSize);
/**
* prints an Object type's internal data, sent along with a data type indicator
*/
void PrintObj(Object* obj, int type);
/**
* recursive method to free an entire list, given a head and type
*/
void freeList(Object* obj, int type);
/**
* recursive method to print an entire list, given a head and type
*/
void PrintList(Object* obj, int type);
/**
* recursive method to attempt to add a specific Object into a specific list given the list length at length, the method returns 0 on failure and 1 on success
*/
int addToObject(Object* source, Object* ptr, int length);
/**
* doubles the array on a given table and manages copying the lists into index -> index*2
*/
void DoubleList(Table* table);
/**
* recursive method to find a specific object at a given list with its given head and it's given type... a copy of the object's data is sent too, returns NULL on failure
*/
Object* FindObject(Object* head, void* data, int type);
/**
*
* recursive method to decrease all the indexes after the head sent by one, used to make sure the computed indexes are accurate upon removal of an Object
(ie: indexes 0,1,2,3 if 2 is removed, indexes become 0,1,3... so 3 is sent to decrease itself to 2)
*/
void decreaseIndex(Object* head);
#endif
