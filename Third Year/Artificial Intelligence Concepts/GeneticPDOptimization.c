#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

struct individual{
   unsigned long long chromosome;
   float fitness;
};

void select(struct individual population[], struct individual offspring[]);
void mutate(struct individual offspring[]);
struct individual mate(struct individual, struct individual);
void merge(struct individual population[], int l, int m, int r);
void mergeSortPopulation(struct individual population[], int l, int r);
void recombine(struct individual population[], struct individual offspring[]);
void evaluate(struct individual population[], int size);
float PlayTFT(int history, unsigned long long strategy, int numberOfIterations);
float PlayRAND(int history, unsigned long long strategy, int numberOfIterations);
void generatePopulation(struct individual populations[]);

#define POPULATIONSIZE 500
#define OFFSPRINGSIZE 499
#define ITERATIONS 1000000
#define MUTATECONTROL  13276//if the random number generated on line 61 is less then this number, a mutation occurs. 100% probability = 32,767
#define MUTATEINTENSITY 1//the number of bits changed

int main(const int argc, const char ** argv){

int k = 0;

struct individual P[POPULATIONSIZE]; //initial population
struct individual O[OFFSPRINGSIZE];//offspring

generatePopulation(P);
evaluate(P, POPULATIONSIZE);
mergeSortPopulation(P, 0, POPULATIONSIZE-1);

printf("BEFORE:\nFitness:\tHex Chromosome:\n");
for(k = POPULATIONSIZE-10; k < POPULATIONSIZE; k++){
    printf("%f\t%llx\n", P[k].fitness, P[k].chromosome);
}

srand(time(NULL));
while(k < ITERATIONS){
    recombine(P, O);
    mutate(O);
    evaluate(O, OFFSPRINGSIZE);
    mergeSortPopulation(O, 0, OFFSPRINGSIZE-1);
    select(P, O);
    k ++;
    printf("\rIterations Completed: %d\tBest: %f\t%llx", k, P[49].fitness, P[49].chromosome);
}
mergeSortPopulation(P, 0, POPULATIONSIZE-1);
printf("\n-------------------------------------\nAFTER:\nFitness:\tHex Chromosome:\n");
float fitavg = 0;
for(k = POPULATIONSIZE-50; k < POPULATIONSIZE; k+=10){
    printf("%d) %f\t%llx\n", k, P[k].fitness, P[k].chromosome);
}
for(k = POPULATIONSIZE-10; k < POPULATIONSIZE; k++){
    printf("\t%d) %f\t%llx\n", k, P[k].fitness, P[k].chromosome);
}
for(k = 0; k < POPULATIONSIZE; k++){
    fitavg+=P[k].fitness;
}
    printf("\n\tAVG = %f", fitavg/(float)POPULATIONSIZE);

return 0;
}

void select(struct individual P[], struct individual O[]){

    int i, j, k;
    struct individual T[POPULATIONSIZE];
    for(i = 0; i < POPULATIONSIZE; i ++){
        T[i].chromosome = P[i].chromosome;
        T[i].fitness = P[i].fitness;
    }
    i = POPULATIONSIZE-1;
    j = OFFSPRINGSIZE-1;
    k = POPULATIONSIZE-1;

    while (i >= 0 && j >= 0 && k >= 0)
    {
        if (T[i].fitness > O[j].fitness)
        {
            P[k].chromosome = T[i].chromosome;
            P[k].fitness = T[i].fitness;
            i--;
        }else{
            P[k].chromosome = O[j].chromosome;
            P[k].fitness = O[j].fitness;
            j--;
        }
        k--;
    }

    while (k >= 0)//copy remaining elements in T into P
    {
        P[k].chromosome = T[i].chromosome;
        P[k].fitness = T[i].fitness;
        i--;
        k--;
    }

    return;
}

void mutate(struct individual O[]){
    unsigned long long a, c;
    int i, j;
    for(i = 0; i < (POPULATIONSIZE - 1); i++){
        a = rand();
        if(a < MUTATECONTROL){
            for(j = 0; j < MUTATEINTENSITY; j++){
                a = pow(2, rand() % 64);
                c = O[i].chromosome;
                c = c ^ a;
                O[i].chromosome = c;
            }
        }
    }
    return;
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

void mergeSortPopulation(struct individual P[], int l, int r){
    if(l<r){
        int m = (l+r)/2;
        mergeSortPopulation(P, l, m);
        mergeSortPopulation(P, m+1, r);

        merge(P, l, m, r);
    }
}

struct individual mate(struct individual P1, struct individual P2){
    int SP = (rand() % 64) + 1; //splitpint
    struct individual C; // child
    unsigned long long P1c = P1.chromosome;
    unsigned long long P2c = P2.chromosome;
    unsigned long long Cc = 0;

    P1c = P1c >> (64 - SP); //set non-propagated bits in parent one and parent two to 0
    P1c = P1c << (64 - SP);
    P2c = P2c << SP;
    P2c = P2c >> SP;

    Cc += P1c;  //copy bits into child chromosome
    Cc += P2c;
    C.chromosome = Cc;
    C.fitness = 0;

    return C;
}

void recombine(struct individual P[], struct individual O[]){
    mergeSortPopulation(P, 0, POPULATIONSIZE-1);
    struct individual child;
    for(int i = 0; i < (OFFSPRINGSIZE); i++){
        O[i] = mate(P[i], P[i+1]);
    }
return;
}

void generatePopulation(struct individual P[]){
    int i;
    unsigned long long int S = 0;
    for(i = 0; i < POPULATIONSIZE; i++){
        S = rand();
        S = S << 15;
        S += rand();
        S = S << 15;
        S += rand();
        S = S << 15;
        S += rand();
        S = S << 4;
        S += (rand() % 16);
        P[i].chromosome = S;                //P[i].chromosome = 0 to 18446744073709551615
        P[i].fitness = 0;                   //initialize fitness to 0
    }
    return;
}


void evaluate(struct individual P[], int size){
    unsigned int h;
    float scoresSum = 0.0;
    unsigned long long int S;

    int firstmoves[8] = {0, 2, 9, 11, 36, 38, 45, 47};

    for(int i = 0; i < size; i++){
        S = P[i].chromosome;
        //for(h = 0; h < 8; h++){
        //    scoresSum += PlayTFT(firstmoves[h], S, 100);
        //}
        //P[i].fitness = (float)(scoresSum/8);
        for(h = 0; h < 64; h++){
            scoresSum += PlayRAND(firstmoves[h], S, 100);
        }
        P[i].fitness = (float)(scoresSum/64);
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
