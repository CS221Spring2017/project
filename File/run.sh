#!/bin/bash

echo '--random access---'
for i in `seq 8`
do ./seq.o $i$
done &
wait;