#!bin/bash

echo "installing..."

make

make clean

mv tyfd /usr/bin/tyfd
