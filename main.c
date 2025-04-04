#include "string_collection.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

void logMessage(char* message) {
    FILE* outFile = fopen(OUTPUT_FILE, "a");
    if (outFile) {
        fprintf(outFile, "%s\n", message);
        fclose(outFile);
    }
}

void printResult(void* result, struct FieldInfo* type) {
    if (result) {
        printf("Result: ");
        type->toString(result);
        printf("\n");
    }
}


typedef struct {
    void** elements;         
    size_t size;             
    size_t capacity;         
    struct FieldInfo* type;  
} Collection;

Collection* createCollection(struct FieldInfo* type) {
    Collection* col = malloc(sizeof(Collection));
    if (!col) return NULL;
    col->elements = malloc(10 * sizeof(void*));  
    if (!col->elements) {
        free(col);
        return NULL;
    }
    col->size = 0;
    col->capacity = 10;
    col->type = type;
    return col;
}


void addElement(Collection* col, void* value) {
    if (col->size == col->capacity) {
        col->capacity *= 2;
        void** new_elements = realloc(col->elements, col->capacity * sizeof(void*));
        if (!new_elements) {
            logMessage("Error: Memory reallocation failed.");
            return;
        }
        col->elements = new_elements;
    }
    col->elements[col->size++] = value;
}

void deleteCollection(Collection* col) {
    if (!col) return;
    for (size_t i = 0; i < col->size; i++) {
        col->type->free_element(col->elements[i]);
    }
    free(col->elements);
    free(col);
}

void* getElement(Collection* col, size_t index) {
    if (index >= col->size) return NULL;
    return col->elements[index];
}

int main() {
    FILE* inFile = fopen(INPUT_FILE, "r");
    if (!inFile) {
        logMessage("Error: Cannot open input file.");
        return 1;
    }

    FILE* outFile = fopen(OUTPUT_FILE, "w");
    if (!outFile) {
        fclose(inFile);
        logMessage("Error: Cannot open output file.");
        return 1;
    }
    fclose(outFile);

    char command[50];
    Collection* collection = NULL;
    struct FieldInfo* type = NULL;

    while (fscanf(inFile, "%s", command) != EOF) {
        if (strcmp(command, "CREATE_CHAR") == 0) {
            type = getCharFieldInfo();
            collection = createCollection(type);
            logMessage("Char collection created.");
        }
        else if (strcmp(command, "CREATE_STRING") == 0) {
            type = getStringFieldInfo();
            collection = createCollection(type);
            logMessage("String collection created.");
        }
        else if (strcmp(command, "ADD_CHAR") == 0) {
            if (!collection || type != getCharFieldInfo()) {
                logMessage("Error: Invalid collection or type.");
                continue;
            }
            char c;
            if (fscanf(inFile, " %c", &c) != 1) {
                logMessage("Error: Failed to read character.");
                continue;
            }
            char_t* value = char_create(c);
            addElement(collection, value);
            logMessage("Char added to collection.");
        }
        else if (strcmp(command, "ADD_STRING") == 0) {
            if (!collection || type != getStringFieldInfo()) {
                logMessage("Error: Invalid collection or type.");
                continue;
            }
            char str[100];
            if (fscanf(inFile, "%s", str) != 1) {
                logMessage("Error: Failed to read string.");
                continue;
            }
            string_t* value = string_create(str);
            addElement(collection, value);
            logMessage("String added to collection.");
        }
        else if (strcmp(command, "CONCAT") == 0) {
            if (!collection) {
                logMessage("Error: Collection not created.");
                continue;
            }
            int index1, index2;
            if (fscanf(inFile, "%d %d", &index1, &index2) != 2) {
                logMessage("Error: Failed to read indices for CONCAT.");
                continue;
            }
            if (index1 < 0 || index1 >= collection->size || index2 < 0 || index2 >= collection->size) {
                logMessage("Error: Invalid index.");
                continue;
            }
            void* elem1 = getElement(collection, index1);
            void* elem2 = getElement(collection, index2);
            void* result = collection->type->concat(elem1, elem2);
            logMessage("Elements concatenated.");
            printResult(result, collection->type);
            collection->type->free_element(result);
        }
        else if (strcmp(command, "SUBSTRING") == 0) {
            if (!collection) {
                logMessage("Error: Collection not created.");
                continue;
            }
            int index, start, end;
            if (fscanf(inFile, "%d %d %d", &index, &start, &end) != 3) {
                logMessage("Error: Failed to read parameters for SUBSTRING.");
                continue;
            }
            if (index < 0 || index >= collection->size) {
                logMessage("Error: Invalid index.");
                continue;
            }
            void* elem = getElement(collection, index);
            void* result = collection->type->substring(elem, start, end);
            if (result) {
                logMessage("Substring extracted.");
                printResult(result, collection->type);
                collection->type->free_element(result);
            } else {
                logMessage("Error: Substring extraction failed.");
            }
        }
        else if (strcmp(command, "SPLIT") == 0) {
            if (!collection) {
                logMessage("Error: Collection not created.");
                continue;
            }
            int index;
            if (fscanf(inFile, "%d", &index) != 1) {
                logMessage("Error: Failed to read index for SPLIT.");
                continue;
            }
            if (index < 0 || index >= collection->size) {
                logMessage("Error: Invalid index.");
                continue;
            }
            void* elem = getElement(collection, index);
            size_t count;
            void** words = collection->type->split(elem, &count);
            if (words) {
                logMessage("String split into words.");
                // Вывод каждого слова
                for (size_t i = 0; i < count; i++) {
                    printf("Word %zu: ", i);
                    collection->type->toString(words[i]);
                    printf("\n");
                    collection->type->free_element(words[i]);
                }
                free(words);
            } else {
                logMessage("Error: Split failed.");
            }
        }
        else if (strcmp(command, "DELETE") == 0) {
            if (collection) {
                deleteCollection(collection);
                collection = NULL;
                logMessage("Collection deleted.");
            }
        }
        else {
            logMessage("Error: Unknown command.");
        }
    }

    if (collection) {
        deleteCollection(collection);
    }
    fclose(inFile);
    return 0;
}
