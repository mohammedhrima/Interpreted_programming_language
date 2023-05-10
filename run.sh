#!/bin/bash

# Get the name of the operating system
OS=$(uname)
# Check if the operating system is macOS
if [[ "$OS" == "Darwin" ]]; then
    # if [[ compiler.c -nt exe ]]; then
    gcc -fsanitize=address -fsanitize=null -g3 -O2 compiler.c
    # fi
      # ./exe $1
# Check if the operating system is Ubuntu
elif [[ "$OS" == "Linux" ]] && grep -q "Ubuntu" /etc/os-release; then
    export LSAN_OPTIONS=detect_leaks=0
    if [[ compiler.c -nt exe ]]; then
      gcc -Wall -Werror -Wextra -fsanitize=address -fsanitize=null -g3 -O2 compiler.c
    fi
      ./exe $1
else
    echo "Unsupported operating system: $OS"
    exit 1
fi

for dir in ./*.dSYM/; do
  if [[ -d "$dir" ]]; then
    rm -r "$dir"
  fi
done
