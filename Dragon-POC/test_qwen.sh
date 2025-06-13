#!/bin/bash

if [ -e qwen_binaries ]
then
        rm -r qwen_binaries
fi

if ! command -v /usr/bin/time &> /dev/null; then
    echo "Error: /usr/bin/time is required but not found."
    exit 1
fi


mkdir qwen_binaries
for file in qwen/*.cpp; do
    if [[ -f "$file" ]]; then
        filename=$(basename "$file")
        newname="${filename%.cpp}"
        echo "Source code : $filename"
        echo "Binary: $newname"

        g++ $file -o qwen_binaries/$newname
        /usr/bin/time -f "\nMax Memory: %M KB\nUser Time: %U sec\nSystem Time: %S sec\nElapsed Time: %E" ./validate_binary.sh qwen_binaries/$newname
    fi
done

exit 0
