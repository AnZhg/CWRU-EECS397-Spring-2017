// Created by ZHigang An on Mar. 13, 2017.
// Case western Reserve University
//
// Compile: gcc Blink_file.c -o Blink_file
// Run: sudo ./Blink_file
// Blink a LED by accessing system file diretly.

#include <stdio.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

#include <fcntl.h>
#include <unistd.h>

#define IN   0
#define OUT  1

#define LOW  0
#define HIGH 1

// GPIO16.
#define PIN 16

int GPIOExport(int pin)
{
#define BUFFER_MAX 3
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;
	
    fd = open("/sys/class/gpio/export", O_WRONLY);
    if (fd == -1) {
		fprintf(stderr, "Failed to open export for writing.\n");
		return -1;
    }
	
    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    
    return 0;
}

int GPIOUnexport(int pin)
{
    char buffer[BUFFER_MAX];
    ssize_t bytes_written;
    int fd;
	
    fd = open("/sys/class/gpio/unexport", O_WRONLY);
    if (fd == -1) {
		fprintf(stderr, "Failed to open unexport for writing.\n");
		return -1;
    }
	
    bytes_written = snprintf(buffer, BUFFER_MAX, "%d", pin);
    write(fd, buffer, bytes_written);
    close(fd);
    
    return 0;
}

int GPIODirection(int pin, int dir)
{
    static const char s_directions_str[]  = "in\0out";
	
#define DIRECTION_MAX 35
    char path[DIRECTION_MAX];
    int fd;
	
    snprintf(path, DIRECTION_MAX, "/sys/class/gpio/gpio%d/direction", pin);
    fd = open(path, O_WRONLY);
    if (-1 == fd) {
		fprintf(stderr, "Failed to open gpio direction for writing.\n");
		return -1;
    }
	
    if (write(fd, &s_directions_str[IN == dir ? 0 : 3], IN == dir ? 2 : 3) == -1) {
		fprintf(stderr, "Failed to set direction.\n");
		return -1;
    }
	
    close(fd);
    
    return 0;
}

int GPIORead(int pin)
{
#define VALUE_MAX 30
    char path[VALUE_MAX];
    char value_str[3];
    int fd;
	
    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    fd = open(path, O_RDONLY);
    if (fd == -1) {
		fprintf(stderr, "Failed to open gpio value for reading.\n");
		return -1;
    }
	
    if (read(fd, value_str, 3) == -1) {
		fprintf(stderr, "Failed to read value.\n");
		return -1;
    }
	
    close(fd);
	
    return atoi(value_str);
}

int GPIOWrite(int pin, int value)
{
	static const char values_str[] = "01";
	
    char path[VALUE_MAX];
    int fd;
	
	// Get the whole string.
    snprintf(path, VALUE_MAX, "/sys/class/gpio/gpio%d/value", pin);
    
    // Open the file.
    fd = open(path, O_WRONLY);
    
    if (fd == -1) {
		fprintf(stderr, "Failed to open gpio value for writing.\n");
		return -1;
    }
	
    if (write(fd, &values_str[value ? 1 : 0], 1) != 1) {
		fprintf(stderr, "Failed to write value.\n");
		return -1;
    }
	
    close(fd);
    
    return 0;
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;
	
    int repeat = 9;
	
    // Enable GPIO pins.
    if (GPIOExport(PIN) == -1) {
		fprintf(stderr, "Failed to export GPIO.\n");
		return -1;
	}
	
    // Set GPIO directions.
    if (GPIODirection(PIN, OUT) == -1) {
		fprintf(stderr, "Failed to set GPIO direction.\n");
		return -1;
	}
	
    do {
		// Write GPIO value.
		if (GPIOWrite(PIN, repeat % 2) == -1) {
			fprintf(stderr, "Failed to write to GPIO.\n");
			return -1;
		}
		
		// Delay 500 ms.
		usleep(500 * 1000);
    } while (--repeat);
	
    // Disable GPIO pins.
    if (GPIOUnexport(PIN) == -1) {
		fprintf(stderr, "Failed to unexport GPIO.\n");
		return -1;
	}
	
    return 0;
}