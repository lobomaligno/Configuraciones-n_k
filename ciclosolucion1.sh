#!/bin/bash
i=0
while [ 71 -ge $i ]
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
    ./fusion
    ./repartir 0
    exit
fi
