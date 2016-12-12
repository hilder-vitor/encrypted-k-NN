#!/bin/bash

if [ ! -d results_paillier_crt/ ]; then
	mkdir -p results_paillier_crt/uniform/
	mkdir -p results_paillier_crt/weighted/
elif [ ! -d results_paillier_crt/uniform/ ]; then
	mkdir results_paillier_crt/uniform/
elif [ ! -d results_paillier_crt/weighted/ ]; then
	mkdir results_paillier_crt/weighted/
fi

for base in abalone climate credit iris wall wine
do
	echo $base
	for ((k = 1; k <= 9; k+= 2))
	do
		echo "k = $k"
		python knn.py -m uniform -f datasets/$base/$base".data" -k $k
		./knn datasets/$base/$base".data" $k > results_paillier_crt/uniform/"$base"_$k;
	done
done

for base in abalone climate credit iris wall wine
do
	echo $base
	for ((k = 1; k <= 9; k+= 2))
	do
		echo "k = $k"
		python knn.py -m distance -f datasets/$base/$base".data" -k $k
		./wknn datasets/$base/$base".data" $k > results_paillier_crt/weighted/"$base"_$k;
	done
done
