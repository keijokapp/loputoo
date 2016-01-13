
#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

struct ht_entry {
	char *key, *value;
	struct ht_entry* next;
};

struct ht_hashtable {
	int _size;
	struct ht_entry** _table;	
};

struct ht_hashtable* ht_hashtable_new(unsigned int size) {
	if(size < 1) size = 255;

	struct ht_hashtable* hashtable = malloc(sizeof(struct ht_hashtable));
	if(!hashtable) return NULL;
	hashtable->_table = malloc(sizeof(struct ht_entry*) * size);
	if(!hashtable->_table) return NULL;

	hashtable->_size = size;
	memset(hashtable->_table, 0, sizeof(struct ht_entry*) * size);

	return hashtable;	
}

static int ht_hash(const char* key) {
	unsigned long int hashval = 7;
	int hash = 7;
	while(*key) {
		hashval *= 31;
		hashval += *key;
		key++;
	}
	return hashval;
}

/* Create a key-value pair. */
struct ht_entry* ht_entry_new(const char* key, const char* value) {
	struct ht_entry* entry = malloc(sizeof(struct ht_entry));
	if(!entry) return NULL;
	entry->key = strdup(key);
	entry->value = strdup(value);
	entry->next = NULL;
	return entry;
}

void ht_hashtable_set(struct ht_hashtable* hashtable, const char* key, const char* value) {
	int bin = ht_hash(key) % hashtable->_size;

	struct ht_entry* last = NULL;
	struct ht_entry* next = hashtable->_table[bin];
	while(next && strcmp(key, next->key) > 0 ) {
		last = next;
		next = next->next;
	}


	/* There's already a pair.  Let's replace that string. */
	if(next && strcmp(key, next->key) == 0 ) {
		free(next->value);
		next->value = strdup(value);
	} else {
		struct ht_entry* entry = ht_entry_new(key, value);
		if(next == hashtable->_table[bin]) {
			/* We're at the start of the linked list in this bin. */
			entry->next = next;
			hashtable->_table[bin] = entry;
		} else if(next == NULL) {
			/* We're at the end of the linked list in this bin. */
			last->next = entry;
		} else  {
			/* We're in the middle of the list. */
			entry->next = next;
			last->next = entry;
		}
	}
}

/* Retrieve a entry from a hash table. */
const char* ht_hashtable_get(const struct ht_hashtable* hashtable, const char* key) {
	int bin = ht_hash(key) % hashtable->_size;

	/* Step through the bin, looking for our value. */
	struct ht_entry* entry = hashtable->_table[bin];
	while(entry && strcmp(key, entry->key) > 0) {
		entry = entry->next;
	}

	/* Did we actually find anything? */
	if(entry && strcmp(key, entry->key) == 0) {
		return entry->value;
	}
	return NULL;
}

/*
int main(int argc, const char* argv[]) {
	struct ht_hashtable* hashtable = ht_hashtable_new(20);

	ht_hashtable_set(hashtable, "key1", "inky");
	ht_hashtable_set(hashtable, "key2", "pinky");
	ht_hashtable_set(hashtable, "key3", "blinky");
	ht_hashtable_set(hashtable, "key4", "floyd");

	printf( "%s\n", ht_hashtable_get(hashtable, "key1"));
	printf( "%s\n", ht_hashtable_get(hashtable, "key2"));
	printf( "%s\n", ht_hashtable_get(hashtable, "key3"));
	printf( "%s\n", ht_hashtable_get(hashtable, "key4"));

	return 0;
}*/
