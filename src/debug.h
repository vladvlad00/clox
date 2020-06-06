#ifndef clox_debug_h
#define clox_debug_h

#include "chunk.h"

void disassembleChunk(Chunk* chunk, const char* name); //prints information about a chunk of instructions
int disassembleInstruction(Chunk* chunk, int offset); //prints information about an instruction and
                                                      // returns the offset of the next instruction
static int simpleInstruction(const char* name, int offset);
static int constantInstruction(const char* name, Chunk* chunk, int offset);

#endif //CLOX_DEBUG_H
