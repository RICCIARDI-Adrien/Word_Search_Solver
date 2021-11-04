#!/bin/sh

Files=$(find . -iname "*.txt" | sort)

for File in $Files
do
	../word-search-solver $File
	if [ $? -ne 0 ]
	then
		printf "\033[31m+-------------+\033[0m\n"
		printf "\033[31m| Test failed |\033[0m\n"
		printf "\033[31m+-------------+\033[0m\n"
		exit 1
	fi

	printf "\n"
done

printf "\033[32m+---------------------+\033[0m\n"
printf "\033[32m| All tests succeeded |\033[0m\n"
printf "\033[32m+---------------------+\033[0m\n"
