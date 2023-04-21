#!/bin/bash

for dir in ./*.dSYM/; do
  if [[ -d "$dir" ]]; then
    rm -r "$dir"
  fi
done

# Get the name of the operating system
OS=$(uname)

# Check if the operating system is macOS
if [[ "$OS" == "Darwin" ]]; then
    rm -rf exe && gcc -fsanitize=address -fsanitize=null -g3 file.c  -o exe && ./exe
# Check if the operating system is Ubuntu
elif [[ "$OS" == "Linux" ]] && grep -q "Ubuntu" /etc/os-release; then
    export LSAN_OPTIONS=detect_leaks=0
    rm -rf exe && gcc -fsanitize=address -fsanitize=null -g3 -lm file.c  -o exe && ./exe
else
    echo "Unsupported operating system: $OS"
    exit 1
fi


#rm -rf exe && gcc -fsanitize=address -fsanitize=null -g3 file.c parsing.c tools.c debug.c -o exe

