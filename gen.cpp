#include <iostream>
#include <unistd.h>
#include <random>
using namespace std;

const int MAX_N = 1000;
const int MIN_N = 1;

const int MAX_ADD_DIST = 10000;
const int MIN_ADD_DIST = 0;


int main(){
    srand(getpid());

    int n = MIN_N + rand()%(MAX_N - MIN_N + 1);
    cout << n << '\n';

    int last_dist = 0;
    for(int i = 0; i < n; i++){
        last_dist += MIN_ADD_DIST + rand() % (MAX_ADD_DIST - MIN_ADD_DIST + 1);
        cout << (1 + rand() % n) << ' ' << last_dist << '\n';
    }

}