#!/bin/bash
# measurement overhead
echo '---Measure Network---'
echo 'RTT'
for i in `seq 10`
do ./rtt/server.out 21999
./rtt/client.out localhost 21999
done &
wait;