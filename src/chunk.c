#include <stdlib.h>
#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk)
{
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->lineCount = 0;
    chunk->lineCapacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initValueArray(&chunk->constants);
}

void writeChunk(Chunk* chunk, uint8_t byte, int line)
{
    if (chunk->count+1 > chunk->capacity)
    {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(chunk->code, uint8_t, oldCapacity, chunk->capacity);
    }
    chunk->code[chunk->count] = byte;
    chunk->count++;
    if (chunk->lineCount > 0 && chunk->lines[chunk->lineCount].line == line)
        chunk->lines[chunk->lineCount].count++;
    else
    {
        if (chunk->lineCount+1  > chunk->lineCapacity)
        {
            int oldCapacity = chunk->lineCapacity;
            chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
            chunk->lines = GROW_ARRAY(chunk->lines, Line, oldCapacity, chunk->lineCapacity);
        }
        chunk->lines[chunk->lineCount].line = line;
        chunk->lines[chunk->lineCount].count = 1;
        chunk->lineCount++;
    }
}

void freeChunk(Chunk* chunk)
{
    FREE_ARRAY(chunk->code, uint8_t, chunk->capacity);
    FREE_ARRAY(chunk->lines, Line, chunk->capacity);
    freeValueArray(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value constant)
{
    writeValueArray(&chunk->constants, constant);
    return chunk->constants.count-1;
}

int getLine(Chunk* chunk, int offset)
{
    int s = 0;
    for (int i=0;;i++)
    {
        if (s+chunk->lines[i].count > offset)
            return chunk->lines[i].line;
        s += chunk->lines[i].count;
    }
}