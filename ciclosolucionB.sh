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
echo "Borrar solucion%s.bin"
i=0
while [ 71 -ge $i ]
do
    rm solucion$i.bin
    let i=$i+1
done
df -h
echo "fusion"
let start=`date +%s`
./Bfusion
let end=`date +%s`
let runtime=$end-$start
echo "Tardo: $runtime"
if [ $2 -lt $(($1-2*$3-1)) ]; then
    ./Brepartir 0
fi
exit

