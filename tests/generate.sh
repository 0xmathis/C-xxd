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
    number=$2

    for ((i=1; i<=$number; i++))
    do
        filename=$outputDir$(cat /dev/urandom | tr -dc 'a-z0-9' | fold -w 32 | head -n 1).bin
        dd if=/dev/urandom of=$filename bs=$size count=1 &> /dev/null
        echo Generated $filename
    done
}

# Clean test dir
rm -rf $outputDir/*.bin

for ((j=1; j<=16; j++))
do
    generate $j 5
done

generate $smallSize $n
generate $bigSize $n
