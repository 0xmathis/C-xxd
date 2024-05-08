#!/bin/bash

# Number of files to generate
n=10

# Files size in bytes
smallSize=1024 # 2^10
bigSize=32768 # 2^15

# Output dir
outputDir="tests/"

generate() {
    size=$1

    for ((i=1; i<=n; i++))
    do
        filename=$outputDir$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 32 | head -n 1).bin
        dd if=/dev/urandom of=$filename bs=$size count=1 &> /dev/null
        echo Generated $filename
    done
}

# Clean test dir
rm -rf $outputDir/*.bin

generate $smallSize
generate $bigSize
