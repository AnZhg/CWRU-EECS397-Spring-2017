// Create by AnZhg on Feb. 15, 2017.
// Case Western Reserve University
//
// Required Library: wiringPi
// Compile: gcc -Os HTSensor_poll.c -lwiringPi -o HTSensor_poll
//
// Using polling method to read DHT sensor.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include <wiringPi.h>

// DHT sensor is connected to GPIO4.
#define DHT_PIN 4

// Reading DHT sensor.
// Return true if data is valid, false if invalid.
bool readSensor(uint8_t *humidity, uint8_t *temperature)
{
    // Decimal part of humidity and temperature.
    // Not used by sensor. But useful for data validation.
    uint8_t decHumidity;
    uint8_t decTemperature;
    
    // Checksum.
    uint8_t checksum;
    
    // Count how many high pulses we have.
    int highPulseCounter = 0;
    
    // Data pointer.
    uint8_t *p;
    
    // Timestamps.
    unsigned int startTime, endTime;
    
    // Initialize transaction.
    // Set pin mode output.
    pinMode(DHT_PIN, OUTPUT);
    
    // Set pull-up resisitor.
    pullUpDnControl(DHT_PIN, PUD_UP);
    
    // Pull low.
    digitalWrite(DHT_PIN, LOW);
    
    // Delay 20 ms.
    delay(20);
    
    // Pull high.
    digitalWrite(DHT_PIN, HIGH);
    
    // Delay 20 us.
    delayMicroseconds(20);
    
    // Set pin mode input.
    pinMode(DHT_PIN, INPUT);
    
    // Wait for response.
    while(digitalRead(DHT_PIN) == LOW);
    
    // Wait for the end of high voltage pulse.
    delayMicroseconds(60);
    
    // Start receiving data.
    while (highPulseCounter < 40) {
        // Wait for rising edge.
        while (digitalRead(DHT_PIN) == LOW);
        
        // Get start timestamp.
        // Note that micros() wraps around after about 71 min.
        startTime = micros();
        
        // We are in high voltage. Count time to avoid infinite loop.
        while (digitalRead(DHT_PIN) == HIGH) {
            delayMicroseconds(1);
            if (micros() - startTime > 100) {
                // Waited for too long. Failed reading data.
                return false;
            }
        }
        
        // Get end timestamp.
        endTime = micros();
        
        // Point to corresponding data.
        switch (highPulseCounter) {
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
        
        *p <<= 1;
        
        // '0': 26~28 us; '1': 70 us.
        // (28 + 70) / 2 = 49.
        *p |= endTime - startTime > 49;
        
        // Increment counter.
        ++highPulseCounter;
    }
    
    // Transaction finished. Verify data.
    if (*humidity + decHumidity + *temperature + decTemperature == checksum) {
        // Good data.
        return true;
    } else {
        // Bad data.
        return false;
    }
}

int main(int argc, const char *argv[])
{
    // Setup GPIO.
    if (wiringPiSetupGpio() == -1) {
	fprintf(stderr, "Failed to initialize wiringPi.\n");
        return -1;
    }
    // Temperature and relative humidity.
    uint8_t temperature, humidity;
    
    // Trial times.
    int attempts = 0;
    
    // Success flag.
    bool didSucceedReading = false;
    
    // Read DHT sensor.
    while (true) {
        if (readSensor(&humidity, &temperature) == false) {
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
	
	return 0;
}
