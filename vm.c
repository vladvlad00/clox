#include "vm.h"
#include "common.h"
#include "debug.h"
#include <stdio.h>

VM vm;

void initVM()
{
    resetStack();
}

void freeVM()
{

}

InterpretResult interpret(Chunk* chunk)
{
    vm.chunk = chunk;
    vm.ip = chunk->code;
    return run();
}

static void resetStack()
{
    vm.stackTop = vm.stack;
}

void push(Value value)
{
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop()
{
    vm.stackTop--;
    return *vm.stackTop;
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
    for (;;)
    {
#ifdef DEBUG_TRACE_EXECUTION
        printf("STACK:\n");
        for (Value* stackElement=vm.stack; stackElement < vm.stackTop; stackElement++)
        {
            printf("[ ");
            printValue(*stackElement);
            printf(" ]\n");
        }

        disassembleInstruction(vm.chunk, (int)(vm.ip-vm.chunk->code));
#endif
        uint8_t instruction = READ_BYTE();
        switch (instruction)
        {
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
            case OP_CONSTANT:
            {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
}