#ifndef clox_compiler_h
#define clox_compiler_h
#include "vm.h"

void compile(const char *source, Chunk *chunk);

#endif