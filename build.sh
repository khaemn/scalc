mkdir build
cd build
cmake ../
make -j
./scalc l [ SUM [ DIF ../test/a.txt ../test/b.txt ../test/c.txt ] [ INT ../test/b.txt ../test/c.txt ] ]
