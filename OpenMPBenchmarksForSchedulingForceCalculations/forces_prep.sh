#!/bin/bash

rm *.o *.exe *.txt

#~

echo "Running in serial" 
g++ -c forces_prep.cpp
g++ -o forces_prep.exe forces_prep.o -lm
time ./forces_prep.exe >> forces_prep.txt

#~

echo "Running in parallel"
g++ -DOMP -fopenmp -c forces_prep.cpp
g++ -DOMP -fopenmp -o forces_prep.exe forces_prep.o -lm
time ./forces_prep.exe >> forces_prep.txt
