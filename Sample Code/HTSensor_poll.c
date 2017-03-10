// Create by AnZhg on Feb. 15, 2017.
// Case Western Reserve University
//
// Required Library: wiringPi
// Compile: gcc -Os HTSensor_poll.c -lwiringPi -o HTSensor_poll
// Using polling method to read DHT sensor.

#include <wiringPi.h> 
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define MAX_TIME 85
int DHT11PIN;
int dht11_val[5] = { 0, 0, 0, 0, 0 };
bool isinit = false;
bool InitDHT(int pinval);

float getTemp();
float getHumidity();
bool dht11_read_val();
bool InitDHT(int pinval)
{
       if (wiringPiSetup() == -1)
       {
              isinit = false;
              return isinit;
       }
       DHT11PIN = pinval;
       // initialize pin
 

       isinit = true;
       return isinit;
}
 

float getTemp()
{
       return (float)(dht11_val[2] + dht11_val[3] / 10);
}
 

float getHumidity()
{
       return (float)(dht11_val[0] + dht11_val[1] / 10);
}
 

bool dht11_read_val()
{
       if (!isinit)
              return false;
       uint8_t lststate = HIGH;
       uint8_t counter = 0;
       uint8_t j = 0, i;
       float farenheit;
       for (i = 0; i < 5; i++)
              dht11_val[i] = 0;
       pinMode(DHT11PIN, OUTPUT);
       digitalWrite(DHT11PIN, LOW);
       delay(18);
       digitalWrite(DHT11PIN, HIGH);
       delayMicroseconds(40);
       pinMode(DHT11PIN, INPUT);
       for (i = 0; i < MAX_TIME; i++)
       {
              counter = 0;
              while (digitalRead(DHT11PIN) == lststate){
                     counter++;
                     delayMicroseconds(1);
                     if (counter == 255)
                           break;
              }
              lststate = digitalRead(DHT11PIN);
              if (counter == 255)
                     break;
              // top 3 transistions are ignored 
              if ((i >= 4) && (i % 2 == 0)){
                     dht11_val[j / 8] <<= 1;
                     if (counter>16)
                           dht11_val[j / 8] |= 1;
                     j++;
              }
       }
       // verify cheksum and print the verified data 
       if ((j >= 40) && (dht11_val[4] == ((dht11_val[0] + dht11_val[1] + dht11_val[2] + dht11_val[3]) & 0xFF)))
       {
              if ((dht11_val[0] == 0) && (dht11_val[2] == 0))
                     return false;
              return true;
       }
       return false;
}

int main()
{
	InitDHT(7);
	
	int counter = 0;
	while(dht11_read_val() == false)
		++counter;
		
	printf("Failed %d times\n", counter);
	printf("T: %f\nH: %f\n", getTemp(), getHumidity());
	
	return 0;
}
