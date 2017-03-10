// Create by AnZhg on Mar. 9, 2017.
// Case Western Reserve University
//
// Required Library: wiringPi
// Compile: gcc -Os HTSensor_intr.c -lwiringPi -o HTSensor_intr
//
// Using interrupt method to read DHT sensor.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <wiringPi.h>

// DHT sensor is connected to pin 4.
#define DHT_PIN 4

// Required by API. Does nothing.
void interruptHandler()
{
}

// Read DHT sensor.
// Return 0 when successful;
// -1 when interrupt time out during initialzation;
// -2 when interrupt time out during transaction;
// -3 when bad data.
int readSensor(uint8_t *humidity, uint8_t *temperature)
{
	// Decimal part of temperature and humidity.
	// Not used by sensor. Only for validating data.
	uint8_t decHumidity;
        uint8_t decTemperature;
	
	// Checksum of data transaction.
	uint8_t checksum;
	
	// Set pin mode as output.
	pinMode(DHT_PIN, OUTPUT);

	// Set pull-up resistor.
	pullUpDnControl(DHT_PIN, PUD_UP);
	
	// Pull low.
	digitalWrite(DHT_PIN, LOW);
	
	// Delay 20 ms.
	delay(20);
	
	// Pull high.
	digitalWrite(DHT_PIN, HIGH);
	
	// Wait for 20 us.
	delayMicroseconds(20);

  	// Set pin mode as input.
	pinMode(DHT_PIN, INPUT);
	
	int interruptCounter = 0;
	
	// Don't count time during initialization of communication.
	while (interruptCounter < 3) {
		if (waitForInterrupt(DHT_PIN, 1000) != 1) {
			// Failed when waiting for interrupt during initialization.
			return -1;
		}
		
		// Increment counter.
		++interruptCounter;
	}
	
	// Reset counter.
	interruptCounter = 0;
	
	// Data pointer.
	uint8_t *p;
	
	unsigned int startTime = micros();
	unsigned int endTime;
	
	// Get 40 bits data.
	while(interruptCounter < 40) {
		if (waitForInterrupt(DHT_PIN, 1000) == 1) {
			// Successful.
			// Get current time.
			endTime = micros();
			
			// Point to corresponding data.
			switch (interruptCounter) {
				case 0:
					p = humidity;
					break;
				case 8:
					p = &decHumidity;
					break;
				case 16:
					p = temperature;
					break;
				case 24:
					p = &decTemperature;
					break;
				case 32:
					p = &checksum;
					break;
				
				default:
					break;
			}
			
			(*p) <<= 1;
			(*p) |= endTime - startTime > 100;
			
			// Increment counter.
			++interruptCounter;
			
			// Current end time is next start time.
			startTime = endTime;
		} else {
			// Failed when waiting for interrupt during transaction.
			return -2;
		}
	}
	
	// We got enough data. Check it.
	if (*humidity + decHumidity + *temperature + decTemperature == checksum) {
		// Good data.
		return 0;
	} else {
		// Bad data.
		return -3;
	}
}

int main(int argv, const char argc[])
{
	// Silence compiler warning when -Wextra is used.
	(void)argc;
	(void)argv;
	
	// Setup wiringPi.
	wiringPiSetupGpio();
	
	// Register interrupt handler. Triggered when there is a rising edge.
	wiringPiISR(DHT_PIN, INT_EDGE_RISING, &interruptHandler);
	
	// Temperature and relative humidity.
	uint8_t humidity, temperature;
	
	// Trial times.
	int attempts = 0;
	
	// Success flag.
	bool didSucceedReading = false;
	
	// Read DHT sensor.
	while (true) {
		if (readSensor(&humidity, &temperature) != 0) {
			if (++attempts == 20) {
				// Too many error readings.
				break;
			}
			
			// Clean up.
			wiringPiSetupGpio();
			
			// Delay a little bit.
			delay(20);
		} else {
			didSucceedReading = true;
			break;
		}
	}
	
	if (didSucceedReading) {
		printf("Failed %d time(s).\n", attempts);
		printf("Humidity: %d%%. Temperature: %d °C.\n", humidity, temperature);
	} else {
		printf("Failed reading sensor.\n");
	}
}
