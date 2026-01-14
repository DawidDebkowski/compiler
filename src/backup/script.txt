#!/bin/sh

src="."
dst="./backup"
mkdir -p "$dst"

for f in "$src"/*; do
    [ -f "$f" ] || continue
    name=$(basename "$f")
    base="${name%.*}"

    case "$name" in
        *.hpp)
            cp "$f" "$dst/${base}_hpp.txt"
            ;;
        *)
            cp "$f" "$dst/${base}.txt"
            ;;
    esac
done

