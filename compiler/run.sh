#!/bin/bash

# Get the name of the operating system
OS=$(uname)
# Check if the operating system is macOS
if [[ "$OS" == "Darwin" ]]; then
    gcc -fsanitize=address -fsanitize=null -g3 -O2 compiler.c -o exe && ./exe
    rm -rf exe
# Check if the operating system is Ubuntu
elif [[ "$OS" == "Linux" ]] && grep -q "Ubuntu" /etc/os-release; then
    export LSAN_OPTIONS=detect_leaks=0
    gcc -fsanitize=address -fsanitize=null -g3 -lm -O2 compiler.c  -o exe && ./exe
    rm -rf exe
else
    echo "Unsupported operating system: $OS"
    exit 1
fi

for dir in ./*.dSYM/; do
  if [[ -d "$dir" ]]; then
    rm -r "$dir"
  fi
done
