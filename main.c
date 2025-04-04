#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Тип элемента: CHAR (символ) или STRING (строка)
typedef enum { CHAR, STRING } ElementType;

// Структура элемента коллекции
typedef struct {
    void* data;          // Данные (char или char*)
    ElementType type;    // Тип элемента
} StringElement;

// Виртуальная таблица операций
typedef struct {
    void (*concat)(StringElement* dest, StringElement* src);
    StringElement* (*substring)(StringElement* elem, size_t i, size_t j);
    void (*split)(StringElement* elem);
    char* (*toString)(StringElement* elem);
} StringVTable;

// Создание элемента
StringElement* create_char_element(char c);
StringElement* create_string_element(const char* str);

// Операции
void string_concat(StringElement* dest, StringElement* src);
StringElement* string_substring(StringElement* elem, size_t i, size_t j);
void string_split(StringElement* elem);
char* string_toString(StringElement* elem);

// Освобождение памяти
void free_element(StringElement* elem);
