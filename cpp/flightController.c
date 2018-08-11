#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
// #include <systemd/sd-daemon.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <pca9685.h>

#define GYROADDR 0x1c
#define GYROX 0x28
#define GYROY 0x2a
#define MMAPLOCATION "/run/user/1000/mmapTest"
#define MMAPSIZE 44
#define READSIZE 28
#define WRITESIZE 16
#define PWMMIN 200
#define PWMMAX 500
#define SENSORDIV 30

#define PCA9685 0x40
#define PWMFREQ 50
#define CH0 300
#define CH1 301
#define CH2 302
#define CH3 303
#define ARMSPEED 85

// Declare memory mapped variables
int sharedMem;
char mappedBuffer[MMAPSIZE];
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

int main() {
	// Setup mmap for sharing variables with node
	sharedMem = open(MMAPLOCATION,O_RDWR | O_CREAT,0666); // Create the file in tmpfs
	lseek(sharedMem,MMAPSIZE-1,SEEK_SET); // Stretch the file
	write(sharedMem,"",1); // Write empty data to the end
	sync(); // Sync to fs for node

	// Setup memory locations
	// throttle = (float *) mappedBuffer;
	throttle = (short *) &mappedBuffer;
	moveX = (short *) &mappedBuffer + ( sizeof(short) * 1 );
	moveY = (short *) &mappedBuffer + ( sizeof(short) * 2 );
	moveRot = (short *) &mappedBuffer + ( sizeof(short) * 3);
	cX = (short *) &mappedBuffer + ( sizeof(short) * sizeof(short));
	cY = (short *) &mappedBuffer + ( sizeof(short) * 5);
	// sensorMult = (short *) &mappedBuffer + ( sizeof(short) * 6);
	gX = (short *) &mappedBuffer + ( sizeof(short) * 7);
	gY = (short *) &mappedBuffer + ( sizeof(short) * 8);
	gZ = (short *) &mappedBuffer + ( sizeof(short) * 9);
	bar = (short *) &mappedBuffer + ( sizeof(short) * 10);

	// Setup I2C devices
	int gyro = wiringPiI2CSetup(GYROADDR);
	int mtr = pca9685Setup(CH0, 0x40, PWMFREQ);

	// Initialize devices
	wiringPiSetup();
	pca9685PWMReset(mtr);

	// sd_notify(0, "READY=1");

	short motors[4];

	while(1) {
		sync();
		lseek(sharedMem,0,SEEK_SET); // Go to inputs
		read(sharedMem, &mappedBuffer, READSIZE); // Read inputs

		*gX = wiringPiI2CReadReg16(gyro,GYROX);
		*gY = wiringPiI2CReadReg16(gyro,GYROY);
		// std::cout << *gX << " " << *gY << "\n";

		short x = (*gX - *cX) / 33 / (SENSORDIV);
		short y = (*gY - *cY) / 33 / (SENSORDIV);

		motors[0] = ( y/2) + ( x/2) + (( *moveX / (1000 / *throttle))/2) + ((-*moveY / (1000 / *throttle))/2) + (-*moveRot/2) + (*throttle);
		motors[1] = (-y/2) + ( x/2) + ((-*moveX / (1000 / *throttle))/2) + ((-*moveY / (1000 / *throttle))/2) + ( *moveRot/2) + (*throttle);
		motors[2] = ( y/2) + (-x/2) + (( *moveX / (1000 / *throttle))/2) + (( *moveY / (1000 / *throttle))/2) + ( *moveRot/2) + (*throttle);
		motors[3] = (-y/2) + (-x/2) + ((-*moveX / (1000 / *throttle))/2) + (( *moveY / (1000 / *throttle))/2) + (-*moveRot/2) + (*throttle);

		for (char i = 0; i < 4; i++) {
			if(motors[i] > 1000) {
				short offset = motors[i] - 1000;
				for (char j = 0; j < 4; j++) {
					motors[j] -= offset;
					if(motors[j] < 0) {
						motors[j] = ARMSPEED;
					}
				}
			} else if(motors[i] < 0) {
				motors[i] = ARMSPEED;
			}
		}

		//std::cout << *throttle << "\t" << *moveX << "\t" << *moveY << "\t" << *moveRot << "\t" << *cX << "\t" << *cY << "\t" << SENSORDIV << "\n";

		for (char i = 0; i < 4; i++) {
			if(*throttle >= ARMSPEED) {
				pwmWrite((CH0 + i),motors[i]/3 + PWMMIN);
			} else {
				pwmWrite((CH0 + i),PWMMIN);
			}
		}

		write(sharedMem, &gX, WRITESIZE);
		sync();

		sleep(0);
	}



	close(sharedMem);

	return 0;
}
