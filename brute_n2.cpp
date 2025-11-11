#include <iostream>
#include <stdlib.h>
using namespace std;

const int nlim = (1<<20);
int type[nlim];
int dist[nlim];
int n;

int min3(int i1, int i2, int i3){
    return min(dist[i2] - dist[i1], dist[i3] - dist[i2]);
}
int max3(int i1, int i2, int i3){
    return max(dist[i2] - dist[i1], dist[i3] - dist[i2]);
}
bool diff_types(int i1, int i2, int i3){
    if(type[i1] != type[i2] && type[i2] != type[i3] && type[i3] != type[i1]){
        return true;
    }
    return false;
}

int get_min(int l){
    int result = -1;
    int mid, r;
    mid = r = l;

    while(r+1 < n){
        r++;
        while(type[mid] == type[l] && mid <= r){
            mid++;
        }
        if(diff_types(l, mid, r)){
            //cout << l << ' ' << mid << ' ' << r << '\n';
            //cout << max3(l, mid, r) << '\n';

            if(result == -1){
                result = max3(l, mid, r);
            }
            else result = min(result, max3(l, mid, r));

            while(dist[mid+1] - dist[l] <= dist[r] - dist[mid+1] && mid+1<n && mid+1<=r){
                mid++;
                if(diff_types(l, mid, r)){
                    if(result == -1){
                        result = max3(l, mid, r);
                    }
                    else result = min(result, max3(l, mid, r));
                }
            }
            if(mid+1<=r){
                if(diff_types(l, mid+1, r)){
                    if(result == -1){
                        result = max3(l, mid+1, r);
                    }
                    else result = min(result, max3(l, mid+1, r));
                }
            }
        }
    }
    return result;
}

int get_max(int l){
    int result = -1;
    int r1 = -1;
    int r2 = -1;
    for(int i = n-1; i >=l; i--){
        if(type[i] != type[l]){
            if(r1 == -1){
                r1 = i;
            }
            else if(r2 == -1 && type[i] != type[r1]){
                r2 = i;
            }
        }
    }
    //cout << "\nR " << r1 << ' ' << r2 << '\n';
    if(r1 != -1){
        for(int mid = l; mid <= r1; mid++){
            if(diff_types(l, mid, r1)){
                //cout << l << ' ' << mid << ' ' << r1 << '\n';
                //cout << min3(l, mid, r1) << '\n' << '\n';
                if(result == -1)
                    result = min3(l, mid, r1);
                else
                    result = max(result, min3(l, mid, r1));
            }
        }
    }
    if(r2 != -1){
        for(int mid = l; mid <= r2; mid++){
            if(diff_types(l, mid, r2)){
                //cout << l << ' ' << mid << ' ' << r2 << '\n';
                //cout << min3(l, mid, r2) << '\n';
                if(result == -1)
                    result = min3(l, mid, r2);
                else
                    result = max(result, min3(l, mid, r2));
            }
        }
    }

    return result;
}


int main(){
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n;
    for(int i = 0; i < n; i++){
        cin >> type[i];
        cin >> dist[i];
    }
    int mini = -1;
    for(int i = 0; i < n; i++){
        int curr_res = get_min(i);
        if(curr_res == -1){
            continue;
        }
        if(mini == -1){
            mini = curr_res;
        }
        else {
            mini = min(mini, curr_res);
        }
    }
    if(mini == -1){
        mini = 0;
    }

    int maxi = -1;
    for(int i = 0; i < n; i++){
        int curr_res = get_max(i);
        if(curr_res == -1){
            continue;
        }
        maxi = max(maxi, curr_res);
    }
    if(maxi == -1){
        maxi = 0;
    }
    //int maxi = get_max(0);


    cout << mini << ' ' << maxi << '\n';

    return 0;
}
