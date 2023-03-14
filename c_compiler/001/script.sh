#!bin/bash
assert(){
    expected="$1"
    input="$2"

    ./write_to_asm "$input"
    cc file.asm -o asm
    
}