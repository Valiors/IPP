#!/usr/bin/env bash

TEST_FLAGS="--error-exitcode=15 --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all -q"

for INPUT_FILE in $2/*.in
do
	echo "Processing file: ${INPUT_FILE#$2/}"

	TMP_FILE="$(mktemp)";
	
	valgrind $TEST_FLAGS ./$1 < $INPUT_FILE 1>$TMP_FILE.out 2>$TMP_FILE.err
	VALGRIND_RESULT=$?
	
	diff ${INPUT_FILE%in}out $TMP_FILE.out &>/dev/null
	DIFF_OUT_RESULT=$?
	
	diff ${INPUT_FILE%in}err $TMP_FILE.err &>/dev/null
	DIFF_ERROR_RESULT=$?
	
	echo -n "OUTPUT: "
	if [ $DIFF_OUT_RESULT -eq 0 ]
	then	
		echo "OK"
	else
		echo "MISMATCH"
	fi	

	echo -n "STDERR: "
	if [ $DIFF_ERROR_RESULT -eq 0 ]
	then	
		echo "OK"
	else
		echo "MISMATCH"
	fi	

	echo -n "VALGRIND: "
	if [ $VALGRIND_RESULT -eq 0 ]
	then	
		echo "OK"
	else
		echo "MISMATCH"
	fi
done
