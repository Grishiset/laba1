#include "string_collection.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct FieldInfo* CHAR_FIELD_INFO = NULL;
struct FieldInfo* STRING_FIELD_INFO = NULL;


struct FieldInfo* getCharFieldInfo() {
    if (!CHAR_FIELD_INFO) {
        CHAR_FIELD_INFO = malloc(sizeof(struct FieldInfo));
        CHAR_FIELD_INFO->element_size = sizeof(char_t);
        CHAR_FIELD_INFO->toString = char_to_string;
        CHAR_FIELD_INFO->concat = char_concat;
        CHAR_FIELD_INFO->substring = char_substring;
        CHAR_FIELD_INFO->split = char_split;
        CHAR_FIELD_INFO->free_element = char_free;
    }
    return CHAR_FIELD_INFO;
}

char_t* char_create(char value) {
    char_t* obj = malloc(sizeof(char_t));
    obj->value = value;
    return obj;
}

void* char_concat(void* dest, void* src) {
    char_t* d = (char_t*)dest;
    char_t* s = (char_t*)src;
    string_t* result = string_create("");
    result->data = malloc(3);
    sprintf(result->data, "%c%c", d->value, s->value);
    result->length = 2;
    return result;
}

void* char_substring(void* data, size_t i, size_t j) {
    (void)i; (void)j; 
    char_t* c = (char_t*)data;
    return char_create(c->value);
}

void** char_split(void* data, size_t* count) {
    (void)data;
    *count = 0;
    return NULL; 
}

void char_free(void* element) {
    free((char_t*)element);
}

void char_to_string(void* element) {
    char_t* c = (char_t*)element;
    printf("%c", c->value);
}

//-------------------------------------------STRING----------------------------------------------------------------

struct FieldInfo* getStringFieldInfo() {
    if (!STRING_FIELD_INFO) {
        STRING_FIELD_INFO = malloc(sizeof(struct FieldInfo));
        STRING_FIELD_INFO->element_size = sizeof(string_t);
        STRING_FIELD_INFO->toString = string_to_string;
        STRING_FIELD_INFO->concat = string_concat;
        STRING_FIELD_INFO->substring = string_substring;
        STRING_FIELD_INFO->split = string_split;
        STRING_FIELD_INFO->free_element = string_free;
    }
    return STRING_FIELD_INFO;
}

string_t* string_create(const char* value) {
    string_t* str = malloc(sizeof(string_t));
    str->length = strlen(value);
    str->data = malloc(str->length + 1);
    strcpy(str->data, value);
    return str;
}

void* string_concat(void* dest, void* src) {
    string_t* d = (string_t*)dest;
    string_t* s = (string_t*)src;
    string_t* result = string_create("");
    result->data = malloc(d->length + s->length + 1);
    strcpy(result->data, d->data);
    strcat(result->data, s->data);
    result->length = d->length + s->length;
    return result;
}

void* string_substring(void* data, size_t i, size_t j) {
    string_t* str = (string_t*)data;
    if (i >= str->length || j >= str->length || i > j) return NULL;
    
    string_t* substr = string_create("");
    substr->length = j - i + 1;
    substr->data = malloc(substr->length + 1);
    strncpy(substr->data, str->data + i, substr->length);
    substr->data[substr->length] = '\0';
    return substr;
}

void** string_split(void* data, size_t* count) {
    string_t* str = (string_t*)data;
    char* copy = strdup(str->data);  
    if (!copy) {
        *count = 0;
        return NULL;  
    }

    
    *count = 0;
    char* token = strtok(copy, " ");
    while (token) {
        (*count)++;
        token = strtok(NULL, " ");
    }

    
    free(copy);
    copy = strdup(str->data);  
    if (!copy) {
        *count = 0;
        return NULL;  
    }

    
    string_t** words = malloc(*count * sizeof(string_t*));
    if (!words) {
        free(copy);
        *count = 0;
        return NULL;  
    }


    token = strtok(copy, " ");
    for (size_t i = 0; token; i++) {
        words[i] = string_create(token);
        token = strtok(NULL, " ");
    }

    free(copy);  
    return (void**)words;
}

void string_free(void* element) {
    string_t* str = (string_t*)element;
    free(str->data);
    free(str);
}

void string_to_string(void* element) {
    string_t* str = (string_t*)element;
    printf("%s", str->data);
}


void* Zero(struct FieldInfo* type) {
    if (type == getCharFieldInfo()) {
        return char_create('\0');
    } else if (type == getStringFieldInfo()) {
        return string_create("");
    }
    return NULL;
}

void* sum(struct FieldInfo* type, void* a, void* b) {
    return type->concat(a, b);
}

void* sub(struct FieldInfo* type, void* a, void* b) {
    (void)type; (void)a; (void)b;
    return NULL; 
}

void freeFieldInfo(struct FieldInfo* fieldInfo) {
    if (fieldInfo) free(fieldInfo);
}
