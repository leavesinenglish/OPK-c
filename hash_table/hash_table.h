#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H
typedef void *Pointer;

typedef unsigned (*HashFunction)(char *key);

typedef void (*Destructor)(Pointer);

typedef struct elem {
    char *key;
    Pointer data;
} elem;

typedef struct HashTable {
    size_t size;
    HashFunction hashfunc;
    Destructor dtor;
    elem *table;
} HashTable;
unsigned jenkins_one_at_a_time_hash(char *key);
void ht_init(HashTable *ht, size_t size, HashFunction hf, Destructor dtor);
void ht_destroy(HashTable *ht);
int _ht_find_place(HashTable *ht, char *key);
Pointer ht_set(HashTable *ht, char *key, Pointer data);
Pointer ht_get(HashTable *ht, char *key);
int ht_has(HashTable *ht, char *key);
void ht_delete(HashTable *ht, char *key);
void ht_traverse(HashTable *ht, void (*f)(char *key, Pointer data));
void ht_resize(HashTable *ht, size_t new_size);
#endif //HASH_TABLE_HASH_TABLE_H