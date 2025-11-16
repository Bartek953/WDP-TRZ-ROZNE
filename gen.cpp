#include <iostream>
#include <unistd.h>
#include <random>
using namespace std;

const int MAX_N = (1<<20);
const int MIN_N = 1;

const int MAX_ADD_DIST = 2000; //max dist: max_n * max_add
const int MIN_ADD_DIST = 0;

const int MAX_N_TYPES = 100;


int main(){
    srand(getpid());
    std::random_device rd;  
    std::mt19937 gen(rd()); 

    std::uniform_int_distribution<int> dist(0, (1 << 30));

    int n_wyk = 1+dist(gen)%20;
    int n = 1 + dist(gen) % (1<<n_wyk);


    //int n = MIN_N + dist(gen)%(MAX_N - MIN_N + 1);
    cout << n << '\n';

    int wyk = 2 + dist(gen)%20;
    int max_types = (1 << wyk);

    int min_add = dist(gen) % MAX_ADD_DIST;

    int n_types = 1 + dist(gen) % max_types;
    n_types = min(n_types, n);

    int last_dist = 0;

    bool check_overflow = dist(gen) % 2;

    for(int i = 0; i < n; i++){
        //if(rand() % 50 != 0){
            last_dist += min_add + dist(gen) % (MAX_ADD_DIST - min_add + 1);
        //}
        if(check_overflow && i > n - 10){
            last_dist = INT_MAX;
        }
        cout << (1 + dist(gen) % n_types) << ' ' << last_dist << '\n';
    }

}