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
	for ((i = 1; i <= 9; i+= 2))
	do
		echo "i = $i"
		./knn datasets/$base/$base".data" $i > results_paillier_crt/uniform/"$base"_$i;
	done
done

for base in abalone climate credit iris wall wine
do
	echo $base
	for ((i = 1; i <= 9; i+= 2))
	do
		echo "i = $i"
		./wknn datasets/$base/$base".data" $i > results_paillier_crt/weighted/"$base"_$i;
	done
done
