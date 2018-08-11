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

	// Setup I2C devices
	int gyro = wiringPiI2CSetup(GYROADDR);
	int mtr = pca9685Setup(CH0, 0x40, PWMFREQ);

	// Initialize devices
	wiringPiSetup();
	pca9685PWMReset(mtr);

	// sd_notify(0, "READY=1");

	float motors[4];

	while(1) {
		*gX = wiringPiI2CReadReg16(gyro,GYROX);
		*gY = wiringPiI2CReadReg16(gyro,GYROY);
		// std::cout << *gX << " " << *gY << "\n";

		float x = ((float) *gX - *cX) / 32767 * (*sensorMult);
		float y = ((float) *gY - *cY) / 32767 * (*sensorMult);

		motors[0] = ( y/2) + ( x/2) + ( *moveX/2) + (-*moveY/2) + (-*moveRot/2) + (*throttle);
		motors[1] = (-y/2) + ( x/2) + (-*moveX/2) + (-*moveY/2) + ( *moveRot/2) + (*throttle);
		motors[2] = ( y/2) + (-x/2) + ( *moveX/2) + ( *moveY/2) + ( *moveRot/2) + (*throttle);
		motors[3] = (-y/2) + (-x/2) + (-*moveX/2) + ( *moveY/2) + (-*moveRot/2) + (*throttle);

		for (char i = 0; i < 4; i++) {
			if(motors[i] > 1) {
				float offset = motors[i] - 1;
				for (char j = 0; j < 4; j++) {
					motors[j] -= offset;
					if(motors[j] < 0) {
						motors[j] = 0.085;
					}
				}
			} else if(motors[i] < 0) {
				motors[i] = 0.085;
			}
		}

		std::cout << *sensorMult << "\t" << motors[0] << "\t" << motors[1] << "\t" << motors[2] << "\t" << motors[3] << "\n";

		sleep(0);
	}



	close(sharedMem);

	return 0;
}
