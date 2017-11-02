#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

void findPhysAddr( unsigned long logAddr, char* data );//Determines physical address from logical address, updates page table, and loads frame into memory
void initPT();//initializes PT
char *data = 0;//my baking store

unsigned long pageTable[256][2]; //first column holds physical address and second holds valid/invalid bit
static int pageFault = 0;
signed char physMem[65536];

int main( int argc, char * argv[]){
	initPT();

	char fname[30], buf[1], addr[6] = {'\0', '\0', '\0', '\0', '\0', '\0'};
	FILE * ofp;
	int afd, bsfd, ofd; //file descriptors
	int ctrl, bytes, x = 0, y = 0, addrCount = 0;
	float faultRate;
	unsigned long logAddr, physAddr;

	strcpy(fname, argv[1]);
	afd = open(fname, O_RDONLY);
	bsfd = open("BACKING_STORE.bin", O_RDONLY);
	data = mmap(0, 65536, PROT_READ, MAP_PRIVATE, bsfd, 0); //creates memory map of entire bin file
	ofd = open("results.txt", O_RDWR | O_CREAT | O_TRUNC, S_IWUSR | S_IRUSR  | S_IWGRP | S_IRGRP | S_IROTH |S_IWOTH);//clear output file
	close(ofd);

	while(1){
		bytes = read(afd, buf, 1);
		if(bytes < 1) {//if hit EOF
			break;
		}
		if( (buf[0] < 48 || buf[0] > 57) && (y == 0) ){//if char not between 0 and 9 and previously scanned char was
			logAddr = atoll(addr);
			x = 0;
			addrCount++;
			addr[0] = '\0';
			addr[1] = '\0';
			addr[2] = '\0';
			addr[3] = '\0';
			addr[4] = '\0';
			addr[5] = '\0';//clear addr to scen new address
			y = 1;
			findPhysAddr( logAddr, data );
		}else{//else char between 0-9
			y = 0;
			addr[x] = buf[0];
			x++;
		}
	}

	faultRate = (float)pageFault / (float)addrCount;

	ofp = fopen("results.txt", "a");
	fprintf(ofp, "Number of Translated Addresses = %d\n", addrCount);
	fprintf(ofp, "Page Faults = %d\n", pageFault);
	fprintf(ofp, "Page Fault Rate = %.3f\n", faultRate);
	fclose(ofp);

	printf("Number of Translated Addresses = %d\n", addrCount);
	printf("Page Faults = %d\n", pageFault);
	printf("Page Fault Rate = %.3f\n", faultRate);

	close(bsfd);
	close(afd);
	return 0;
}

void findPhysAddr( unsigned long logAddr, char * data ){

	unsigned long page, frame, offset, physAddr, bitmask;
	static unsigned long count = 0;//number of entries in page table
	int value;
	FILE *fp;

	bitmask = 65535;//set 16 leftmost bits to zero
	logAddr = logAddr & bitmask;

	page = logAddr >> 8;//right shift virtual address 12 bits

	bitmask = 255;	//set 24 leftmost bits to zero
	offset = logAddr & bitmask;

	if(pageTable[page][1] == 0){ //valid
		frame = pageTable[page][0];
		physAddr = frame << 8;
		physAddr += offset; //add new offset
		value = physMem[frame*256 + offset];
	}else{//invalid
		physAddr = count << 8; //page number equals number of entries in pageTable
		physAddr += offset; //offset equals same offset as logical address
		count++;
		pageFault++;
		pageTable[page][1] = 0; //set to valid
		frame = physAddr >> 8;	//extraxt frame from physical address
		pageTable[page][0] = frame; //update frame table
		memcpy(physMem+frame*256, data+page*256, 256); //load frame into memory
		value = physMem[frame*256 + offset]; //obtain value from frame
	}

	fp = fopen("results.txt", "a");
	fprintf(fp, "Vitual address: %lu\t", logAddr);
	fprintf(fp, "Physical addrss: %lu\t", physAddr);
	fprintf(fp, "Value: %d\n",value);
	fclose(fp);

	printf("Vitual address: %lu\t", logAddr);
	printf("Physical addrss: %lu\t", physAddr);
	printf("Value: %d\n",value);

}

void initPT(){
	int i ;
	for(i = 0; i< 256; i++){
		pageTable[i][1] = 1; //set to invalid
		pageTable[i][0] = 256; // outside range for page number, used to indicate value not set
	}

	return;
}
