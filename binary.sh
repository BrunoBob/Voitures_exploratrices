#!/bin/bash

for file in $(ls $1); do
	if [ -d $1/$file ];
	then
		newPath="$1/$file"
		./binary.sh $newPath $2;
	elif [[ $file == *.c ]];
	then
		if [ $1 != "." ]
		then
			lenght=${#file}-2
			target=${file:0:$lenght}
			make binary target=$target path=$1 debug=$2
		fi
	fi

done


