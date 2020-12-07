./build.sh

cd build

TEST_FOLDER="../test"

./scalc [ DIF $TEST_FOLDER/nonzero.txt $TEST_FOLDER/naturals.txt ] > nonzero_dif_naturals.txt
TEST=`cmp nonzero_dif_naturals.txt $TEST_FOLDER/zero.txt`
if [ "$TEST" ]
then 
    echo "DIF [1 2 3 ... ] [0 1 2 ... ] == 0, FAILED"
else
    echo "DIF [1 2 3 ... ] [0 1 2 ... ] == 0, PASSED"
fi
rm nonzero_dif_naturals.txt

./scalc [ SUM $TEST_FOLDER/odds.txt $TEST_FOLDER/evens.txt ] > odds_sum_evens.txt
TEST=`cmp odds_sum_evens.txt $TEST_FOLDER/naturals.txt`
if [ "$TEST" ]
then 
    echo "SUM [1 3 5 ... ] [0 2 4 ... ] == [1 2 3 4 ... ], FAILED"
else
    echo "SUM [1 3 5 ... ] [0 2 4 ... ] == [1 2 3 4 ... ], PASSED"
fi
rm odds_sum_evens.txt

./scalc [ DIF $TEST_FOLDER/nonzero.txt $TEST_FOLDER/nonzero.txt $TEST_FOLDER/nonzero.txt $TEST_FOLDER/zero.txt ] > nonzero_dif_zero.txt
TEST=`cmp nonzero_dif_zero.txt $TEST_FOLDER/zero.txt`
if [ "$TEST" ]
then 
    echo "DIF [1 2 3 ... ] [1 2 3 ... ] [ 0 ] == [ 0 ], FAILED"
else
    echo "DIF [1 2 3 ... ] [1 2 3 ... ] [ 0 ] == [ 0 ], PASSED"
fi
rm nonzero_dif_zero.txt
