#pragma once
#include <stdlib.h>
#include <stddef.h>


struct FieldInfo {
    size_t element_size;                
    void (*toString)(void* element);    
    void* (*concat)(void* dest, void* src);       
    void* (*substring)(void* data, size_t i, size_t j); 
    void** (*split)(void* data, size_t* count);  
    void (*free_element)(void* element); 
};


typedef struct {
    char value; 
} char_t;


typedef struct {
    char* data;    
    size_t length;
} string_t;


struct FieldInfo* getCharFieldInfo();
char_t* char_create(char value);
void* char_concat(void* dest, void* src);
void* char_substring(void* data, size_t i, size_t j);
void** char_split(void* data, size_t* count);
void char_free(void* element);
void char_to_string(void* element);


struct FieldInfo* getStringFieldInfo();
string_t* string_create(const char* value);
void* string_concat(void* dest, void* src);
void* string_substring(void* data, size_t i, size_t j);
void** string_split(void* data, size_t* count);
void string_free(void* element);
void string_to_string(void* element);


void* Zero(struct FieldInfo* type);
void* sum(struct FieldInfo* type, void* a, void* b);
void* sub(struct FieldInfo* type, void* a, void* b);
void freeFieldInfo(struct FieldInfo* fieldInfo);
