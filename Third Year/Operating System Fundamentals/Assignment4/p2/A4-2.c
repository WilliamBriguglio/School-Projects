#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N_ADDR 1000000 // number of randomly generated addresses

unsigned long getPage(unsigned long );
unsigned long getOffset(unsigned long);
void popArray(unsigned long *);

int main(){

unsigned long page, offset, addr[N_ADDR];
popArray(addr);

for( int i = 0; i < N_ADDR; i++){
	page = getPage(addr[i]);
	offset = getOffset(addr[i]);
}

return 0;
}

void popArray(unsigned long * addr){
//populate array with random numbers between 0 and 4,294,967,295
srandom((unsigned)time(NULL));
	for( int i = 0; i < N_ADDR; i++){
		addr[i] = (unsigned)random() % 4294967295u + 0;
	}
}

unsigned long getOffset(unsigned long  addr){
//select only the 12 leftmost bits using bitwise AND
unsigned long bitmask = 4095; //0000 0000 0000 0000 0000 1111 1111 1111
return addr & bitmask;
}

unsigned long getPage(unsigned long addr){
//right shift virtual address 12 bits
return addr >> 12;
}
