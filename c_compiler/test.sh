#!/bin/bash
file="$1"
#expected="$2"
input="$2"

gcc -fsanitize=address -fsanitize=null $file.c -o cexe
./cexe $input > $file.asm
rm -rf ./cexe
gcc $file.asm -o ./aexe
./aexe
#rm -rf ./aexe
