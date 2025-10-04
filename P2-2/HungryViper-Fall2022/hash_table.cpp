// ============================================
// The HashTable class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


/*
Student Name: Arina Shah
Date: 11/1/2022
=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
    e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
    e.g. "_HashTable", "_HashTableEntry"

    Recall that in C, we have to type "struct" together with the name of the struct
    in order to initialize a new variable. To avoid this, in hash_table.h
    we use typedef to provide new "nicknames" for "struct _HashTable" and
    "struct _HashTableEntry". As a result, we can create new struct variables
    by just using:
        - "HashTable myNewTable;"
    or
        - "HashTableEntry myNewHashTableEntry;"
    
    The preceding underscore "_" simply provides a distinction between the names
    of the actual struct defition and the "nicknames" that we use to initialize
    new structs.
    [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
    the first letter is lower-case.
    e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
        It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
    as a distinction between function local variables and struct members.
    e.g. "num_buckets" is a member of "HashTable".
*/


/****************************************************************************
 * Include the Public Interface
 *
 * By including the public interface at the top of the file, the compiler can
 * enforce that the function declarations in the the header are not in
 * conflict with the definitions in the file. This is not a guarantee of
 * correctness, but it is better than nothing!
 ***************************************************************************/
#include "hash_table.h"

/****************************************************************************
 * Include other private dependencies
 *
 * These other modules are used in the implementation of the hash table module,
 * but are not required by users of the hash table.
 ***************************************************************************/
#include <stdlib.h> // For malloc and free
#include <stdio.h>  // For printf

/****************************************************************************
 * Hidden Definitions
 *
 * These definitions are not available outside of this file. However, because
 * the are forward declared in hash_table.h, the type names are
 * available everywhere and user code can hold pointers to these structs.
 ***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable
{
    /** The array of pointers to the head of a singly linked list, whose nodes
        are HashTableEntry objects */
    HashTableEntry **buckets;

    /** The hash function pointer */
    HashFunction hash;

    /** The number of buckets in the hash table */
    unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry
{
    /** The key for the hash table entry */
    unsigned int key;

    /** The value associated with this hash table entry */
    void *value;

    /**
     * A pointer pointing to the next hash table entry
     * NULL means there is no next entry (i.e. this is the tail)
     */
    HashTableEntry *next;
};

/****************************************************************************
 * Private Functions
 *
 * These functions are not available outside of this file, since they are not
 * declared in hash_table.h.
 ***************************************************************************/
/**
 * createHashTableEntry
 *
 * Helper function that creates a hash table entry by allocating memory for it on
 * the heap. It initializes the entry with key and value, initialize pointer to
 * the next entry as NULL, and return the pointer to this hash table entry.
 *
 * @param key The key corresponds to the hash table entry
 * @param value The value stored in the hash table entry
 * @return The pointer to the hash table entry
 */
static HashTableEntry *createHashTableEntry(unsigned int key, void *value)
{
    // TODO: Implement
    HashTableEntry *newHashTableEntry = (HashTableEntry *)malloc(sizeof(HashTableEntry));
    newHashTableEntry->key = key;
    newHashTableEntry->value = value;
    newHashTableEntry->next = NULL;
    return newHashTableEntry;
}

/**
 * findItem
 *
 * Helper function that checks whether there exists the hash table entry that
 * contains a specific key.
 *
 * @param hashTable The pointer to the hash table.
 * @param key The key corresponds to the hash table entry
 * @return The pointer to the hash table entry, or NULL if key does not exist
 */
static HashTableEntry *findItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    unsigned int index = hashTable->hash(key);
    HashTableEntry* curr = hashTable->buckets[index];
    while (curr != NULL) {
        if (curr->key == key) {
            return curr;
        }
        curr = curr->next;
    }
    
    return NULL;
}

/****************************************************************************
 * Public Interface Functions
 *
 * These functions implement the public interface as specified in the header
 * file, and make use of the private functions and hidden definitions in the
 * above sections.
 ****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable *createHashTable(HashFunction hashFunction, unsigned int numBuckets)
{
    // The hash table has to contain at least one bucket. Exit gracefully if
    // this condition is not met.
    if (numBuckets == 0)
    {
        printf("Hash table has to contain at least 1 bucket...\n");
        exit(1);
    }

    // Allocate memory for the new HashTable struct on heap.
    HashTable *newTable = (HashTable *)malloc(sizeof(HashTable));

    // Initialize the components of the new HashTable struct.
    newTable->hash = hashFunction;
    newTable->num_buckets = numBuckets;
    newTable->buckets = (HashTableEntry **)malloc(numBuckets * sizeof(HashTableEntry *));

    // As the new buckets are empty, init each bucket as NULL.
    unsigned int i;
    for (i = 0; i < numBuckets; ++i)
    {
        newTable->buckets[i] = NULL;
    }

    // Return the new HashTable struct.
    return newTable;
}

void destroyHashTable(HashTable *hashTable)
{
    // TODO: Implement
    // 1. Loop through each bucket of the hash table to remove all items.
    //      1a. set temp to be the first entry of the ith bucket
    //      1b. delete all entries
       
    // 2. Free buckets
    
    // 3. Free hash table

    for (unsigned int i = 0; i < hashTable->num_buckets; i++) {
        if (hashTable->buckets[i]) {
            HashTableEntry* temp = hashTable->buckets[i];
            HashTableEntry* item = temp->next;
            while (item) {
                free(temp->value);
                free(temp);
                temp = item;
                item = item->next;
            }
            free(temp->value);
            free(temp);
            free(item);
        } 
    }
    free(hashTable->buckets);
    free(hashTable);
    return;

}

void *insertItem(HashTable *hashTable, unsigned int key, void *value)
{
    // TODO: Implement
    //1. First, we want to check if the key is present in the hash table.
        
    //2. If the key is present in the hash table, store new value and return old value

    //3. If not, create entry for new value and return NULL

    HashTableEntry* item = findItem(hashTable, key);
    if (item == NULL) {
        unsigned int index = hashTable->hash(key);
        HashTableEntry* newEntry = createHashTableEntry(key, value);
        newEntry->next = hashTable->buckets[index];
        hashTable->buckets[index] = newEntry;
        return NULL;
    } else {
        void *oldVal = item->value;
        item->value = value;
        return oldVal;
    }

}

void *getItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //1. First, we want to check if the key is present in the hash table.

    //2. If the key exist, return the value
    
    //3. If not. just return NULL
    HashTableEntry* item = findItem(hashTable, key);
    if (item == NULL) {
        return NULL;
    } else {
        return item->value;
    }
}

void *removeItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //1. Get the bucket number and the head entry
    
    //2. If the head holds the key, change the head to the next value, and return the old value
    
    //3. If not the head, search for the key to be removed 
    
    //4. If the key is not present in the list, return NULL
    
    //5. Unlink node from the list and return old value
    unsigned int index = hashTable->hash(key);
    HashTableEntry* curr = hashTable->buckets[index];
    if(curr == NULL) {
        return NULL;
    }
    if (curr->key == key) {
        void *oldVal = curr->value;
        hashTable->buckets[index] = curr->next;
        free(curr);
        return oldVal;
    }
    while (curr && curr->next) {
        if (curr->next->key == key) {
            HashTableEntry *del = curr->next;
            void *oldVal = del->value;
            curr->next = curr->next->next;
            free(del);
            return oldVal;
        }
        curr = curr->next;
    }
    return NULL;
}

void deleteItem(HashTable *hashTable, unsigned int key)
{
    // TODO: Implement
    //1. Remove the entry and free the returned data
    if (findItem(hashTable, key) == NULL) {
        return;
    }
    unsigned int index = hashTable->hash(key);
    HashTableEntry* curr = hashTable->buckets[index];
    if (curr == NULL) {
        return;
    }
    if (curr->key == key) {
        hashTable->buckets[index] = curr->next;
        free(curr->value);
        free(curr);
        return;
    }
    while (curr && curr->next) {
        if (curr->next->key == key) {
            HashTableEntry* del = curr->next;
            curr->next = curr->next->next;
            free(del->value);
            free(del);
            return;
        }
        curr = curr->next;
    }
}

void *getHead(HashTable *hashTable, unsigned int index) {
    return hashTable->buckets[index]->value;
}

unsigned int returnNumBuckets(HashTable *hashTable) {
    return hashTable->num_buckets;
}

void *getLinkedListElement(HashTable *hashTable, unsigned int bucket, unsigned int index) {
    HashTableEntry* curr = hashTable->buckets[bucket];
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->value;
}

HashTableEntry *getNext(HashTable *hashTable, unsigned int bucket, unsigned int index) {
    HashTableEntry* curr = hashTable->buckets[bucket];
    for (int i = 0; i < index; i++) {
        curr = curr->next;
    }
    return curr->next;
}
