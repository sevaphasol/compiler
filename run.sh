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

TMPFILE_FRONT=$(mktemp frontend_tmp.XXXXXX)
TMPFILE_MIDDLE=$(mktemp middleend_tmp.XXXXXX)

front-end/build/frontend "$SRCFILE" "$TMPFILE_FRONT"
middle-end/build/middle-end -i "$TMPFILE_FRONT" -o "$TMPFILE_MIDDLE"
back-end/build/backend -i "$TMPFILE_MIDDLE" -o "$OUTFILE" -S "$ASMFILE"

rm -f "$TMPFILE_FRONT"
rm -f "$TMPFILE_MIDDLE"
