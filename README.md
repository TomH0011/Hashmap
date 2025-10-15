C Hashmap (Hash Table Implementation)

A simple, educational implementation of a hashmap (hash table) in pure C, built from scratch.
This project demonstrates how hashing, collision handling, dynamic memory allocation, and key–value storage work under the hood without relying on external libraries.

----------------Features----------------

Store, search, and delete key–value pairs

Open addressing with double hashing for collision resolution

Simple prime-based resizing (optional extension)

Fully implemented with manual memory management (malloc/free)

Clear, readable structure designed for learning

----------------How It Works----------------

Each key is passed through a hashing function that converts it into an integer index.

The index maps to an array position (the "bucket") where the key–value pair is stored.

If two keys hash to the same index, double hashing is used to find the next open slot.

Keys and values are stored as dynamically allocated strings (char*).

Memory is manually freed when items or the table are deleted.

----------------Project Structure----------------
.
├── hash_table.c      # Implementation
├── hash_table.h      # Structs & function declarations
├── prime.c / prime.h # Prime utilities for table sizing
└── main.c            # Example usage

----------------Example Usage----------------
```
#include "hash_table.h"
#include <stdio.h>

int main(void) {
    ht_hash_table* ht = ht_new();
    ht_insert(ht, "user", "Tom");
    ht_insert(ht, "language", "C");

    printf("user: %s\n", ht_search(ht, "user"));
    printf("language: %s\n", ht_search(ht, "language"));

    ht_delete(ht, "user");
    ht_del_hash_table(ht);
    return 0;
}
```

Output:

user: Tom
language: C

----------------Functions----------------
Function	Description
ht_new()	Creates a new hash table
ht_insert(ht, key, value)	Inserts or updates a key–value pair
ht_search(ht, key)	Retrieves a value by key
ht_delete(ht, key)	Removes a key–value pair
ht_del_hash_table(ht)	Frees all memory used by the table
----------------Build & Run----------------

Compile with gcc:

gcc -o hash_map main.c hash_table.c prime.c
./hash_map
