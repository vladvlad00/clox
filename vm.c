#include "vm.h"
#include "common.h"
#include "debug.h"
#include "compiler.h"
#include <stdio.h>
#include <stdarg.h>

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

static void runtimeError(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");

    size_t instruction = vm.ip-vm.chunk->code;
    int line = getLine(vm.chunk,instruction);
    printf("[line %d] in script\n", line);

    resetStack();
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

static Value peek(int distance)
{
    return vm.stackTop[-1-distance];
}

static InterpretResult run()
{
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op) \
    do \
    { \
        if (!IS_NUMBER(peek(0)) || !IS_NUMBER(peek(1))) \
        { \
            runtimeError("Operands must be numbers"); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        double b = AS_NUMBER(pop()); \
        double a = AS_NUMBER(pop()); \
        push(valueType(a op b)); \
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
                if (!IS_NUMBER(peek(0)))
                {
                    runtimeError("Operand must be a number");
                    return INTERPRET_RUNTIME_ERROR;
                }
                push(NUMBER_VAL(-AS_NUMBER(pop())));
                break;
            case OP_ADD:
                BINARY_OP(NUMBER_VAL, +);
                break;
            case OP_SUBTRACT:
                BINARY_OP(NUMBER_VAL, -);
                break;
            case OP_MULTIPLY:
                BINARY_OP(NUMBER_VAL, *);
                break;
            case OP_DIVIDE:
                BINARY_OP(NUMBER_VAL, /);
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