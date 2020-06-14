#!/bin/bash

g++ -g node_generator.cpp -o B01_ng.out

for n in `seq 1 60`;
do	
	for itr in `seq 1 10`;
	do	
		echo "n=$n,iteration=$itr"
		echo "n=$n,iteration=$itr" >> R01_LoN.txt
		./B01_ng.out $n	
		sleep 1
	done 
done

