#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct individual{
   unsigned long long chromosome;
   float fitness;
};

unsigned long long mutate(unsigned long long c);
void generateLocalMoves(struct individual climber, struct individual localMoves[]);
void merge(struct individual population[], int l, int m, int r);
void mergeSortPopulation(struct individual population[], int l, int r);
void evaluate(struct individual population[]);
float PlayTFT(int history, unsigned long long strategy, int numberOfIterations);
float PlayRAND(int history, unsigned long long strategy, int numberOfIterations);
struct individual generateIndividual();

#define MUTATEINTENSITY 10
#define ITERATIONS 10000

int main(const int argc, const char ** argv){

int k = 0;

struct individual P = generateIndividual(); //initial person
struct individual L[64];
generateLocalMoves(P, L);//Local moves
evaluate(L);
mergeSortPopulation(L, 0, 63);
float max =  L[63].fitness;
unsigned long long maxC = L[63].chromosome;
P.chromosome = L[63].chromosome;
P.fitness = L[63].fitness;

printf("BEFORE: %f\t%llx\n", L[63].fitness, L[63].chromosome);

while(k < ITERATIONS){
    generateLocalMoves(P, L);
    evaluate(L);
    mergeSortPopulation(L, 0, 63);
    if(L[63].fitness > P.fitness){
        P.chromosome = L[63].chromosome;
        P.fitness = L[63].fitness;
    }else{
        P.chromosome = mutate(P.chromosome);
        P.fitness = 0;
    }
    if(P.fitness > max){
        max = P.fitness;
        maxC = P.chromosome;
    }

    k ++;
}


printf("\nAFTER: %f\t%llx\n", P.fitness, P.chromosome);

return 0;
}

unsigned long long mutate(unsigned long long c){
    srand(time(NULL));
    unsigned long long a = 0.0;
    int j;
    for(j = 0; j < MUTATEINTENSITY; j++){
        a = pow(2, rand() % 64);
        c = c ^ a;
    }
    return c;
}

void merge(struct individual P[], int l, int m, int r){

    int i, j, k;
    int a = m - l + 1;
    int b =  r - m;
    struct individual L[a], R[b];

    for (i = 0; i < a; i++){ // copy left sice into temp array
        L[i].chromosome = P[l + i].chromosome;
        L[i].fitness = P[l + i].fitness;
    }
    for (j = 0; j < b; j++){ //copy right side into temp array
        R[j].chromosome = P[m + 1+ j].chromosome;
        R[j].fitness = P[m + 1+ j].fitness;
    }


    i = 0;
    j = 0;
    k = l;
    while (i < a && j < b)
    {
        if (L[i].fitness <= R[j].fitness)
        {
            P[k].chromosome = L[i].chromosome;
            P[k].fitness = L[i].fitness;
            i++;
        }
        else
        {
            P[k].chromosome = R[j].chromosome;
            P[k].fitness = R[j].fitness;
            j++;
        }
        k++;
    }

    while (i < a)//copy remaining elements in L into P
    {
        P[k].chromosome = L[i].chromosome;
        P[k].fitness = L[i].fitness;
        i++;
        k++;
    }
    while (j < b)//copy remaining elements in R into P
    {
        P[k].chromosome = R[j].chromosome;
        P[k].fitness = R[j].fitness;
        j++;
        k++;
    }
}

void mergeSortPopulation(struct individual L[], int l, int r){
    if(l<r){
        int m = (l+r)/2;
        mergeSortPopulation(L, l, m);
        mergeSortPopulation(L, m+1, r);

        merge(L, l, m, r);
    }
}

struct individual generateIndividual(){
    int i;
    struct individual P;
    srand(time(NULL));
    unsigned long long int S = 0;
    S = rand();
    S = S << 15;
    S += rand();
    S = S << 15;
    S += rand();
    S = S << 15;
    S += rand();
    S = S << 4;
    S += (rand() % 16);
    P.chromosome = S;                //P[i].chromosome = 0 to 18446744073709551615
    P.fitness = 0;                   //initialize fitness to 0
    return P;
}


void generateLocalMoves(struct individual C, struct individual L[]){
    unsigned long long a, c;
    c = C.chromosome;
    for(int i = 0; i < 64; i++){
        a = pow(2, i);
        L[i].chromosome = c ^ a;
        L[i].fitness = 0;
    }
    return;
}

void evaluate(struct individual L[]){
    unsigned int h;
    float scoresSum = 0.0;
    unsigned long long int S;

    int firstmoves[8] = {1, 2, 9, 11, 36, 38, 45, 47};

    for(int i = 0; i < 64; i++){
        S = L[i].chromosome;
        for(h = 0; h < 8; h++){
            scoresSum += PlayTFT(firstmoves[h], S, 100);
        }
        //for(h = 0; h < 64; h++){
        //    scoresSum += PlayRAND(firstmoves[h], S, 100);
        //}
        L[i].fitness = (float)(scoresSum/8);
        scoresSum = 0.0;
    }
    return;
}

float PlayTFT(int h, unsigned long long S1, int ni){
    int i = 0;
    int TFTmove = 0;
    int S1score = 0;
    int S1move = (S1 >> (long long unsigned int)(64 - h - 1)) & 0x0000000000000001;


    TFTmove = h & 0x0002;
    TFTmove = TFTmove >> 1;
    while( i < ni){
        h = h & 0x000F;
        h = h << 2;

        if(S1move == 0 && TFTmove == 0){            // 0 = cooperate, 1 = defect
            S1score += 3;
            TFTmove = 0;
        }else if(S1move == 0 && TFTmove == 1){
            S1score += 0;
            TFTmove = 0;
            h += 1;
        }else if(S1move == 1 && TFTmove == 0){
            S1score += 5;
            TFTmove = 1;
            h += 2;
        }else if(S1move == 1 && TFTmove == 1){
            S1score += 1;
            TFTmove = 1;
            h += 3;
        }
        S1move = (S1 >> (long long unsigned int)(64 - h - 1)) & 0x0000000000000001;
        i++;
    }
    return (float)S1score/(float)ni;
}

float PlayRAND(int h, unsigned long long S1, int ni){
    int i = 0;
    srand(time(NULL));
    int RANDmove = rand() % 2;
    int S1score = 0;
    int S1move = (S1 >> (long long unsigned int)(64 - h - 1)) & 0x0000000000000001;

    while( i < ni){
        RANDmove = rand() % 2;
        h = h & 0x000F;
        h = h << 2;

        if(S1move == 0 && RANDmove == 0){            // 0 = cooperate, 1 = defect
            S1score += 3;
            RANDmove = 0;
        }else if(S1move == 0 && RANDmove == 1){
            S1score += 0;
            h += 1;
        }else if(S1move == 1 && RANDmove == 0){
            S1score += 5;
            h += 2;
        }else if(S1move == 1 && RANDmove == 1){
            S1score += 1;
            h += 3;
        }
        S1move = (S1 >> (long long unsigned int)(64 - h - 1)) & 0x0000000000000001;
        i++;
    }
    return (float)S1score/(float)ni;
}
