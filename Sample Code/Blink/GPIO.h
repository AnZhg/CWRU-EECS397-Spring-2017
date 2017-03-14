// Created by Zhigang An on Mar. 13, 2017.
// Case Western Reserve university
// 
// Header file for GPIO library.

#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// Mapping to /dev/mem.
int initializeGPIO();

// Removes mapping between process memory and physical memory.
int terminateGPIO();

// Confiigure GPIO direction.
void setPinDirection(unsigned int pin, const unsigned int direction);

// Reads pin value.
unsigned int digitalRead(unsigned int pin);

// GPIO registers.
#define GPFSET0	7u
#define GPFCLR0	10u
#define GPFLEV0	13u
#define GPFSEL0	0u
#define GPFSEL1	1u
#define GPFSEL2	2u
#define GPFSEL3	3u

// Pin directions.
#define INPUT	0u
#define OUTPUT	1u

// Output states.
#define LOW		0u
#define HIGH	1u

// Mapping result.
volatile unsigned int *gpio;

// Write pin.
void digitalWrite(unsigned int pin, const unsigned int state);

// For maximum speed, used the following inline functions.
// Pull a pin high.
static void inline writePinHigh(unsigned int pin)
{
	*(gpio + GPFSET0) = (1 << pin);
}

// Pull a pin low.
static void inline writePinLow(unsigned int pin)
{
	*(gpio + GPFCLR0) = (1 << pin);
}

#endif  // GPIO_H
