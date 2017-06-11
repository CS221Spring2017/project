#!/bin/bash
echo '---sequential access---'
for ((i=1;i<10;i++))
do
	sudo purge &&
	./cont.o $i 0
done

echo '----random access----'
for ((i=1;i<10;i++))
do
	sudo purge &&
	./cont.o $i 1
done

exit 0