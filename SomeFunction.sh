# ./LSHADE 30 10 1 100000 >> CEC_test_1.txt



 j=1
   for (( j; j<7; j=j+1 ))
   do
        echo $j
        ./LSHADE 30 100 $j 1000000 >> SomeFunction/100D/LSHADE_CEC_${j}.txt
   done

#  j=1
#    for (( j; j<13; j=j+1 ))
#    do
#         echo $j
#         ./LSHADE 30 30 $j 300000 >> SomeFunction/30D/LSHADE_CEC_${j}.txt
#    done


#      j=1
#      for (( j; j<13; j=j+1 ))
#      do
#         echo $j
#         ./LSHADE 30 50 $j 500000 >> SomeFunction/50D/LSHADE_CEC_${j}.txt
#      done

# j=1
#    for (( j; j<13; j=j+1 ))
#    do
#         echo $j
#         ./LSHADE 30 100 $j 1000000 >> SomeFunction/100D/LSHADE_CEC_${j}.txt
#    done