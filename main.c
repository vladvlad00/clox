#include "common.h"
#include "chunk.h"
#include "debug.h"

int main()
{
    Chunk chunk;

    initChunk(&chunk);

    int x = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 12);
    writeChunk(&chunk,x, 12);
    writeChunk(&chunk, OP_RETURN, 13);
    disassembleChunk(&chunk, "test");
    freeChunk(&chunk);
    return 0;
}