#include <iostream>
using namespace std;

typedef struct hotel {
    int type;
    int distance;
} hotel;


int main(){
    int n;
    cin >> n;

    hotel* road = new hotel[n];

    for(int i = 0; i < n; i++){
        cin >> road[i].type >> road[i].distance;
    }

    int mini, maxi;
    mini = maxi = -1;

    for(int i = 0; i < n; i++){
        for(int j = i + 1; j < n; j++){
            for(int k = j + 1; k < n; k++){
                if(road[i].type != road[j].type && road[j].type != road[k].type && road[k].type != road[i].type){
                    int d1 = road[j].distance - road[i].distance;
                    int d2 = road[k].distance - road[j].distance;
                    if(mini == -1){
                        mini = max(d1, d2);
                        maxi = min(d1, d2);
                    }
                    else {
                        mini = min(mini, max(d1, d2));
                        maxi = max(maxi, min(d1, d2));
                    }
                }
            }
        }
    }
    if(mini == -1){
        cout << "0 0\n";
    }
    else {
        cout << mini << ' ' << maxi << '\n';
    }
    delete [] road;
    return 0;
}