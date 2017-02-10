#!/bin/sh
add_minidebug()
{
    local debuginfo="$1"
    local binary="$2"

    local dynsyms=`mktemp`
    local funcsyms=`mktemp`
    local keep_symbols=`mktemp`
    local mini_debuginfo=`mktemp`

    # Extract the dynamic symbols from the main binary, there is no need to also have these
    # in the normal symbol table
    nm -D "$binary" --format=posix --defined-only | awk '{ print $1 }' | sort > "$dynsyms"
    # Extract all the text (i.e. function) symbols from the debuginfo 
    nm "$debuginfo" --format=posix --defined-only | awk '{ if ($2 == "T" || $2 == "t") print $1 }' | sort > "$funcsyms"
    # Keep all the function symbols not already in the dynamic symbol table
    comm -13 "$dynsyms" "$funcsyms" > "$keep_symbols"
    # Copy the full debuginfo, keeping only a minumal set of symbols and removing some unnecessary sections
    objcopy -S --remove-section .gdb_index --remove-section .comment --keep-symbols="$keep_symbols" "$debuginfo" "$mini_debuginfo" &> /dev/null
    #Inject the compressed data into the .gnu_debugdata section of the original binary
    xz "$mini_debuginfo"
    mini_debuginfo="${mini_debuginfo}.xz"
    objcopy --add-section .gnu_debugdata="$mini_debuginfo" "$binary"
    cp "$dynsyms" "$funcsyms" "$keep_symbols" "$mini_debuginfo" .
    rm -f "$dynsyms" "$funcsyms" "$keep_symbols" "$mini_debuginfo"
}

add_minidebug $1 $2
