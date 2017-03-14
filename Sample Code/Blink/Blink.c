// Created by Zhigang An on Mar. 13, 2017.
// Case Western Reserve university
// 
// Compile: gcc GPIO.c Blink.c -o Blink
// Run: sudo ./Blink
//
// Access registers directly to blink LED.

#include "GPIO.h"

int main(int argc, const char *argv[])
{
	// Initialize.
	initializeGPIO();
	
	// Set GPIO16 as output.
	setPinDirection(16, OUTPUT);
	
	// Blink 10 times.
	int i = 0;
	for (; i < 10; ++i) {
		// Pull GPIO4 pin high.
		writePinHigh(16);
		
		// Delay 500 ms.
		usleep(500000);
		
		// Pull GPIO16 pin low.
		writePinLow(16);
		
		// Delay 500 ms.
		usleep(500000);
	}
    
    // Cleanup.
    terminateGPIO();
	
	return 0;
}
