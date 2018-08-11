#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GYROADDR 0x1c
#define GYROX 0x28
#define GYROY 0x2a
#define MMAPSIZE 50
#define MMAPLOCATION "/run/user/1000/mmapTest"
#define PWMMIN 200
#define PWMMAX 500

#define PCA9685 0x40
#define PWMFREQ 50
#define CH0 300
#define CH1 301
#define CH2 302
#define CH3 303

// Declare memory mapped variables
int sharedMem;
void *mappedBuffer;
float *throttle;
float *moveX;
float *moveY;
float *moveRot;
float *gX;
float *gY;
float *gZ;
float *bar;
float *cX;
float *cY;
float *sensorMult;

int main() {
	// Setup mmap for sharing variables with node
	sharedMem = open(MMAPLOCATION,O_RDWR | O_CREAT,0666); // Create the file in tmpfs
	lseek(sharedMem,MMAPSIZE-1,SEEK_SET); // Stretch the file
	write(sharedMem,"",1); // Write empty data to the end
	sync(); // Sync to fs for node

	mappedBuffer = mmap(NULL,2048,PROT_READ | PROT_WRITE, MAP_SHARED,sharedMem,0);

	// Setup memory locations
	// throttle = (float *) mappedBuffer;
	throttle = (float *) mappedBuffer;
	moveX = (float *) mappedBuffer + ( sizeof(float) * 1 );
	moveY = (float *) mappedBuffer + ( sizeof(float) * 2 );
	moveRot = (float *) mappedBuffer + ( sizeof(float) * 3);
	gX = (float *) mappedBuffer + ( sizeof(float) * 4);
	gY = (float *) mappedBuffer + ( sizeof(float) * 5);
	gZ = (float *) mappedBuffer + ( sizeof(float) * 6);
	bar = (float *) mappedBuffer + ( sizeof(float) * 7);
	cX = (float *) mappedBuffer + ( sizeof(float) * 8);
	cY = (float *) mappedBuffer + ( sizeof(float) * 9);
	sensorMult = (float *) mappedBuffer + ( sizeof(float) * 10);

	// sd_notify(0, "READY=1");


	while(1) {
		*sensorMult = 1;

		msync(mappedBuffer, MMAPSIZE, MS_SYNC | MS_INVALIDATE);

		sleep(0);
	}



	close(sharedMem);

	return 0;
}
