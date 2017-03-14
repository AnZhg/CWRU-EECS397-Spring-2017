// Created by Zhigang An on Mar. 13, 2017.
// Case Western Reserve university
// 
// Source file for GPIO library.

#include "GPIO.h"

// GPIO base address for Raspberry Pi 2 or later.
#define GPIO_BASE_ADDR	0x3F200000
// GPIO map length.
#define GPIO_MAP_LEN	0xB4

/**
 * This function maps a block of physical memory into the memory of 
 * the calling process. It enables a user space process to access 
 * registers in physical memory directly without having to interact 
 * with in kernel side code i.e. device drivers
 **/
volatile unsigned int *mapRegisterAddress(unsigned long baseAddress)
{
	int mem_fd = 0;
	void *registerAddressMap = MAP_FAILED;
	
	// Open /dev/mem.
	if (!mem_fd) {
		if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC)) < 0) {
			perror("Can't open /dev/mem.");
			exit(1);
		}
	}
	
	// mmap IO.
	registerAddressMap = mmap(
		NULL,							// Any adddress in our space will do.
		GPIO_MAP_LEN,					// Map length.
		PROT_READ|PROT_WRITE|PROT_EXEC,	// Enable reading & writting to mapped memory.
		MAP_SHARED|MAP_LOCKED,			// Shared with other processes.
		mem_fd,							// File to map.
		baseAddress						// Offset to base address.
	);
	
	if (registerAddressMap == MAP_FAILED) {
		perror("mmap error.");
		close(mem_fd);
		exit (1);
	}
	
	// Close /dev/mem after mmap.
	if(close(mem_fd) < 0) {
		perror("Couldn't close /dev/mem file descriptor.");
		exit(1);
	}	
	
	return (volatile unsigned *)registerAddressMap;
}

// Initialize.
int initializeGPIO()
{
	gpio = mapRegisterAddress(GPIO_BASE_ADDR);
}

// Clean up.
int terminateGPIO()
{
	// unmap GPIO registers (physical memory) from process memoy.
	if (munmap((void *)gpio, GPIO_MAP_LEN) < 0) {
		perror("munmap(gpio) failed.");
		exit(1);
	}
}

// Write a 32-bit value to one of the GPIO addresses.
void writeGPIORegister(unsigned int reg, unsigned int value)
{
	*(gpio + reg) = value;
}

// Read a 32-bit value from one of the GPIO addresses.
void readGPIORegister(unsigned int reg, unsigned int value)
{
	value = *(gpio + reg);
}

// Set GPIO direction.
void setPinDirection(unsigned int pin, const unsigned int direction)
{
	unsigned int GPFSEL;
	
	switch (pin / 10) {	
		case 0:
			GPFSEL = GPFSEL0;
			break;
		case 1:
			GPFSEL = GPFSEL1;
			break;
		case 2:
			GPFSEL = GPFSEL2;
			break;
		case 3:
			GPFSEL = GPFSEL3;
			break;
		
		default:
			break;
	}
	
	*(gpio + GPFSEL) &= ~(7 << ((pin % 10) * 3));
	
	if (direction == OUTPUT) {
		*(gpio + GPFSEL) |=  (1 << ((pin % 10) * 3));
	}
}

// Read pin and return its value.
unsigned int digitalRead(unsigned int pin)
{
	return ((*(gpio + GPFLEV0) & (1 << pin)) != 0);
}
	
// Write GPIO.
void digitalWrite(unsigned int pin, const unsigned int state)
{
	if(state == HIGH) {
		*(gpio + GPFSET0) = (1 << pin);
	} else {
		*(gpio + GPFCLR0) = (1 << pin);
	}
}
