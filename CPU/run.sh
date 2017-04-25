#!/bin/bash
# measurement overhead
echo '---measurement overhead---'
echo 'reading overhead'
for i in `seq 10`
do ./build/measure1.out 10000
done &
wait;
echo 'loop overhead'
for i in `seq 10`
do ./build/measure2.out 10000
done &
wait;
echo '---procedure overhead---'
./build/procedure.o 10000
echo '---system call overhead---'
./build/system.o 10
echo '---task creation (process) overhead---'
./build/create_process.o 100

