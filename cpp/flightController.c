#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GYROADDR 0x1c
#define MMAPSIZE 16
#define MMAPLOCATION "/run/user/1000/mmapTest"

// Declare memory mapped variables
int sharedMem;
void *mappedBuffer;
short *throttle;

int main() {
	// Setup mmap for sharing variables with node
	sharedMem = open(MMAPLOCATION,O_RDWR | O_CREAT,0666); // Create the file in tmpfs
	lseek(sharedMem,MMAPSIZE-1,SEEK_SET); // Stretch the file
	write(sharedMem,"",1); // Write empty data to the end
	sync(); // Sync to fs for node

	mappedBuffer = mmap(NULL,2048,PROT_READ | PROT_WRITE, MAP_SHARED,sharedMem,0);

	// Setup memory locations
	throttle = (short *) mappedBuffer;

	while(*throttle == 0) {
		sleep(0);
	}

	std::cout << *throttle << "\n";

	close(sharedMem);

	return 0;
}
