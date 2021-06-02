# ./LSHADE 30 10 1 100000 >> CEC_test_1.txt

 j=1
   for (( j; j<31; j=j+1 ))
   do
        echo $j
        ./LSHADE 30 50 $j 500000 >> CEC_2017/50D/LSHADE_CEC_${j}.txt
   done