#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char * argv[]){

unsigned long addr = atoll(argv[1]);
//saves argument one of type long long in the unsigned long varible addr
//range 0 - 4,294,967,295 (32-bit)
unsigned long page, offset, bitmask;

//right shift virtual address 12 bits
page = addr >> 12;

//select only the 12 leftmost bits using bitwise AND
bitmask = 4095;	//0000 0000 0000 0000 0000 1111 1111 1111
offset = addr & bitmask;

printf("The address %lu contains:\n", addr);
printf("Page number =  %lu\n", page);
printf("Offset      =  %lu\n", offset);

return 0;
}
