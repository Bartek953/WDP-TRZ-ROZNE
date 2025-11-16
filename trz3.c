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

//because we're dealing with similar functions for maximum and minimum
//I will be replacing them with functions taking parameter "order"
//order = 1 means we're dealing with maximum
//and order = -1 means we're dealing with minimum


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
int minmax(int x, int y, int order){
    if(order == 1){
        return max(x, y);
    }
    return min(x, y);
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

hotel* create_tab(int size){
    hotel* result = (hotel*) malloc((size_t) size * sizeof(hotel));
    if(!result){
        printf("[MALLOC ERROR] error while creating table for size = %d\n", size);
        exit(-1);
    }
    return result;
}

//set3 struct functions as set of three hotels of different types, to which you can add element
//it stores three with lowest distance in mini[]
//and three with biggest distance in maxi[]
//if set has less than 3 elements, they will be initialized to {-1, -1} by constructor
//elements in mini are decreasing
//and elements in maxi are increasing (in terms of distance)
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

//just checks if all properties of set3 are preserved
void set_assert(set3* set){
    for(int i = 0; i <= 1; i++){
        assert(set->maxi[i].dist <= set->maxi[i + 1].dist);
        assert(set->mini[i].dist >= set->mini[i + 1].dist);

        assert(set->maxi[i].type != set->maxi[i + 1].type || set->maxi[i].type == -1);
        assert(set->mini[i].type != set->mini[i + 1].type || set->mini[i].type == -1);
    }
    assert(set->maxi[2].type != set->maxi[0].type || set->maxi[2].type == -1);
    assert(set->mini[2].type != set->mini[0].type || set->mini[2].type == -1);
}

//prints set->maxi if order = 1
//and set->mini otherwise
void set3_print(set3* set, int order){
    hotel* tab = (order == 1 ? set->maxi : set->mini);
    for(int i = 0; i < 3; i++){
        hotel_print(tab[i]);
    }
    printf("\n");
}

//bubble sort
//Im using it only for n = 3, 
//so it does not affect time complexity
void sort(hotel* tab, int n, int order){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - 1; j++){
            if(tab[j].dist * order > tab[j + 1].dist * order){
                hotel h = tab[j];
                tab[j] = tab[j + 1];
                tab[j + 1] = h;
            }
        }
    }
}

//is h2 better than h1 in given order
//(in terms: would we like replace h1 with h2 in set->mini/maxi)
bool better(hotel h1, hotel h2, int order){
    return h1.type == -1 || h1.dist * order <= h2.dist * order;
}

//inserts elements into set3
//order specifies whetever we're inserting element into maxi[] or mini[]
void set3_insert(set3* set, hotel element, int order){
    set_assert(set);

    int to_replace_ind = 0;
    hotel* tab;

    //if element.type exists in set3, we want to upade it
    //otherwise, we would like to replace some {-1, -1} from set3
    //which will be at 0 index in maxi[] (increasing order) or 2 in mini[] (decreasing order)
    //otherwise we will be replacing index 0 (in maxi[] it's smallest distance)
    //(and in mini[] its biggest distance)
    if(order == 1){
        tab = set->maxi;
    }
    else {
        tab = set->mini;
        if(tab[2].type == -1){
            to_replace_ind = 2;
        }
    }
    for(int i = 0; i < 3; i++){
        if(tab[i].type == element.type){
            to_replace_ind = i;
        }
    }
    if(better(tab[to_replace_ind], element, order)){
        tab[to_replace_ind] = element;
    }
    //we need to sort the array which we changed, to preserve properties of set
    //that array has 3 elements, so it's O(1)
    sort(tab, 3, order);
}
//this function returns to greatest elements (which types different from current type)
//in given order from set3
//and writes them into variables h1 and h2
void get_diff(set3* set, hotel* h1, hotel* h2, int curr_type, int order){
    set_assert(set);

    int how_many_set = 0;
    hotel* tab;

    if(order == 1){
        tab = set->maxi;
    }
    else {
        tab = set->mini;
    }
    //distances in maxi[] are increasing and in mini[] are decreasing
    //so in terms of specified order both arrays are increasing
    //(we want biggest elements from maxi[] and smallest from mini[])
    //therefore we want to take last 2 elements possible to take
    //(not equal to {-1, -1} and with types different from curr_type)
    //you can notice, that elements in set3 have different types,
    //so we dont have to worry about h1 and h2 having the same types
    for(int i = 2; i >= 0; i--){
        if(tab[i].type != -1 && tab[i].type != curr_type){
            if(how_many_set == 0){
                (*h1) = tab[i];
                how_many_set++;
            }
            else if(how_many_set == 1){
                (*h2) = tab[i];
                how_many_set++;
            }
        }
    }
    if(how_many_set <= 1){
        (*h2) = (hotel){-1, -1};
    }
    if(how_many_set == 0){
        (*h1) = (hotel){-1, -1};
    }
}

//this functions improve result with distances between hotels h1, h2, h3
//in terms of given order
void improve_res(hotel h1, hotel h2, hotel h3, int* result, int order){
    if(h1.type == -1 || h3.type == -1 || h1.type == h3.type){
        return;
    }
    assert(h1.dist <= h2.dist);
    assert(h2.dist <= h3.dist);

    int curr_res = minmax(h2.dist - h1.dist, h3.dist - h2.dist, -order);
    if((*result) == -1){
        (*result) = curr_res;
    }
    else {
        (*result) = minmax((*result), curr_res, order);
    }
}

//this function find biggest/smallest distance between three hotels of different types
//depending on given order
int get_result(hotel* road, int n, int order){
    int result = -1;

    hotel* left1 = create_tab(n);
    hotel* left2 = create_tab(n);
    //if order = -1, then left1[i], left2[i] are hotels of different types on the left closest to i-th hotel
    //if order = 1, then they are hotels on left farthest from i-th hotel

    set3 left_set = set3_create();
    for(int i = 0; i < n; i++){
        set3_insert(&left_set, road[i], -order);
        get_diff(&left_set, &left1[i], &left2[i], road[i].type, -order);
    }

    set3 right_set = set3_create();
    hotel right1, right2;
    //if order = -1, then right1, right2 are hotels of different types on the right closest to i-th hotel
    //if order = 1, then they are hotels on right farthest from i-th hotel
    for(int i = n - 1; i >= 0; i--){
        set3_insert(&right_set, road[i], order);
        get_diff(&right_set, &right1, &right2, road[i].type, order);

        //taking road[i] as middle hotel, optimal left hotel must be left1 or left2
        //and optimal right hotel must be right1 or right2
        //(checking only one hotel on each side isn't enough, because they could have the same types)
        //therefore we're checking all possible combination of left1, left2, right1, right2
        improve_res(left1[i], road[i], right1, &result, order);
        improve_res(left2[i], road[i], right1, &result, order);
        improve_res(left1[i], road[i], right2, &result, order);
        improve_res(left2[i], road[i], right2, &result, order);
    }

    if(result == -1){
        result = 0;
    }
    free(left1);
    free(left2);
    return result;
}

int main(void){
    int n;
    scanf("%d", &n);

    hotel* road = create_tab(n);
    for(int i = 0; i < n; i++){
        scanf("%d %d", &road[i].type, &road[i].dist);
    }
    
    printf("%d %d\n", get_result(road, n, -1), get_result(road, n, 1));

    free(road);
    return 0;
}
