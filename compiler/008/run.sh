#!/bin/bash

for dir in ./*.dSYM/; do
  if [[ -d "$dir" ]]; then
    rm -r "$dir"
  fi
done

rm -rf exe && gcc -fsanitize=address -fsanitize=null -g3 file.c parsing.c tools.c debug.c -o exe