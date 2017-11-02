#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

int circle_count = 0;
int total_points = 100000;

double random_double();
void incCircleCount();

int main( int argc, char *argv[]){
	double est_pi, x, y;

	#pragma omp parallel for //divid work contained in for loop among multiple threads to run in parallel
	for(int i = 0; i < total_points; i++){
		srandom((unsigned)time(NULL)); //seed random number generator
		//obtain a  random (x,y) point  between (-1,-1) and (1,1)
		x = random_double() * 2.0 -1.0;
		y = random_double() * 2.0 -1.0;
		if( sqrt(x*x + y*y) < 1.0){	//test for hit

			incCircleCount();
		}
	}

	est_pi = (4 * ((double)circle_count/(double)total_points));//estimate pi
	printf("%f\n", est_pi);

return 0;
}

double random_double(){
	return (random() / ((double)RAND_MAX +1));
}

void incCircleCount(){
	#pragma omp citical //protects against race condiditon
	{
	++circle_count;
	}
}
