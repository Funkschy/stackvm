#ifndef CRISPY_HASHMAP_H
#define CRISPY_HASHMAP_H

#include "value.h"

typedef enum {
    HT_KEY_OBJSTRING, HT_KEY_CSTRING, HT_KEY_INT, HT_KEY_IDENT_STRING
} HTKeyType;

typedef union {
    struct {
        const char *key_ident_string;
        size_t ident_length;
    };
    const char *key_c_string;
    ObjString *key_obj_string;
    uint32_t key_int;
} HTItemKey;

typedef struct table_item_t {
    HTItemKey key;
    Value value;
    struct table_item_t *next;
} HTItem;

typedef struct {
    HTKeyType key_type;

    uint32_t cap;
    uint32_t size;

    void (*free_callback)(HTItem *);

    HTItem **buckets;
} HashTable;

uint32_t hash(HTItemKey key, HTKeyType type);

void ht_init(HashTable *ht, HTKeyType key_type, uint32_t init_cap, void(*free_callback)(HTItem *));

void ht_free(HashTable *ht);

void free_string_literal(HTItem *item);

void free_heap_string(HTItem *item);

Value ht_get(HashTable *ht, HTItemKey key);

void ht_delete(HashTable *ht, HTItemKey key);

void ht_put(HashTable *ht, HTItemKey key, Value value);

#endif //CRISPY_HASHMAP_H
