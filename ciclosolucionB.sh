#!/bin/bash
i=0
echo "Paso"
let start=`date +%s`
while [ 71 -ge $i ]
do
    ./Bpaso $i &
    let i=$i+1
done
wait
let end=`date +%s`
let runtime=$end-$start
echo "Tardo: $runtime"
df -h
echo "Borrar solucion%s.txt"
i=0
while [ 71 -ge $i ]
do
    rm solucion$i.bin
    let i=$i+1
done
df -h
