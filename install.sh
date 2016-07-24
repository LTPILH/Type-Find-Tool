#!bin/bash

echo "installing..."

make

make clean

mv tyfd /usr/local/tyfd
