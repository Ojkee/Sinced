#!/bin/bash

SRC_DIR="src"
BUILD_DIR="build"
BIN_DIR="$BUILD_DIR/bin"
EXECUTABLE="main"
LINK_PATH="/usr/local/bin/scd"
RECORDS_DIR="records"

mkdir -p "$BIN_DIR"

spinner() {
    local pid=$1
    local delay=0.1
    local spinstr='|/-\'
    while [ "$(ps a | awk '{print $1}' | grep -w $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

echo "Compiling project..."
clang++ -std=c++20 -Wall -Wextra -O2 "$SRC_DIR/main.cc" -o "$BIN_DIR/$EXECUTABLE" \
    "$SRC_DIR/date/base_date.cc" \
    "$SRC_DIR/date/format_date.cc" \
    "$SRC_DIR/date/format_date_factory.cc" \
    "$SRC_DIR/entry/entry_base.cc" \
    "$SRC_DIR/entry/entry_filter.cc" \
    "$SRC_DIR/entry/entry_filter_factory.cc" \
    "$SRC_DIR/entry/entry_formatter.cc" \
    "$SRC_DIR/entry/entry_handler.cc" \
    "$SRC_DIR/entry/entry_sorter.cc" \
    "$SRC_DIR/entry/entry_sorter_factory.cc" \
    "$SRC_DIR/mcg_reader/reader.cc" \
    "$SRC_DIR/syntax_analysis/interpreter.cc" \
    "$SRC_DIR/syntax_analysis/lexer.cc" \
    "$SRC_DIR/syntax_analysis/token.cc" -I include &
compilation_pid=$!

spinner $compilation_pid

wait $compilation_pid
if [ $? -ne 0 ]; then
    echo "Compilation failed."
    exit 1
fi

if [ ! -f "$BIN_DIR/$EXECUTABLE" ]; then
    echo "Error: Executable not found at $BIN_DIR/$EXECUTABLE"
    exit 1
fi

if [ -L "$LINK_PATH" ]; then
    sudo rm "$LINK_PATH"
fi
sudo ln -s "$(pwd)/$BIN_DIR/$EXECUTABLE" "$LINK_PATH"

echo "Setting permissions for records directory..."
sudo chmod -R 777 "$RECORDS_DIR"

echo "Setup complete. You can now use the 'scd' command."
