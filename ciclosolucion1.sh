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
let i=$1-$2
#if [ $i = 6 ]; then
#    ./ciclosolucionf.sh
#exit 0
#else
echo "fusion"
let start=`date +%s`
./fusion
let end=`date +%s`
let runtime=$end-$start
echo "Tardo: $runtime"
./repartir 0
exit
#fi
