struct ht_hashtable {
	int _size;
	struct ht_entry** _table;	
};

struct ht_hashtable* ht_hashtable_new(unsigned int size);
struct ht_entry* ht_entry_new(const char* key, const char* value);
void ht_hashtable_set(struct ht_hashtable* hashtable, const char* key, const char* value);
const char* ht_hashtable_get(const struct ht_hashtable* hashtable, const char* key);
