# ./LSHADE 30 10 1 100000 >> CEC_test_1.txt

 j=1
   for (( j; j<31; j=j+1 ))
   do
        echo $j
        ./LSHADE 30 30 $j 300000 >> CEC_2017/30D/LSHADE_CEC_${j}.txt
   done