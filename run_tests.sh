./build.sh

cd build

TEST_FOLDER="../test"

./scalc [ DIF $TEST_FOLDER/nonzero.txt $TEST_FOLDER/naturals.txt ] > nonzero_dif_naturals.txt
TEST1=`cmp nonzero_dif_naturals.txt $TEST_FOLDER/zero.txt`
if [ "$TEST1" ]
then 
    echo "DIF [1 2 3 ... ] [0 1 2 ... ] == 0, FAILED"
else
    echo "DIF [1 2 3 ... ] [0 1 2 ... ] == 0, PASSED"
fi
rm nonzero_dif_naturals.txt

./scalc [ SUM $TEST_FOLDER/odds.txt $TEST_FOLDER/evens.txt ] > odds_sum_evens.txt
TEST2=`cmp odds_sum_evens.txt $TEST_FOLDER/naturals.txt`
if [ "$TEST2" ]
then 
    echo "SUM [1 3 5 ... ] [0 2 4 ... ] == [1 2 3 4 ... ], FAILED"
else
    echo "SUM [1 3 5 ... ] [0 2 4 ... ] == [1 2 3 4 ... ], PASSED"
fi
rm odds_sum_evens.txt

./scalc [ INT $TEST_FOLDER/naturals.txt $TEST_FOLDER/evens.txt $TEST_FOLDER/zero.txt ] > int_naturals_evens_zero.txt
TEST3=`cmp int_naturals_evens_zero.txt $TEST_FOLDER/zero.txt`
if [ "$TEST3" ]
then 
    echo "INT [1 2 3 ... ] [1 2 3 ... ] [ 0 ] == [ 0 ], FAILED"
else
    echo "INT [1 2 3 ... ] [1 2 3 ... ] [ 0 ] == [ 0 ], PASSED"
fi
rm int_naturals_evens_zero.txt

./scalc [ GR 2 $TEST_FOLDER/nonzero.txt $TEST_FOLDER/odds.txt $TEST_FOLDER/zero.txt $TEST_FOLDER/evens.txt $TEST_FOLDER/zero.txt ] > test.txt
TEST4=`cmp test.txt $TEST_FOLDER/zero.txt`
if [ "$TEST4" ]
then 
    echo "GR 2 [1 2 3 ... ] [1 2 3 ... ] [ 0 ] [ 0 ] [0 2 4 ... ] == [ 0 ], FAILED"
else
    echo "GR 2 [1 2 3 ... ] [1 2 3 ... ] [ 0 ] [ 0 ] [0 2 4 ... ] == [ 0 ], PASSED"
fi
rm test.txt

./scalc [ EQ 2 $TEST_FOLDER/nonzero.txt $TEST_FOLDER/odds.txt $TEST_FOLDER/evens.txt $TEST_FOLDER/zero.txt ] > test.txt
TEST5=`cmp test.txt $TEST_FOLDER/naturals.txt`
if [ "$TEST5" ]
then 
    echo "EQ 2 [1 2 3 ... ] [1 3 5... ] [ 0 ] [0 2 4 ... ] [ 0 ] == [ N ], FAILED"
else
    echo "EQ 2 [1 2 3 ... ] [1 3 5... ] [ 0 ] [0 2 4 ... ] [ 0 ] == [ N ], PASSED"
fi
rm test.txt

./scalc [ LE 2 $TEST_FOLDER/odds.txt $TEST_FOLDER/evens.txt $TEST_FOLDER/zero.txt ] > test.txt
TEST6=`cmp test.txt $TEST_FOLDER/nonzero.txt`
if [ "$TEST6" ]
then 
    echo "LE 2 [1 3 5... ] [ 0 ] [0 2 4 ... ] == [ 1 2 3 4 ... ], FAILED"
else
    echo "LE 2 [1 3 5... ] [ 0 ] [0 2 4 ... ] == [ 1 2 3 4 ... ], PASSED"
fi
rm test.txt

./scalc [ EQ 1 [ GR 0 [ LE 10 [ INT [ SUM [ EQ 3 $TEST_FOLDER/zero.txt [ SUM $TEST_FOLDER/empty.txt ] $TEST_FOLDER/empty.txt [ DIF $TEST_FOLDER/odds.txt $TEST_FOLDER/evens.txt ] $TEST_FOLDER/zero.txt ] ] ] ] ] ] > test.txt
TEST7=`cmp test.txt $TEST_FOLDER/zero.txt`
if [ "$TEST7" ]
then 
    echo "Deep tree test, FAILED"
else
    echo "Deep tree test, PASSED"
fi
rm test.txt
