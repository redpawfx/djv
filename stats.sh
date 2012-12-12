#!/bin/sh

find . -name "*.h" -or -name "*.cpp" | xargs wc -l

find . -name "*.h" -or -name "*.cpp" | wc -l
