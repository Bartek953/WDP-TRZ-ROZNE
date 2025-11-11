gcc @opcje_mac trz2.c -o wzo
g++ -std=c++20 brute_n2.cpp -o bruteN2
g++ -std=c++20 gen.cpp -o gen

ile=4000
dir=testy/big

echo "$dir $ile"

for((i=3000;i<ile;i++))
do
    echo "test $i"
    ./gen > trash/t

    MallocNanoZone=0 ./wzo < trash/t > trash/t.wzo

    ./bruteN2 < trash/t > trash/t.bn2

    cat trash/t.wzo
    cat trash/t.bn2

    diff -b trash/t.wzo trash/t.bn2 || break
    echo "OK"
    echo ""

    #cp trash/t $dir/$i.in
    #cp trash/t.wzo $dir/$i.out
done