#!/bin/bash
find . -type f | while read -r file; do
  dir=$(dirname "$file")
  base=$(basename "$file")
  new_base=$(echo "$base" | tr '[:upper:]' '[:lower:]' | sed 's/\([a-z0-9]\)\([A-Z]\)/\1_\2/g' | tr '[:upper:]' '[:lower:]')
  mv "$file" "$dir/$new_base"
done