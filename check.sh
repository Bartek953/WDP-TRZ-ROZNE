gcc @opcje_mac trz2.c -o wzo
g++ -std=c++20 brute.cpp -o brute
g++ -std=c++20 gen.cpp -o gen

for((i=0;i<10000;i++))
do
    echo "test $i"
    ./gen > t
    ./wzo < t > t.wzo
    ./brute < t > t.brute
    diff -b t.wzo t.brute || break
    echo "OK"
    echo ""
done