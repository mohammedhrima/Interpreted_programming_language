
# Add the 'mini' folder to the PATH environment variable for macOS with zsh and ohmyzsh
if [ "${SHELL##*/}" = "zsh" ]; then
  file="$HOME/.zsh"
  if [ ! -f "$file" ]; then
  echo "File not found: $file"
  exit 1
  fi
  temp_file=$(mktemp)

  # Remove the specific line from the file
  sed '/export PATH="$HOME\/mini:$PATH"/d' "$file" > "$temp_file"

  # Overwrite the original file with the modified content
  mv "$temp_file" "$file"
fi



rm -rf "$HOME/mini"

