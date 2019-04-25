#!/bin/bash

find . -maxdepth 1 '(' -name "*.h" -o -name "*.c" ')' \
    -exec echo '```c' ';' -exec cat {} ';' -exec echo '```' ';' >  report.md
