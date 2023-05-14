# Get the name of the operating system
OS=$(uname)
# Check if the operating system is macOS
if [[ "$OS" == "Darwin" ]]; then
    # if [[ compiler.c -nt exe ]]; then
    gcc -fsanitize=address -fsanitize=null -g3 -O2 compiler.c -o mini
    # fi
      # ./exe $1
# Check if the operating system is Ubuntu
elif [[ "$OS" == "Linux" ]] && grep -q "Ubuntu" /etc/os-release; then
    export LSAN_OPTIONS=detect_leaks=0
    if [[ compiler.c -nt exe ]]; then
      gcc -Wall -Werror -Wextra -fsanitize=address -fsanitize=null -g3 -O2 compiler.c -o mini
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

#!/bin/bash

# Create the 'mini' folder in the home directory if it doesn't exist
mkdir -p "$HOME/mini"

# Move the file './mini' to the 'mini' folder
mv ./mini "$HOME/mini"

# Add the 'mini' folder to the PATH environment variable for Linux
if ! grep -qxF 'export PATH="$HOME/mini:$PATH"' "$HOME/.bashrc"; then
  echo 'export PATH="$HOME/mini:$PATH"' >> "$HOME/.bashrc"
  source "~/.bashrc"
fi

# Add the 'mini' folder to the PATH environment variable for macOS with zsh and ohmyzsh
if [ "${SHELL##*/}" = "zsh" ]; then
  if ! grep -qxF 'export PATH="$HOME/mini:$PATH"' "$HOME/.zshrc"; then
    echo 'export PATH="$HOME/mini:$PATH"' >> "$HOME/.zshrc"
    source "~/.zshrc"
  fi
fi

