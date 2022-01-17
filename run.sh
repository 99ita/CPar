#!/bin/bash
echo Compiling...
make
if (($# == 3)); 
then
	srun --partition=day --nodelist=compute-134-111 ./bSort $1 $2 $3
	exit 0
fi
if (($# == 4));
then
	srun --partition=day --nodelist=compute-134-111 ./bSort $1 $2 $3 $4
	exit 0
fi
if (($# == 5));
then
	srun --partition=day --nodelist=compute-134-111 ./bSort $1 $2 $3 $4 $5
	exit 0
else 
	srun --partition=day --nodelist=compute-134-111 ./bSort
	exit 0
fi
