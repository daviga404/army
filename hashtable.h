#ifndef HASHTABLE_H
#define HASHTABLE_H

/*
    Definitions
*/
#define MAX_LOAD_FACTOR 0.75

/*
    Structs
*/
struct hashtable {
    int num_entries;
    int num_buckets;
    struct entry **buckets;
};

struct entry {
    const char *key;
    const void *value;
    struct entry *next;
};

/*
    Function declarations
*/

/**
 * Creates a new hash table
 *
 * @param buckets The number of buckets (slots) to initialize the hash table with
 * @return hash_table A pointer to the newly created hash table
 */
struct hashtable *hash_create(unsigned int buckets);

/**
 * Gets an object from a hash table
 *
 * @param table The table
 * @param key The key of the object
 * @return The object, or null if not found
 */
const void *hash_get(struct hashtable *table, const char *key);

/**
 * Inserts an object into the hash table
 *
 * @param table The table to put an object into
 * @param key The key to assign to the value
 * @param value The value
 * @return A non-zero value if the object was inserted or replaced. Zero otherwise
 */
int hash_insert(struct hashtable *table, const char *key, const void *value);

/**
 * Hashes a string value. This is used in determining the
 * bucket in which the value will be placed (the returned value
 * is modulo the number of buckets).
 *
 * @param key Key to hash
 * @return Hash value
 */
int hash_hash(const char *key);

/**
 * Resizes the hash table to the new number of buckets. This
 * function involves rehashing each entry and reallocating
 * memory for the new buckets. Thus, it is an expensive operation.
 *
 * @param table Table to resize
 * @param buckets New number of buckets to resize table to
 */
void hash_resize(struct hashtable *table, int buckets);

/**
 * Removes an element from the hashtable
 *
 * @param table Table to remove element from
 * @param key Key of element to remove
 * @return The removed element value
 */
const void *hash_delete(struct hashtable *table, const char *key);

/**
 * Destroys a hash table instance (frees memory). Note that the
 * table given MUST have been created by hash_create, and also that
 * the table will NOT be usable after destruction. Any attempts to
 * use it after destruction will lead to your program's and possibly
 * your own destruction for being a bad programmer.
 *
 * @param table Table to destroy
 */
void hash_destroy(struct hashtable *table);

/**
 * Removes all elements from a hashtable
 *
 * @param table Hash table to clear
 */
void hash_clear(struct hashtable *table);

#endif /* HASHTABLE_H */