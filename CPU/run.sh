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
for i in `seq 3`
do ./build/procedure.o 10000
done &
wait;

echo '---system call overhead---'
for i in `seq 1`
do ./build/system.o 10
done &
wait;

echo '---task creation (process) overhead---'
for i in `seq 10`
do ./build/create_process.o 100
done &
wait;

echo '---task creation (thread) overhead---'
for i in `seq 10`
do ./build/create_thread.o 2
done &
wait;

echo '---Context switch (process) overhead---'
for i in `seq 10`
do ./build/context_switch_p.o 100
done &
wait;

echo '---Context switch (thread) overhead---'
for i in `seq 12`
do ./build/context_switch_t.o 100
done &
wait;


