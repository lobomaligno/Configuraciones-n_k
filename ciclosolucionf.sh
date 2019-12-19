#!/bin/bash
# Script de hola mundo
cat S*.txt|sort|uniq>st.txt
S=$(wc -l <st.txt)
echo "Hay $S soluciones totales"