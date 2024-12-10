#ifndef HASHTABLE_H
#define HASHTABLE_H
typedef struct hashTable {
	List *table[];
	void *(*get)(struct hashTable *hashTable, int key);
	void (*set)(struct hashTable *hashTable, int key, void *value);
	void (*remove)(struct hashTable *hashTable, int key);
	void (*free_value)(void *value);
} HashTable;

HashTable *alloc_hash_table((void)(*free_value)(void *value));

void free_hash_table(HashTable *hash_table);
#endif
