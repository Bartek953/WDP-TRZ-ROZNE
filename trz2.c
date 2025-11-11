#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

int max(int x, int y){
    if(x > y)
        return x;
    return y;
}
int min(int x, int y){
    if(x < y)
        return x;
    return y;
}
int abs(int x){
    return max(x, -x);
}

typedef struct hotel {
    int type;
    int dist;
}   hotel;

void hotel_print(hotel h){
    printf("(%d, %d)\n", h.type, h.dist);
}

typedef struct set3 {
    hotel mini[3];
    hotel maxi[3];
} set3;

set3 set3_create(void){
    set3 result;
    for(int i = 0; i < 3; i++){
        result.mini[i] = (hotel){-1, -1};
        result.maxi[i] = (hotel){-1, -1};
    }
    return result;
}

void set3_print(set3* set){
    for(int i = 0; i < 3; i++){
        hotel_print(set->mini[i]);
    }
    for(int i = 0; i < 3; i++){
        hotel_print(set->maxi[i]);
    }
    printf("\n");
}

void sort(hotel* tab, int n){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - 1; j++){
            if(tab[j].dist > tab[j + 1].dist){
                hotel h = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = h;
            }
        }
    }
}

void set3_insert(set3* set, hotel element){
    int ind_min, ind_max;

    if(set->mini[0].type == -1){
        ind_min = 0;
    }
    else {
        ind_min = 2;
    }
    ind_max = 0;

    for(int i = 0; i < 3; i++){
        if(set->mini[i].type == element.type){
            ind_min = i;
        }
        if(set->maxi[i].type == element.type){
            ind_max = i;
        }
    }
    if(set->mini[ind_min].type == -1 || set->mini[ind_min].dist >= element.dist){
        set->mini[ind_min] = element;
    }
    if(set->maxi[ind_max].type == -1 || set->maxi[ind_max].dist <= element.dist){
        set->maxi[ind_max] = element;
    }
    sort(set->mini, 3);
    sort(set->maxi, 3);
}

void get_diff(set3* set, hotel* el1, hotel* el2, int curr_type, int order){
    if(order == 1){
        int ind = 2;
        for(int i = 2; i >= 0; i--){
            if(set->maxi[i].type != -1 && set->maxi[i].type != curr_type){
                if(ind == 2){
                    (*el2) = set->maxi[i];
                    ind--;
                }
                else if(ind == 1){
                    (*el1) = set->maxi[i];
                    ind--;
                }
            }
        }
        if(ind >= 1){
            (*el1) = (hotel){-1, -1};
        }
        if(ind == 2){
            (*el2) = (hotel){-1, -1};
        }

    }
    else {
        int ind = 1;
        for(int i = 0; i < 3; i++){
            if(set->mini[i].type != -1 && set->mini[i].type != curr_type){
                if(ind == 1){
                    (*el1) = set->mini[i];
                    ind++;
                }
                else if(ind == 2){
                    (*el2) = set->mini[i];
                    ind++;
                }
            }
        }
        if(ind <= 2){
            (*el2) = (hotel){-1, -1};
        }
        if(ind == 1){
            (*el1) = (hotel){-1, -1};
        }
    }
}

void maximize_res(hotel h1, hotel h2, hotel h3, int* result){
    if(h1.type == -1 || h3.type == -1 || h1.type == h3.type){
        return;
    }
    assert(h1.dist <= h2.dist);
    assert(h2.dist <= h3.dist);

    int curr_wyn = min(h2.dist - h1.dist, h3.dist - h2.dist);
    if((*result) == -1){
        (*result) = curr_wyn;
    }
    else {
        (*result) = max((*result), curr_wyn);
    }
}
void minimize_res(hotel h1, hotel h2, hotel h3, int* result){
    if(h1.type == -1 || h3.type == -1 || h1.type == h3.type){
        return;
    }
    assert(h1.dist <= h2.dist);
    assert(h2.dist <= h3.dist);

    int curr_wyn = max(h2.dist - h1.dist, h3.dist - h2.dist);
    if((*result) == -1){
        (*result) = curr_wyn;
    }
    else {
        (*result) = min((*result), curr_wyn);
    }
}

int get_min(hotel* road, int n){
    int result = -1;

    hotel* maxi1 = (hotel*) malloc((size_t)n * sizeof(hotel));
    hotel* maxi2 = (hotel*) malloc((size_t)n * sizeof(hotel));
    if(!maxi1 || !maxi2){
        printf("[MALLOC ERROR]");
        exit(-1);
    }

    set3 set1 = set3_create();
    for(int i = 0; i < n; i++){
        set3_insert(&set1, road[i]);
        get_diff(&set1, &maxi1[i], &maxi2[i], road[i].type, 1);
    }
    set3 set2 = set3_create();
    for(int i = n - 1; i >= 0; i--){
        set3_insert(&set2, road[i]);
        hotel mini1, mini2;
        get_diff(&set2, &mini1, &mini2, road[i].type, -1);

        minimize_res(maxi1[i], road[i], mini1, &result);
        minimize_res(maxi2[i], road[i], mini1, &result);
        minimize_res(maxi1[i], road[i], mini2, &result);
        minimize_res(maxi2[i], road[i], mini2, &result);
    }


    free(maxi1);
    free(maxi2);
    if(result == -1){
        return 0;
    }
    return result;
}

int get_max(hotel* road, int n){
    int result = -1;
    
    hotel* mini1 = (hotel*) malloc((size_t)n * sizeof(hotel));
    hotel* mini2 = (hotel*) malloc((size_t)n * sizeof(hotel));
    if(!mini1 || !mini2){
        printf("[MALLOC ERROR]\n");
        exit(-1);
    }

    set3 set1 = set3_create();
    for(int i = 0; i < n; i++){
        set3_insert(&set1, road[i]);
        get_diff(&set1, &mini1[i], &mini2[i], road[i].type, -1);
        
        //printf("\n%d:\n", i);
        //hotel_print(mini1[i]);
        //hotel_print(mini2[i]);
        //set3_print(&set1);
    }
    set3 set2 = set3_create();
    for(int i = n - 1; i >= 0; i--){
        set3_insert(&set2, road[i]);
        hotel maxi1, maxi2;
        get_diff(&set2, &maxi1, &maxi2, road[i].type, 1);

        maximize_res(mini1[i], road[i], maxi1, &result);
        maximize_res(mini2[i], road[i], maxi1, &result);
        maximize_res(mini1[i], road[i], maxi2, &result);
        maximize_res(mini2[i], road[i], maxi2, &result);
    }

    free(mini1);
    free(mini2);
    if(result == -1){
        return 0;
    }
    return result;
}

int main(void){
    int n;
    scanf("%d", &n);

    hotel* road = (hotel*) malloc((size_t)n * sizeof(hotel));
    if(!road){
        printf("[MALLOC ERROR]\n");
        exit(-1);
    }
    for(int i = 0; i < n; i++){
        scanf("%d %d", &road[i].type, &road[i].dist);
    }

    printf("%d %d\n", get_min(road, n), get_max(road, n));
    free(road);
    return 0;
}
