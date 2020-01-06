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
echo "fusion"
let start=`date +%s`
./fusion
let end=`date +%s`
let runtime=$end-$start
echo "Tardo: $runtime"
if [ $2 < 2*$3 ]; then
    ./repartir 0
fi
exit
