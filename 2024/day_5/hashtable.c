#include "hashtable.h"
#include "linkedlist.h"

#define TABLE_SIZE 100

typedef struct payload {
	int key;
	void *value;
} Payload;

HashTable *alloc_hash_table((void)(*free_value)(void *value)){
	HashTable *hash_table = malloc(sizeof(HashTable));
	List *table = malloc(sizeof(List) * TABLE_SIZE);
	hash_table->table = table;
	hash_table->get = get;
	hash_table->set = set;
	hash_table->remove = remove;
	hash_table->free_value = free_value;
	return hash_table;
}

void *get(HashTable *hash_table, int key){

}

void set(HashTable *hashTable, int key, void *value){

}

void remove(HashTable *hashTable, int key){

}

// this is not the value, but the payload...
void free_value(void *value){

}


// free the user payload
// then free the hash tables payload
// then free the list element?
//
void free_hash_table_payload(void *)

void free_hash_table(HashTable *hash_table){
	List *table = hash_table->table;

	for (int i = 0; i < TABLE_SIZE; i++) {
		// free the payload within, then free the 
		// this method requires that we provide a function that will free the payload
		// that function would be defined here...
		//
		free_list(table[i]);
	}

	free(table);
	hash_table->table = NULL;
	free(hash_table);
}

