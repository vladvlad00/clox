#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum
{
    OP_CONSTANT,
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_NEGATE,
    OP_ADD,
    OP_SUBTRACT,
    OP_MULTIPLY,
    OP_DIVIDE,
    OP_RETURN
} OpCode;

typedef struct
{
    int line;
    int count;
} Line;

typedef struct
{
    int count;
    int capacity;
    int lineCount;
    int lineCapacity;
    Line* lines;
    ValueArray constants;
    uint8_t* code;
} Chunk;

void initChunk(Chunk* chunk); //initializes a chunk
void writeChunk(Chunk* chunk, uint8_t byte, int line); //appends a byte at the end of a chunk
void freeChunk(Chunk* chunk); //frees a chunk
int addConstant(Chunk* chunt, Value constant); //adds a constant to the chunk and it returns its index
int getLine(Chunk* chunk, int offset);
#endif
