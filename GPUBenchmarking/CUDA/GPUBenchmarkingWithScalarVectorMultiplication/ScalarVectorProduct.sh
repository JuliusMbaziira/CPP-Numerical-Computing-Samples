#!/bin/bash

rm *.o *.mod *.txt

#~ compile and run

nvcc -c ScalarVectorProduct.cu
nvcc ScalarVectorProduct.o -o ScalarVectorProduct.exe
./ScalarVectorProduct.exe 
