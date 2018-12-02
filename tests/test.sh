#!/bin/bash

err=0

for i in {001..2}
do

	correct_output=$(<out_$i)		
	../ifj18 test_$i


	if [ "$?" -ne "$correct_output" ];then
  		echo "ERROR_file_test_"$i"   expected-"$correct_output"  get-"$? > error.log
		err=1
	fi
	
done

	if [ "$err" -eq 1 ];then
		echo "ERROR has occurred and error.log file was created"
	fi
