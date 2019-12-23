#!/bin/bash
# Script de hola mundo
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