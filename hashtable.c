#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "hashtable.h"

// Forward declarations
static void add_entry(struct entry **bucketPtr, struct entry *entry);
static int bucket_index(const char *key, int num_buckets);

struct hashtable *hash_create(unsigned int buckets)
{
    if (buckets < 1)
        return NULL;

    struct hashtable *table;
    if ((table = (struct hashtable *)malloc( sizeof(struct hashtable) )) == NULL)
        return NULL;

    table->num_entries = 0;
    table->num_buckets = buckets;
    table->buckets = (struct entry **)malloc( sizeof(struct entry *) * buckets );

    if (!(table->buckets))
        return NULL;

    // Set all buckets to NULL (empty linked lists)
    for (int i = 0; i < buckets; i++)
        (table->buckets)[i] = NULL;

    return table;
}

int hash_hash(const char *key)
{
    // Uses Java's (OpenJDK) implementation of hashCode() for
    // java.lang.String; the formula for the hash is:
    //     s[0]*31^(n-1) + s[1]*31^(n-2) + ... + s[n-1],
    // where s[i] is the i-th character in `key`, s is the key, and
    // n is the length of the string `key`.
    // 
    // This should provide ample distribution of keys.

    int hash = 0;
    while (*key)
        hash = hash * 31 + *(key++);
    return hash;
}

bool hash_insert(struct hashtable *table, const char *key, const void *value)
{
    // Find relevant bucket and keep index (used to find reference to bucket in buckets array)
    int index            = bucket_index(key, table->num_buckets);
    struct entry *bucket = table->buckets[index];

    // Check the table for an existing entry (bucket is first entry, as it
    // represents the head of the linked list)
    for (struct entry *entry = bucket; entry != NULL; entry = entry->next)
    {
        if (!strcmp(entry->key, key))
        {
            entry->value = value;
            return true;
        }
    }

    // Ensure that the table will not exceed the maximum load factor
    // with the new number of entries; if it will, resize it to double the
    // number of buckets, and set the new index/bucket.
    if ( (float)(table->num_entries + 1) / table->num_buckets > MAX_LOAD_FACTOR )
    {
        hash_resize(table, table->num_buckets * 2);

        index = bucket_index(key, table->num_buckets);
        bucket = table->buckets[index];
    }

    // Create the new entry to add to the table
    struct entry *new_entry;

    if ((new_entry = (struct entry *)malloc( sizeof(struct entry) )) == NULL)
        return false;

    new_entry->key = key;
    new_entry->value = value;
    new_entry->next = NULL;

    add_entry(&(table->buckets[index]), new_entry);

    // Increment the number of table entries
    ++table->num_entries;
    return true;
}

void hash_resize(struct hashtable *table, int buckets)
{
    struct entry **new_buckets;
    if ((new_buckets = (struct entry **)malloc( sizeof(struct entry *) * buckets )) == NULL)
        return;

    // Set all buckets to NULL (empty linked lists)
    for (int i = 0; i < buckets; i++)
        (new_buckets)[i] = NULL;

    // Iterate over all existing buckets
    for (int i = 0; i < table->num_buckets; i++)
    {   
        // Iterate over each entry in the existing bucket,
        // rehashing it and adding it to a bucket (or creating
        // a bucket) in the new list of buckets.
        struct entry *entry = table->buckets[i];
        while (entry != NULL)
        {
            struct entry *next = entry->next;
            int new_index = bucket_index(entry->key, buckets);

            add_entry(&(new_buckets[new_index]), entry);

            // Get next entry
            entry = next;
        }
    }

    // Free the buckets!
    free(table->buckets);

    table->buckets = new_buckets;
    table->num_buckets = buckets;
}

const void *hash_get(struct hashtable *table, const char *key)
{
    int index = bucket_index(key, table->num_buckets);
    struct entry *bucket = table->buckets[index];

    // If bucket exists, traverse the list
    if (bucket != NULL)
    {
        for (struct entry *entry = bucket; entry != NULL; entry = entry->next)
        {
            // Check to see if entry key matches
            if (!strcmp(entry->key, key))
                return entry->value;
        }
    }

    return NULL;
}

const void *hash_delete(struct hashtable *table, const char *key)
{ 
    int index = bucket_index(key, table->num_buckets);
    struct entry *bucket = table->buckets[index];

    // Stop if bucket doesn't exist
    if (!bucket)
        return NULL;

    struct entry *prev = NULL;
    for (struct entry *entry = bucket; entry != NULL; prev = entry, entry = entry->next)
    {
        // Find element with same key in bucket
        if (!strcmp(entry->key, key))
        {
            const void *value = entry->value;

            // Remove from linked list chain, if it was not the head.
            // Otherwise, make the next element the head (which may be NULL).
            if (prev != NULL)
                prev->next = entry->next;
            else
                table->buckets[index] = entry->next; // Will set to NULL if it is the only element

            // Free the entry (it was malloc'd upon insertion)
            free(entry);

            // Decrement number of entries
            table->num_entries--;

            return value;
        }
    }

    return NULL;

}

void hash_destroy(struct hashtable *table)
{
    int i;
    struct entry *entry;

    // Iterate over each bucket
    for (i = 0; i < table->num_buckets; i++)
    {
        entry = table->buckets[i];

        // Free every entry in the bucket
        while (entry != NULL)
        {
            struct entry *next = entry->next;
            free(entry);

            entry = next;
        }
    }
    
    // Free the table (malloc'd in hash_create)
    free(table);
}

void hash_clear(struct hashtable *table)
{
    int i;
    struct entry *entry;

    // Iterate over each bucket
    for (i = 0; i < table->num_buckets; i++)
    {
        entry = table->buckets[i];

        // Free every entry
        while (entry != NULL)
        {
            struct entry *next = entry->next;
            free(entry);

            entry = next;
        }

        // Set each bucket to empty
        table->buckets[i] = NULL;
    }

    table->num_entries = 0;
}

/**
 * Helper function to add an entry to a bucket in a table (i.e. append to
 * linked list or create a new linked list).
 *
 * If the bucket is a null-pointer, we will need to change it such that it
 * points to the new entry (which will be the first in the linked list).
 * Hence, a pointer to the bucket in the buckets array (e.g. &(table->buckets[index]))
 * is needed.
 *
 * If the bucketPtr points to a value, the entry is instead linked to the last
 * element in the list.
 *
 * @param bucketPtr Pointer to the pointer to the bucket (linked list, so head) in the table (e.g. &(table->buckets[index]))
 * @param entry Entry to add (`next` is set to NULL)
 */
static void add_entry(struct entry **bucketPtr, struct entry *entry)
{
    if (*bucketPtr != NULL)
    {
        // Find the last element in the existing bucket and link it
        // to `entry`
        struct entry *last = *bucketPtr; // Pointer to entry struct of [first] element in list
        while (last->next != NULL)
            last = last->next;

        last->next = entry;
    }
    else
    {
        *bucketPtr = entry;
    }

    // Terminate the linked list (this should be the last element in it)
    entry->next = NULL;
}

/**
 * Gets the index of the bucket a key should fall under
 * given the number of buckets and key
 *
 * @param key Key to check
 * @param num_buckets Number of buckets to get index relative to
 * @return Index of bucket
 */
static int bucket_index(const char *key, int num_buckets)
{
    return hash_hash(key) % num_buckets;
}