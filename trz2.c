#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

//time: O(n)
//memory: O(n)

//in comments I often use nomenclature "on the left", "on the right"
//left means hotels that have distance closer to 0
//right means hotel that have distance close to MAX_DISTANCE
//in case of few hotels in the same place, order does not matter

//0------------------>MAX_DISTANCE

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

//struct of motel
//type - site of hotel
//dist - distance
typedef struct hotel {
    int type;
    int dist;
}   hotel;

void hotel_print(hotel h){
    printf("(%d, %d)\n", h.type, h.dist);
}


//set3 struct functions as set of three hotels of different types, to which you can add element
//it stores three with lowest distance in mini[]
//and three with biggest distance in maxi[]
//if set has less than 3 elements, they will be initialized to {-1, -1} by constructor
typedef struct set3 {
    hotel mini[3];
    hotel maxi[3];
} set3;

//constructs set3
set3 set3_create(void){
    set3 result;
    for(int i = 0; i < 3; i++){
        result.mini[i] = (hotel){-1, -1};
        result.maxi[i] = (hotel){-1, -1};
    }
    return result;
}

void set_assert(set3* set){
    for(int i = 0; i <= 1; i++){
        assert(set->maxi[i].dist <= set->maxi[i + 1].dist);
        assert(set->mini[i].dist <= set->mini[i + 1].dist);

        assert(set->maxi[i].type != set->maxi[i + 1].type || set->maxi[i].type == -1);
        assert(set->mini[i].type != set->mini[i + 1].type || set->mini[i].type == -1);
    }
    assert(set->maxi[2].type != set->maxi[0].type || set->maxi[2].type == -1);
    assert(set->mini[2].type != set->mini[0].type || set->mini[2].type == -1);

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

//bubble sort
//Im using it only for n = 3, 
//so it does not affect time complexity
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

//inserts element into set
//checks if its smaller than some element in mini[]
//and if its bigger that some element in maxi[]
//if yes, replaces it (keeping different types in mini[] and maxi[])
//if no, nothing changes
void set3_insert(set3* set, hotel element){
    set_assert(set);
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

//returns two elements, with types different from curr_type
//writes them into el1, el2
//return elements with maximum distance in set if order = 1
//and elements with minimum distance in set if order = -1
//order should be in {-1, 1}
//if there are less than 2 elements in mini[]/maxi[], not existing elements will be set as {-1, -1}
void get_diff(set3* set, hotel* el1, hotel* el2, int curr_type, int order){
    set_assert(set);
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

//returns maximum of distance(h1, h2) and distance(h2, h3)
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
//returns minimum of distance(h1, h2) and distance(h2, h3)
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

//calculates minimum of maximumus between hotels h1, h2, h3 of different types
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
        //for each hotel h1, Im storing two hotels h2, h3 such that
        //h1, h2, h3 have different types
        //and h2, h3 have maximum possible distance on the left of h1
        //(they are the closest to h1 from the left side)
    }
    set3 set2 = set3_create();
    for(int i = n - 1; i >= 0; i--){
        set3_insert(&set2, road[i]);
        hotel mini1, mini2;
        get_diff(&set2, &mini1, &mini2, road[i].type, -1);
        //for each hotel Im finding closest hotels of different types on the right
        //I also stored the closest on the left
        //so the optimal result, taking i-th hotel as middle
        //will have maxi1/2[i] as left hotel and mini1/2 as right hotel

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
//calculates maximum of manimus between hotels h1, h2, h3 of different types
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
        //for each element h1 Im storing two hotels h2, h3 such that
        //h1, h2, h3 have different types
        //and h2, h3 have smallest distance possible
        set3_insert(&set1, road[i]);
        get_diff(&set1, &mini1[i], &mini2[i], road[i].type, -1);
    }
    set3 set2 = set3_create();
    for(int i = n - 1; i >= 0; i--){
        //for each element h1 Im storing two hotels h2, h3 such that
        //h1, h2, h3 have different types
        //and h2, h3 have biggest distance possible
        set3_insert(&set2, road[i]);
        hotel maxi1, maxi2;
        get_diff(&set2, &maxi1, &maxi2, road[i].type, 1);

        //Taking h1 as middle hotel, we can find optimal result for
        //left hotel as mini1/2
        //right hotel as maxi1/2
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
