#include "item_db.h"

#include "../common.h"

item_db_t* item_db_create()
{
	item_db_t* return_value = malloc(sizeof(item_db_t));
	return_value->items = malloc(sizeof(item_t) * MAX_ITEMS); //this is gonna be fun
	int i ;
	for(i = 0; i < MAX_ITEMS; i++)
	{
		return_value->items[i].id = -1; //means null item which means we don't even LOOK at the other properties. phew. prevents crashes this way.
	}
	return return_value;
}

item_t item_db_get_item_by_id(item_db_t* db, short id)
{
	if(db != NULL)
	{
		if(db->items[id].id == -1)
		{
			item_t null_item_lol = { -1, "null", "no description" };
		}	
		else
			return db->items[id];
	}
}

void item_db_destroy(item_db_t* db)
{
	int i = 0;
	for(i = 0; i < MAX_ITEMS; i++)
	{
		if(db->items[i].name != NULL)
			free(db->items[i].name);
		if(db->items[i].description != NULL)
			free(db->items[i].description);
	}
	
	free(db);
	db = NULL;
}

int item_db_write_to_file(item_db_t* db, const char* filename)
{
	int filesize = (sizeof(char) * 2) + //header
		(sizeof(short)) + //version
		(strlen(DELIM)) + //the delim
		(sizeof(short) * MAX_ITEMS) + //each ID
		((sizeof(char) * 24) * MAX_ITEMS) + //max length 24 for item strings
		((sizeof(char) * 64) * MAX_ITEMS); //max length 64 for item description
	 char* buffer = malloc(sizeof(char) * filesize);
	 int pointer = 0, i;
     serializer_write_char(buffer, &pointer, ITEM_HEADER_0);
     serializer_write_char(buffer, &pointer, ITEM_HEADER_1);

     serializer_write_short(buffer, &pointer, (short)ITEM_VERSION);
	 serializer_write_string(buffer, &pointer, DELIM); //this is how we KNOW it's an item db file and not just a weird older level file.
	 
	 for(i = 0; i < MAX_ITEMS; i++)
	 {
		 if(db->items[i].id == -1)
		 {
			 serializer_write_short(buffer, &pointer, (short)-1);
			 serializer_write_string(buffer, &pointer, "null item");
			 serializer_write_string(buffer, &pointer, "no description");
			 break;
		 }
		 else
		 {
		 	serializer_write_short(buffer, &pointer, (short)db->items[i].id);
		 	if(strlen(db->items[i].name) > 24)
			 	serializer_write_string(buffer, &pointer, "too long");
		 	else
			 	serializer_write_string(buffer, &pointer, db->items[i].name);
		 
		 	if(strlen(db->items[i].description) > 64)
			 	serializer_write_string(buffer, &pointer, "description too long");
		 	else
		 		serializer_write_string(buffer, &pointer, db->items[i].description);
	 	 }
	 }
	 
	 int return_value = serializer_write_to_file(buffer, filesize, filename);
	 
	 free(buffer);
	 
	 return return_value;
}

int item_db_verify_header(char* buffer, short version, char* delim)
{
	char filler[32];
	
	if(buffer[0] == ITEM_HEADER_0 && buffer[1] == ITEM_HEADER_1)
	{
		//header matches!
		printf("header matches\n");
		if(version == ITEM_VERSION)
		{
			//version matches!
			if(strcmp(delim, DELIM) == 0)
			{
				return 1;
			}
			else
			{
				report_fatal_error("Delim mismatch.");
			}
		}
		else
			report_fatal_error("Version mismatch.");
	}
	else
	{
		sprintf(filler, "Header mismatch in level file. (got %c%c, expected %c%c)",
			buffer[0], buffer[1], ITEM_HEADER_0, ITEM_HEADER_1);
		report_fatal_error(filler);
	}
	
	return 0;
}

item_db_t* item_db_read_from_file(const char* filename)
{
	int file_size = serializer_get_file_size(filename);
	printf("filesize: %d\n", file_size);
	if(file_size > 0)
	{
		char* buffer = malloc(sizeof(char) * file_size);
		serializer_read_from_file(buffer, file_size, filename);
		int pointer = 0;
		
		char HEADER[2] = {0};
		
		HEADER[0] = serializer_read_char(buffer, &pointer);
		HEADER[1] = serializer_read_char(buffer, &pointer);
		short version = serializer_read_short(buffer, &pointer);
		char* delim = serializer_read_string(buffer, &pointer);
		
		if(item_db_verify_header(buffer, version, delim))
		{
			printf("header ok!\n");
			item_db_t* item_db = item_db_create();
			short id;
			int i = 0;
			do
			{	
				if(i < MAX_ITEMS)
				{
					id = serializer_read_short(buffer, &pointer);
				
					char* name;
					char* description;
					name = serializer_read_string(buffer, &pointer);
					description = serializer_read_string(buffer, &pointer);
				
					item_db->items[i].id = id;
					item_db->items[i].name = malloc(strlen(name));
					item_db->items[i].description = malloc(strlen(description));
				
					strcpy(item_db->items[i].name, name);
					strcpy(item_db->items[i].description, description);
				
					i++;
				}
				else
				{
					break;
				}
			} while (id != -1);
			
			return item_db;
		}
	}
	
	return NULL;
}

item_db_t* item_db_create_test_db()
{
	item_db_t* test_db = malloc(sizeof(item_db_t));
	test_db->items = malloc(sizeof(item_t) * MAX_ITEMS);
	
	int i;
	for(i = 0; i < MAX_ITEMS; i++)
	{
		if(i >= 24)
		{
			test_db->items[i].id = -1;
			break;
		}
		else
		{
			test_db->items[i].id = i;
			char buffer[24];
			char desc[] = "It's just another test item, what'd you expect?";
			sprintf(buffer, "Test Item %d", i);
			test_db->items[i].name = malloc(strlen(buffer) + 1);
			test_db->items[i].description = malloc(strlen(desc) + 1);
		
			strcpy(test_db->items[i].name, buffer);
			strcpy(test_db->items[i].description, desc);
		}
	}
	printf("done making test db\n");
	return test_db;
}