#!/bin/bash
rm *.exe *.o 

g++ -c  main.cpp  ODESolver.cpp RK4OM.cpp arraybase.cpp

g++ -o ODESolver.exe main.o ODESolver.o RK4OM.o

./ODESolver.exe >> ODESolver_results.txt

./ODESolver.exe
