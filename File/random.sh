#!/bin/bash
echo '----random access----'
for ((j=1;j<10;j++))
do
	sudo purge &&
	./cont.o $j 1
done

exit 0