#!/bin/bash
i=0
echo "Paso"
let start=`date +%s`
while [ 71 -ge $i ]
do
    ./paso $i &
    let i=$i+1
done
wait
let end=`date +%s`
let runtime=$end-$start
echo "Tardo: $runtime"
df -h
