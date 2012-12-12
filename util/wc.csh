#!/bin/csh

find bin lib plugin tests -name "*.h" -or -name "*.cxx" | xargs wc -l
