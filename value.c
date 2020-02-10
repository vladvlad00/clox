#include <stdio.h>
#include "value.h"
#include "memory.h"

void initValueArray(ValueArray* array)
{
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}

void writeValueArray(ValueArray* array, Value value)
{
    if (array->count + 1 > array->capacity)
    {
        int oldCapacity = array->capacity;
        array->capacity = GROW_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(array->values, Value, oldCapacity, array->capacity);
    }
    array->values[array->count] = value;
    array->count++;
}

void freeValueArray(ValueArray* array)
{
    FREE_ARRAY(array, Value, array->capacity);
    initValueArray(array);
}

void printValue(Value value)
{
    printf("%g", value);
}