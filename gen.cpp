#include <iostream>
#include <unistd.h>
#include <random>
using namespace std;

const int MAX_N = 20000;
const int MIN_N = 1;

const int MAX_ADD_DIST = (1 << 30) / MAX_N; //max dist: max_n * max_add
const int MIN_ADD_DIST = 0;

const int MAX_N_TYPES = 1000;


int main(){
    srand(getpid());

    int n = MIN_N + rand()%(MAX_N - MIN_N + 1);
    cout << n << '\n';

    int n_types = 1 + rand() % MAX_N_TYPES;
    n_types = min(n_types, n);

    int last_dist = 0;
    for(int i = 0; i < n; i++){
        if(rand() % 3 != 0){
            last_dist += MIN_ADD_DIST + rand() % (MAX_ADD_DIST - MIN_ADD_DIST + 1);
        }
        cout << (1 + rand() % n_types) << ' ' << last_dist << '\n';
    }

}