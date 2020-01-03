#!/bin/bash
i=0
echo "Paso"
while [ 71 -ge $i ]
do
    ./paso $i
    let i=$i+1
done
let i=$1-$2
#echo "$i"
if [ $i = 6 ];
    then
    echo "Final"
    let start=`date +%s`
        ./ciclosolucionf.sh
    let end=`date +%s`
    echo "Tardo: +$(end-start)"
exit 0
else
    echo "fusion"
    let start=`date +%s`
        ./fusion
    let end=`date +%s`
    echo "Tardo: +$(end-start)"
    let start=`date +%s`
    echo "repartir"
        ./repartir 0
    let end=`date +%s`
    echo "Tardo: +$(end-start)"
    exit
fi
