#!/bin/bash
i=0
while [ 19 -ge $i ]
do
    ./paso $i
    let i=$i+1
done
let i=$1-$2
echo "$i"
if [ $i = 6 ];
    then
    ./ciclosolucionf.sh
exit 0
else
    i=0
    while [ 18 -ge $i ]
    do
        let j=$i+1
        ./fusion $i $j
        let i=$i+2
    done
    i=0
    while [ 16 -ge $i ]
    do
        let j=$i+2
        ./fusion $i $j
        let i=$i+4
    done
    ./fusion 0 4
    ./fusion 8 12 16
    ./fusion 0 8
    ./repartir 0
    exit
fi