#ifndef clox_vm_h
#define clox_vm_h

#include "chunk.h"
#include "value.h"
#define STACK_MAX 256

typedef struct
{
    Chunk *chunk;
    uint8_t *ip; //instruction pointer
    Value stack[STACK_MAX];
    Value *stacktop;
} VM;

typedef enum
{
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpreterResult;

void initVM();
void freeVM();
InterpreterResult interpret(Chunk *source);
void push(Value value);
Value pop();


#endif