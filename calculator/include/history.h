#ifndef HISTORY_H
#define HISTORY_H

#include <stddef.h>

#define MAX_STRING_LENGTH 500   // Max length of each entry string
#define INITIAL_CAPACITY 4      // Initial capacity for the dynamic array

// Structure to represent history
typedef struct {
    char **entries;   // Dynamic array of strings (history entries)
    size_t size;      // Current count of history entries
    size_t capacity;  // Capacity of the array
} History;

// Initialize the history structure
void history_init(History *history);

// Resize the history array if needed
void history_resize(History *history);

// Add a new entry to the history
void history_add(History *history, const char *entry);

// Get an entry from the history
const char *history_get(History *history, size_t index);

// Free all memory used by the history
void history_free(History *history);

// Clear the history (set size to 0, but don't free memory)
void history_clear(History *history);

// Add an entry with the result of a calculation
void add_to_history(History *history, double result);

// Add a formatted entry with two numbers, an operator, and the result
void add_history_entry(History *history, double num1, double num2, char op, double result);

#endif // HISTORY_H
