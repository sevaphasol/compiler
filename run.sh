#!/bin/bash

if [ "$#" -eq 2 ]; then
    SRCFILE="$1"
    OUTFILE="$2"
    ASMFILE="${OUTFILE%.*}.s"
elif [ "$#" -eq 1 ]; then
    SRCFILE="$1"
    OUTFILE="${SRCFILE%.*}.out"
    ASMFILE="${OUTFILE%.*}.s"
else
    echo "Usage: $0 <input-file> [output-file]"
    exit 1
fi

TMPFILE=$(mktemp frontend_tmp.XXXXXX)

front-end/build/frontend "$SRCFILE" "$TMPFILE"
back-end/build/backend -i "$TMPFILE" -o "$OUTFILE" -S "$ASMFILE"

rm -f "$TMPFILE"
