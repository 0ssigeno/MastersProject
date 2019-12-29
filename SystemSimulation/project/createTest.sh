#!/bin/sh

export PATH=$PATH:~/omnetpp-5.4.1/bin/

if [ $# -ne 2 ]
  then
    echo "Argument 1 = module name"
  	echo "Argument 2 = number of repetitions"
else
	cd "./samples/$1/"
	#checking if the omnet.ini has to be modified
	VALUE=$(cat "omnetpp.ini" | grep "repeat ="  | tr -dc '0-9')
	if [ -z $VALUE ]
	then
		echo "repeat = $2" >> omnetpp.ini
	fi
	if [ $VALUE -ne $2 ]
		then
			head -n -1 omnetpp.ini > temp.txt ; mv temp.txt omnetpp.ini
			echo "repeat = $2" >> omnetpp.ini
			
	fi
	rm -rf ./results/*
	#has to start from zero
	NUMBER=$(($2-1))
	./$1   -m -u Cmdenv  -n .:../queueinglib -l ../queueinglib/queueinglib omnetpp.ini  > /dev/null #Non voglio stampe inutili


	cd results
	array=$( ls . )
	for str in $array
	do
		#casting to csv
		EXTENSION=$( echo -n $str | tail -c 4 )
		NAME=${str%????} #prendo prima del punto
		if [ $EXTENSION = ".vec" ]
	   	then 
	   		scavetool x $str -o $NAME.csv > /dev/null #todo solo se sono i .sca
	
		fi
	done
fi