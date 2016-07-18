#!bin/bash

echo "installing..."

make

make t_fbuild

make clean
