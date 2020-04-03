#!/bin/zsh

echo "#include<stdio.h>\n\nint main() { puts (\"Hello world\"); }" | gcc -x c - -c -o hello.o

echo "this is my special data" > mydata

objcopy --add-section .mydata=mydata \
--set-section-flags .mydata=noload,readonly hello.o hello2.o

gcc hello.o -o hello
gcc hello2.o -o hello2

./hello
./hello2

#objdump -sj .mydata hello

#readelf -s  hello.o
#readelf -s  hello2.o


readelf -S hello.o
readelf -S hello2.o


diff <(readelf -s hello.o) <(readelf -s hello2.o)
diff <(readelf -S hello.o) <(readelf -S hello2.o)

