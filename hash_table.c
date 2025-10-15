// Key value pairs
// Hash table is an array of buckets
// Key is passed through hashing function and returns an integer
// the integer is the pairs index in the array of buckets
// to retrieve the key-value pair
// supply the key to the same hashing function
// receive the index
// Then use the index to find the array


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "hash_table.h"
#define HT_PRIME_1 151
#define HT_PRIME_2 163


void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* h, const char* key);

static ht_item HT_DELETED_ITEM = {NULL, NULL};

// k -> key, v -> value
static ht_item* ht_new_item(const char* k, const char* v) {
    // let i be a pointer
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(k);
    i->value = strdup(v);
    return i;
};

// is x a prime number

int isPrime(int x) {
    if (x < 2) return -1;
    else if (x < 4) return 1;
    else if ((x%2)==0) return 0;
    for (int i = 3; i <= floor(sqrt((double) x)); i += 2) {
        if (x % i == 0) return 0;
    }
    return 1;
}


int nextPrime(int x) {
    while (isPrime(x) != 1) {
        x++;
    }
    return x;
}

static ht_hash_table* ht_new_sized(const int base_size) {
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = base_size;

    ht->size = nextPrime(ht->size);

    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

ht_hash_table* ht_new() {
    // // create new pointer ht of size ht_hash_table struct
    // ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    // // initialise how many items we can store
    // ht->size = 53;
    // ht->count = 0;
    // // size_t is unsigned and guarantees the pointer is big enough to handle size
    // // essentially a 'safe' type
    // ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    // return ht;
    return ht_new_sized(HT_INITIAL_BASE_SIZE);
};



// these two methods below will be handled the freeing of data
static void ht_del_item(ht_item* i);
void ht_del_hash_table(ht_hash_table* ht);
static void ht_resize(ht_hash_table* ht, const int base_size);
static void ht_resize_up(ht_hash_table* ht);
static void ht_resize_down(ht_hash_table* ht);
static int ht_hashing(const char* s, const int a, const int numBuckets);
static int ht_get_hash(const char* s, const int numBuckets , const int attempt);


int main(void) {
    //initialise then delete table
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "name1", "Tom");
    ht_insert(ht, "name2", "Bob");
    ht_insert(ht, "name3", "Muhammad");
    ht_delete(ht, "name2");
    printf("name1 is: %s\n", ht_search(ht, "name1"));
    if (ht_search(ht, "name2") == NULL) {
        printf("name2 not found\n");
    }
    printf("name 3 is: %s\n", ht_search(ht, "name3"));
    ht_del_hash_table(ht);
}

static void ht_del_item(ht_item* i) {
    // remove key, value and pointer from memory
    free(i->key);
    free(i->value);
    free(i);
};

void ht_del_hash_table(ht_hash_table* ht) {
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        // recursively delete nested items
        if (item != NULL) {
            ht_del_item(item);
        }
    }
    // free the item from memory and the pointer too
    free(ht->items);
    free(ht);
}

static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->size = new_ht->size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}

static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->size * 2;
    ht_resize(ht, new_size);
}

static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->size / 2;
    ht_resize(ht, new_size);
}

// note this method requires user to pick an 'a'
// due to human error humans are more likely to pick one 'a' over another
// 37, 73 and 69 for example
static int ht_hashing(const char* s, const int a, const int numBuckets) {
    long hash = 0;
    const int stringLength = strlen(s);

    for (int i = 0; i < stringLength; i++) {
        hash = (hash * a + s[i]) % numBuckets;
    }

    return (int)hash;
}

// double hashing to handle collisions
// Primes defined at top of file
static int ht_get_hash(
    const char* s, const int numBuckets , const int attempt) {
    const int hash_a = ht_hashing(s, HT_PRIME_1, numBuckets);
    const int hash_b = ht_hashing(s, HT_PRIME_2, numBuckets);

    return (hash_a + (attempt * (hash_b + 1))) % numBuckets;
}

void ht_insert(ht_hash_table* ht, const char* key, const char* value) {
    const int load = ht->count * 100 / ht->size;
    if (load > 70) {
        ht_resize_up(ht);
    }
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* curr_item = ht->items[index];
    int i = 1;

    while (curr_item != NULL && curr_item != &HT_DELETED_ITEM) {
        // when inserting a new key delete old one to avoid collisions
        if (strcmp(curr_item->key, key) == 0) {
            // frees memory
            ht_del_item(curr_item);
            ht->items[index] = item;
            return;
        }
        index = ht_get_hash(curr_item->key, ht->size, i);
        curr_item = ht->items[index];
        i++;
    };
    ht->items[index] = item;
    ht->count++;
};

char* ht_search(ht_hash_table* ht, const char* key) {
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;

    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                return item->value;
            }
        }

        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    };
    return NULL;
};

void ht_delete(ht_hash_table* ht, const char* key) {
    const int load = ht->count * 100 / ht->size;
    if (load < 10) {
        ht_resize_down(ht);
    }
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while (item != NULL) {
        if (item != &HT_DELETED_ITEM) {
            if (strcmp(item->key, key) == 0) {
                ht_del_item(item);
                ht->items[index] = &HT_DELETED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
}

