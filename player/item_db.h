#ifndef __ITEM_DB_H__
#define __ITEM_DB_H__

#include <stdlib.h>
#include "../serialization/serialization_reader.h"
#include "../serialization/serializer.h"

#define MAX_ITEMS 64
#define ITEM_HEADER_0 'M'
#define ITEM_HEADER_1 'S'
#define ITEM_VERSION 01
#define DELIM "ITEMS"

typedef struct {
	short id;
	char* name;
	char* description;
} item_t;

typedef struct {
	item_t* items;
} item_db_t;

item_db_t* item_db_create(); //creates and loads the item db

item_t item_db_get_item_by_id(item_db_t* db, short id); //this one's easy

void item_db_destroy(item_db_t* db);

int item_db_write_to_file(item_db_t* db, const char* filename);
item_db_t* item_db_read_from_file(const char* filename);
int item_db_verify_header(char* buffer, short version, char* delim);

item_db_t* item_db_create_test_db();

#endif //__ITEM_DB_H__