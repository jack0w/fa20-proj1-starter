#!/bin/bash

for i in $(seq 10000 10100)
do
	diff testOutputs/GliderGuns/GliderGuns_0x1808_${i}.ppm studentOutputs/GliderGuns/GliderGuns_0x1808_${i}.ppm
	if [ $? -ne 0 ]
	then 
		exit 1
	else
		echo "step $i match"
	fi
done
