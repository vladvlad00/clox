#include <stdlib.h>
#include "memory.h"
#include "common.h"
#include "vm.h"

void *reallocate(void *previous, size_t oldSize, size_t newSize)
{
    if (newSize == 0)
    {
        free(previous);
        return NULL;
    }
    return realloc(previous, newSize);
}

static void freeObject(Obj *object)
{
    switch (object->type)
    {
    case OBJ_STRING:
    {
        ObjString *string = (ObjString *)object;
        FREE_ARRAY(string->chars, char, string->length + 1);
        FREE(object, ObjString);
        break;
    }

    default:
        break;
    }
}

void freeObjects()
{
    Obj *object = vm.objects;
    while (object != NULL)
    {
        Obj *next = object->next;
        freObject(object);
        object = next;
    }
}