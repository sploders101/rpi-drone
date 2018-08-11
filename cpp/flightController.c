#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wiringPiI2C.h>

#define GYROADDR 0x1c
#define GYROX 0x28
#define GYROY 0x2a
#define MMAPSIZE 22
#define MMAPLOCATION "/run/user/1000/mmapTest"

// Declare memory mapped variables
int sharedMem;
void *mappedBuffer;
short *throttle;
short *moveX;
short *moveY;
short *moveRot;
short *gX;
short *gY;
short *gZ;
short *bar;
short *cX;
short *cY;
short *sensorMult;

int main() {
	// Setup mmap for sharing variables with node
	sharedMem = open(MMAPLOCATION,O_RDWR | O_CREAT,0666); // Create the file in tmpfs
	lseek(sharedMem,MMAPSIZE-1,SEEK_SET); // Stretch the file
	write(sharedMem,"",1); // Write empty data to the end
	sync(); // Sync to fs for node

	mappedBuffer = mmap(NULL,2048,PROT_READ | PROT_WRITE, MAP_SHARED,sharedMem,0);

	// Setup memory locations
	throttle = (short *) mappedBuffer;
	moveX = (short *) mappedBuffer + ( sizeof(short) * 1 );
	moveY = (short *) mappedBuffer + ( sizeof(short) * 2 );
	moveRot = (short *) mappedBuffer + ( sizeof(short) * 3);
	gX = (short *) mappedBuffer + ( sizeof(short) * 4);
	gY = (short *) mappedBuffer + ( sizeof(short) * 5);
	gZ = (short *) mappedBuffer + ( sizeof(short) * 6);
	bar = (short *) mappedBuffer + ( sizeof(short) * 7);
	cX = (short *) mappedBuffer + ( sizeof(short) * 8);
	cY = (short *) mappedBuffer + ( sizeof(short) * 9);
	sensorMult = (short *) mappedBuffer + ( sizeof(short) * 10);

	// Setup I2C devices
	int gyro = wiringPiI2CSetup(GYROADDR);

	while(1) {
		*gX = wiringPiI2CReadReg16(gyro,GYROX);
		*gY = wiringPiI2CReadReg16(gyro,GYROY);
		std::cout << *gX << " " << *gY << "\n";
		sleep(0);
	}



	close(sharedMem);

	return 0;
}
