#!/bin/bash

g++ -g BS_assignment.cpp -o hnnn.out
g++ -g resource_allocation_SR.cpp -o hnra.out
#g++ -g resource_allocation_IR.cpp -o hnra.out

F=1;
F_r=2;
tsir=0.3;

for k in `seq 4 4`;
do
     	for n in `seq 1 60`;
	do	
		echo "k=$k,n=$n" >> R03_SoP.txt
		echo "k=$k,n=$n" >> R03_SoS.txt
		echo "k=$k,n=$n" >> R03_OR.txt

		for itr in `seq 1 10`;
		do	
			echo "k=$k,n=$n,iteration=$itr"	
			#sleep 1
			
			if [ $itr -eq 10 ]
			then
				sed -n "/^n=$n,iteration=10/,/^n=$(($n + 1)),iteration=1/p" R01_LoN.txt | awk '/^x/'|sed 's/;//g'|sed 's/x//g'|sed 's/ = / /g'|sed 's/\[//g'|sed 's/\]//g'>R01_x_n.txt
				sed -n "/^n=$n,iteration=10/,/^n=$(($n + 1)),iteration=1/p" R01_LoN.txt | awk '/^y/'|sed 's/;//g'|sed 's/y//g'|sed 's/ = / /g'|sed 's/\[//g'|sed 's/\]//g'>R01_y_n.txt
			else
				sed -n "/^n=$n,iteration=$itr$/,/^n=$n,iteration=$(($itr + 1))$/p" R01_LoN.txt | awk '/^x/'|sed 's/;//g'|sed 's/x//g'|sed 's/ = / /g'|sed 's/\[//g'|sed 's/\]//g'>R01_x_n.txt
				sed -n "/^n=$n,iteration=$itr$/,/^n=$n,iteration=$(($itr + 1))$/p" R01_LoN.txt | awk '/^y/'|sed 's/;//g'|sed 's/y//g'|sed 's/ = / /g'|sed 's/\[//g'|sed 's/\]//g'>R01_y_n.txt
			fi

			./hnnn.out $n $F $F_r
			./hnra.out $n $F $k

			rm R02_NN.txt
			rm R01_x_n.txt
			rm R01_y_n.txt
		done 
	done    
done 

rm hnnn.out
rm hnra.out
