//
// Created by tomjh on 14/10/2025.
//

#ifndef HASH_MAP_HASH_TABLE_H
#define HASH_MAP_HASH_TABLE_H
#define HT_INITIAL_BASE_SIZE 53



// hash table contents
typedef struct {
    char* key;
    char* value;
}ht_item;
// our hash table
typedef struct {
    int size;
    int count;
    ht_item** items;
}ht_hash_table;

#endif //HASH_MAP_HASH_TABLE_H