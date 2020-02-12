#include "vm.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"
#include <stdio.h>

VM vm;

void initVM()
{
    resetStack();
}

void freeVM()
{

}

InterpretResult interpret(const char* source)
{
    Chunk chunk;

    initChunk(&chunk);
    if (!compile(source, &chunk))
    {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    vm.chunk = &chunk;
    vm.ip = vm.chunk->code;

    InterpretResult result = run();
    freeChunk(&chunk);
    return result;
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
#define BINARY_OP(op) \
    do \
    { \
       Value b = pop(); \
       Value a = pop(); \
       push(a op b); \
    } while (false)
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
            case OP_NEGATE:
                push(-pop());
                break;
            case OP_ADD:
                BINARY_OP(+);
                break;
            case OP_SUBTRACT:
                BINARY_OP(-);
                break;
            case OP_MULTIPLY:
                BINARY_OP(*);
                break;
            case OP_DIVIDE:
                BINARY_OP(/);
                break;
            case OP_CONSTANT:
            {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case OP_RETURN:
                printValue(pop());
                printf("\n");
                return INTERPRET_OK;
        }
    }
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
}