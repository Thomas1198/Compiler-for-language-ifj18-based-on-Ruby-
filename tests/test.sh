#!/bin/bash
err=0

for i in {40..98}
do

    correct_output=$(<tests/out_$i)
    correct_output=$(($correct_output + 0))
    ../ifj18 tests/test_$i


    if [ "$?" -ne "$correct_output" ];then
          echo "ERROR_file_test_"$i"   expected-"$correct_output"  get-"$? > error.log
        err=1
    fi

done

    if [ "$err" -eq 1 ];then
        echo "ERROR has occurred and error.log file was created"
    fi
