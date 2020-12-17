//============================================================================
// Name        : Hexapodo.cpp
// Author      : NikhilArayath
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
//

// Uses POSIX functions to send and receive data from a Maestro.
// NOTE: The Maestro's serial mode must be set to "USB Dual Port".
// NOTE: You must change the 'const char * device' line below.
//Libraries

//All Servo Numbers move in clockwise direction

#include <iostream>
#include <fcntl.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <windows.h>

#define PI 3.14159265
#ifdef _WIN32
#define O_NOCTTY 0
#else
#include <termios.h>
#endif
#include <string>
using namespace std;

static int Knee1 = 0;
static int Knee2 = 1;
static int Knee3 = 2;
static int Knee4 = 3;
static int Knee5 = 4;
static int Knee6 = 5;
static int Ankle1 = 6;
static int Ankle2 = 7;
static int Ankle3 = 8;
static int Ankle4 = 9;
static int Ankle5 = 10;
static int Ankle6 = 11;
int Hip1 = 12;
int Hip2 = 13;
int Hip3 = 14;
int Hip4 = 15;
int Hip5 = 16;
int Hip6 = 17;
int targetKnee1;
int targetKnee2;
int targetKnee3;
int targetKnee4;
int targetKnee5;
int targetKnee6;
int targetAnkle1;
int targetAnkle2;
int targetAnkle3;
int targetAnkle4;
int targetAnkle5;
int targetAnkle6;
int targetHip1;
int targetHip2;
int targetHip3;
int targetHip4;
int targetHip5;
int targetHip6;
int Knee1Save;
int Knee2Save;
int Knee3Save;
int Knee4Save;
int Knee5Save;
int Knee6Save;
int Ankle1Save;
int Ankle2Save;
int Ankle3Save;
int Ankle4Save;
int Ankle5Save;
int Ankle6Save;
int Hip1Save;
int Hip2Save;
int Hip3Save;
int Hip4Save;
int Hip5Save;
int Hip6Save;

// Gets the position of a Maestro channel.
// See the "Serial Servo Commands" section of the user's guide.
int maestroGetPosition(int fd, unsigned char channel)
{
	unsigned char command[] = {0x90, channel};
	if(write(fd, command, sizeof(command)) == -1)
	{
	perror("error writing");
	return -1;
	}
	unsigned char response[2];
	if(read(fd,response,2) != 2)
	{
	perror("error reading");
	return -1;
	}
	return response[0] + 256*response[1];
}

// Sets the target of a Maestro channel.
// See the "Serial Servo Commands" section of the user's guide.
// The units of 'target' are quarter-microseconds.
int maestroSetTarget(int fd, unsigned char channel, unsigned short target)
{
	unsigned char command[] = {0x84, channel, target & 0x7F, target >> 7 & 0x7F};
	if (write(fd, command, sizeof(command)) == -1)
	{
	perror("error writing");
	return -1;
	}
	return 0;
}


/**
 * This command will return the error number on the Pololu Maestro Board
 * The error number is made of two bytes. so the response needs to add the returned
 * bytes together.
 *
 * To get the error we have to write 0xA1 to the controller to set it into error mode
 * Then read the error
 *
 * @param int fd - The file descriptor to the device
 *
 * @returns int - The number that represents the Error. See Pololu documentation for error numbers
 */
int maestroGetError(int fd)
{
    unsigned char command[] = { 0xA1 };
    if (write(fd, command, sizeof(command)) == -1)
    {
        perror("error getting Error");
        return -1;
    }

    unsigned char response[2];
    if(read(fd,response,1) != 1)
    {
        perror("error reading");
        return -1;
    }

    //Helpfull for debugging
    //printf("Error first: %d\n", response[0]);
    //printf("Error secon: %d\n", response[1]);

    return (int)sqrt(response[0] + 256*response[1]);
}




void moveForward(int fd)
{
	int positionKnee1 = maestroGetPosition(fd, Knee1);
	Knee1Save = positionKnee1;
	int positionKnee3 = maestroGetPosition(fd, Knee3);
	Knee3Save = positionKnee3;
	int positionKnee5 = maestroGetPosition(fd, Knee5);
	Knee5Save = positionKnee5;
	targetKnee1 = (positionKnee1 -2000);
	targetKnee3 = (positionKnee3 -2000);
	targetKnee5 = (positionKnee5 -2000);
	maestroSetTarget(fd, Knee1, targetKnee1);
	maestroSetTarget(fd, Knee3, targetKnee3);
	maestroSetTarget(fd, Knee5, targetKnee5);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec


	int positionHip1 = maestroGetPosition(fd, Hip1);
	Hip1Save = positionHip1;
	int positionHip2 = maestroGetPosition(fd, Hip2);
	Hip2Save = positionHip2;
	int positionHip3 = maestroGetPosition(fd, Hip3);
	Hip3Save = positionHip3;
	int positionHip4 = maestroGetPosition(fd, Hip4);
	Hip4Save = positionHip4;
	int positionHip5 = maestroGetPosition(fd, Hip5);
	Hip5Save = positionHip5;
	int positionHip6 = maestroGetPosition(fd, Hip6);
	Hip6Save = positionHip6;
	targetHip1 = (positionHip1 +1500);
	targetHip3 = (positionHip3 -1500);
	targetHip5 = (positionHip5 -1500);
	maestroSetTarget(fd, Hip1, targetHip1);
	maestroSetTarget(fd, Hip3, targetHip3);
	maestroSetTarget(fd, Hip5, targetHip5);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	positionKnee1 = maestroGetPosition(fd, Knee1);
	positionKnee3 = maestroGetPosition(fd, Knee3);
	positionKnee5 = maestroGetPosition(fd, Knee5);
	targetKnee1 = (positionKnee1 +2000);
	targetKnee3 = (positionKnee3 +2000);
	targetKnee5 = (positionKnee5 +2000);
	maestroSetTarget(fd, Knee1, targetKnee1);
	maestroSetTarget(fd, Knee3, targetKnee3);
	maestroSetTarget(fd, Knee5, targetKnee5);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec
	//________________________________________________________________________________________________________________
	//THE NEXT STEP HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY

	int positionKnee2 = maestroGetPosition(fd, Knee2);
	Knee2Save = positionKnee2;
	int positionKnee4 = maestroGetPosition(fd, Knee4);
	Knee4Save = positionKnee4;
	int positionKnee6 = maestroGetPosition(fd, Knee6);
	Knee6Save = positionKnee6;
	targetKnee2 = (positionKnee2 -2000);
	targetKnee4 = (positionKnee4 -1500);
	targetKnee6 = (positionKnee6 -2000);
	maestroSetTarget(fd, Knee2, targetKnee2);
	maestroSetTarget(fd, Knee4, targetKnee4);
	maestroSetTarget(fd, Knee6, targetKnee6);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	positionHip1 = maestroGetPosition(fd, Hip1);
	positionHip2 = maestroGetPosition(fd, Hip2);
	positionHip3 = maestroGetPosition(fd, Hip3);
	positionHip4 = maestroGetPosition(fd, Hip4);
	positionHip5 = maestroGetPosition(fd, Hip5);
	positionHip6 = maestroGetPosition(fd, Hip6);
	targetHip2 = (positionHip2 +1500);
	targetHip4 = (positionHip4 -1500);
	targetHip6 = (positionHip6 +1500);
	maestroSetTarget(fd, Hip2, targetHip2);
	maestroSetTarget(fd, Hip4, targetHip4);
	maestroSetTarget(fd, Hip6, targetHip6);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	positionKnee2 = maestroGetPosition(fd, Knee2);
	positionKnee4 = maestroGetPosition(fd, Knee4);
	positionKnee6 = maestroGetPosition(fd, Knee6);
	targetKnee2 = (positionKnee2 +2000);
	targetKnee4 = (positionKnee4 +2000);
	targetKnee6 = (positionKnee6 +2000);
	maestroSetTarget(fd, Knee2, targetKnee2);
	maestroSetTarget(fd, Knee4, targetKnee4);
	maestroSetTarget(fd, Knee6, targetKnee6);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec
}

void twist(int fd){

	int positionKnee1 = maestroGetPosition(fd, Knee1);
	Knee1Save = positionKnee1;
	int positionKnee3 = maestroGetPosition(fd, Knee3);
	Knee3Save = positionKnee3;
	int positionKnee5 = maestroGetPosition(fd, Knee5);
	Knee5Save = positionKnee5;
	targetKnee1 = (positionKnee1 -2000);
	targetKnee3 = (positionKnee3 -2000);
	targetKnee5 = (positionKnee5 -2000);
	maestroSetTarget(fd, Knee1, targetKnee1);
	maestroSetTarget(fd, Knee3, targetKnee3);
	maestroSetTarget(fd, Knee5, targetKnee5);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	int positionHip1 = maestroGetPosition(fd, Hip1);
	Hip1Save = positionHip1;
	int positionHip2 = maestroGetPosition(fd, Hip2);
	Hip2Save = positionHip2;
	int positionHip3 = maestroGetPosition(fd, Hip3);
	Hip3Save = positionHip3;
	int positionHip4 = maestroGetPosition(fd, Hip4);
	Hip4Save = positionHip4;
	int positionHip5 = maestroGetPosition(fd, Hip5);
	Hip5Save = positionHip5;
	int positionHip6 = maestroGetPosition(fd, Hip6);
	Hip6Save = positionHip6;
	targetHip1 = (positionHip1 +1000);
	targetHip3 = (positionHip3 +1000);
	targetHip5 = (positionHip5 +1000);
	maestroSetTarget(fd, Hip1, targetHip1);
	maestroSetTarget(fd, Hip3, targetHip3);
	maestroSetTarget(fd, Hip5, targetHip5);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	positionKnee1 = maestroGetPosition(fd, Knee1);
	positionKnee3 = maestroGetPosition(fd, Knee3);
	positionKnee5 = maestroGetPosition(fd, Knee5);
	targetKnee1 = (positionKnee1 +2000);
	targetKnee3 = (positionKnee3 +2000);
	targetKnee5 = (positionKnee5 +2000);
	maestroSetTarget(fd, Knee1, targetKnee1);
	maestroSetTarget(fd, Knee3, targetKnee3);
	maestroSetTarget(fd, Knee5, targetKnee5);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec
	//________________________________________________________________________________________________________________
	//THE NEXT STEP HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY HOORAY

	int positionKnee2 = maestroGetPosition(fd, Knee2);
	Knee2Save = positionKnee2;
	int positionKnee4 = maestroGetPosition(fd, Knee4);
	Knee4Save = positionKnee4;
	int positionKnee6 = maestroGetPosition(fd, Knee6);
	Knee6Save = positionKnee6;
	targetKnee2 = (positionKnee2 -2000);
	targetKnee4 = (positionKnee4 -1500);
	targetKnee6 = (positionKnee6 -2000);
	maestroSetTarget(fd, Knee2, targetKnee2);
	maestroSetTarget(fd, Knee4, targetKnee4);
	maestroSetTarget(fd, Knee6, targetKnee6);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	positionHip1 = maestroGetPosition(fd, Hip1);
	positionHip2 = maestroGetPosition(fd, Hip2);
	positionHip3 = maestroGetPosition(fd, Hip3);
	positionHip4 = maestroGetPosition(fd, Hip4);
	positionHip5 = maestroGetPosition(fd, Hip5);
	positionHip6 = maestroGetPosition(fd, Hip6);
	targetHip2 = (positionHip2 +1000);
	targetHip4 = (positionHip4 +1000);
	targetHip6 = (positionHip6 +1000);
	maestroSetTarget(fd, Hip2, targetHip2);
	maestroSetTarget(fd, Hip4, targetHip4);
	maestroSetTarget(fd, Hip6, targetHip6);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec

	positionKnee2 = maestroGetPosition(fd, Knee2);
	positionKnee4 = maestroGetPosition(fd, Knee4);
	positionKnee6 = maestroGetPosition(fd, Knee6);
	targetKnee2 = (positionKnee2 +2000);
	targetKnee4 = (positionKnee4 +2000);
	targetKnee6 = (positionKnee6 +2000);
	maestroSetTarget(fd, Knee2, targetKnee2);
	maestroSetTarget(fd, Knee4, targetKnee4);
	maestroSetTarget(fd, Knee6, targetKnee6);

	Sleep(500); //Message Sleep for 1/2 Sec 1000ms = 1sec
}



void reset(int fd)
{
	targetKnee1 = (Knee1Save);
	targetKnee2 = (Knee2Save);
	targetKnee3 = (Knee3Save);
	targetKnee4 = (Knee4Save);
	targetKnee5 = (Knee5Save);
	targetKnee6 = (Knee6Save);
	maestroSetTarget(fd, Knee1, targetKnee1);
	maestroSetTarget(fd, Knee2, targetKnee2);
	maestroSetTarget(fd, Knee3, targetKnee3);
	maestroSetTarget(fd, Knee4, targetKnee4);
	maestroSetTarget(fd, Knee5, targetKnee5);
	maestroSetTarget(fd, Knee6, targetKnee6);

	targetHip1 = (Hip1Save);
	targetHip2 = (Hip2Save);
	targetHip3 = (Hip3Save);
	targetHip4 = (Hip4Save);
	targetHip5 = (Hip5Save);
	targetHip6 = (Hip6Save);
	maestroSetTarget(fd, Hip1, targetHip1);
	maestroSetTarget(fd, Hip2, targetHip2);
	maestroSetTarget(fd, Hip3, targetHip3);
	maestroSetTarget(fd, Hip4, targetHip4);
	maestroSetTarget(fd, Hip5, targetHip5);
	maestroSetTarget(fd, Hip6, targetHip6);
	sleep(1);
}

int main() {
	cout << "HEXAPODO" << endl; // prints HEXAPODO
	printf("Inside main loop\n");
	  // Open the Maestro's virtual COM port.
		const char * device = "\\\\.\\COM4";  //"\\\\.\\USBSER000"; // Windows, "\\\\.\\COM6" also works

		int fd = open(device, O_RDWR | O_NOCTTY);
		if (fd == -1)
		{
			perror(device);
			return 1;
		}

		//twist(fd);

		for (int i=0; i<10; i++){

		moveForward(fd);
		reset(fd);
		}
		for (int i=0; i<5; i++){

		twist(fd);
		reset(fd);
		}
		reset(fd);

		close(fd);
		return 0;
}

