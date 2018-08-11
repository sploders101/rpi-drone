#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <wiringPiI2C.h>
#include <pca9685.h>

#define GYROADDR 0x1c
#define GYROX 0x28
#define GYROY 0x2a
#define MMAPSIZE 22
#define MMAPLOCATION "/run/user/1000/mmapTest"

#define PCA9685 0x40
#define CH0 0x08

// Declare memory mapped variables
int sharedMem;
void *mappedBuffer;
unsigned short *throttle;
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
	// throttle = (short *) mappedBuffer;
	throttle = (unsigned short *) mappedBuffer;
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
	int mtr = wiringPiI2CSetup(PCA9685);

	// Initialize devices
	pca9685PWMReset(mtr);

	while(1) {
		*gX = wiringPiI2CReadReg16(gyro,GYROX);
		*gY = wiringPiI2CReadReg16(gyro,GYROY);
		std::cout << *gX << " " << *gY << "\n";

		// wiringPiI2CWriteReg16(mtr,CH0,*throttle);
		pca9685PWMWrite(mtr,0,0,*throttle);

		sleep(0);
	}



	close(sharedMem);

	return 0;
}
