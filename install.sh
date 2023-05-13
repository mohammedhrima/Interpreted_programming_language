folder_name="mini"  # Specify the name of the folder

# Construct the path to the folder within the home directory
folder_path="$HOME/$folder_name"

# Create the folder
mkdir -p "$folder_path"

# Check if the folder was created successfully


# Get the name of the operating system
OS=$(uname)
# Check if the operating system is macOS
if [[ "$OS" == "Darwin" ]]; then
    # if [[ compiler.c -nt exe ]]; then
    gcc -fsanitize=address -fsanitize=null -g3 -O2 compiler.c -o exe
    # fi
      # ./exe $1
# Check if the operating system is Ubuntu
elif [[ "$OS" == "Linux" ]] && grep -q "Ubuntu" /etc/os-release; then
    export LSAN_OPTIONS=detect_leaks=0
    if [[ compiler.c -nt exe ]]; then
      gcc -Wall -Werror -Wextra -fsanitize=address -fsanitize=null -g3 -O2 compiler.c -o exe
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

mv ./exe ~/mini

command_alias="mini"     # Specify the alias name
command_to_alias="~/mini/exe"   # Specify the command to be aliased

# Function to check if alias exists in the given file
check_alias_exists() {
    grep -q "alias $1=" "$2"
}

# Check if ~/.zshrc exists and alias doesn't exist, then add it
if [ -f "$HOME/.zshrc" ] && ! check_alias_exists "$command_alias" "$HOME/.zshrc"; then
    echo "Adding alias to ~/.zshrc"
    echo "alias $command_alias=\"$command_to_alias\"" >> "$HOME/.zshrc"
fi

# Check if ~/.bashrc exists and alias doesn't exist, then add it
if [ -f "$HOME/.bashrc" ] && ! check_alias_exists "$command_alias" "$HOME/.bashrc"; then
    echo "Adding alias to ~/.bashrc"
    echo "alias $command_alias=\"$command_to_alias\"" >> "$HOME/.bashrc"
fi

# Source the appropriate rc file if it exists
if [ -f "$HOME/.zshrc" ]; then
    source "$HOME/.zshrc"
elif [ -f "$HOME/.bashrc" ]; then
    source "$HOME/.bashrc"
fi
