#!/bin/bash

rm *.o *.exe *.txt

#~ Run Particle System 

g++ -c coll2p4w.cpp
g++ -o coll2p4w.exe coll2p4w.o -lm
./coll2p4w.exe
