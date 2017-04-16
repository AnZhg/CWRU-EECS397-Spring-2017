// Created by Zhigang An on Apr. 15, 2017.
// Case Western Reserve university
// 
// Compile: make
// Insert module: sudo insmod THSensor_km.ko
// See the info printed: sudo tail -f /var/log/kern.log
// Remove module: sudo rmmod THSensor_km.ko
//
// Read DHT11 sensor.

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>
#include <linux/timer.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/interrupt.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zhigang An");
MODULE_DESCRIPTION("Read temperature and humidity from DHT11 sensor.");
MODULE_VERSION("1.0");

#define DHT_PIN	2

#define HIGH	1
#define LOW		0

#define IRQF_TRIGGER_EDGE	(IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING)

static unsigned int irqNumber;
static unsigned char data[40];

// Timers.
static struct timer_list lpTimer;	// Millisecond precision timer.
static struct hrtimer hrTimer;		// At least microsecond precision timer.

static void convertData(void)
{
	unsigned char humidity, decHumidity, temperature, decTemperature, checksum;
	unsigned char lowEightBits;
	
	int i = 0;
	// Integer part of (relative) humidity.
	for (; i < 8; ++i) {
		humidity <<= 1;
		humidity |= data[i];
	}
	
	// Decimal part of (relative) humidity.
	for (; i < 16; ++i) {
		decHumidity <<= 1;
		decHumidity |= data[i];
	}
	
	// Integer part of temperaturee.
	for (; i < 24; ++i) {
		temperature <<= 1;
		temperature |= data[i];
	}
	
	// Decimal part of temperature.
	for (; i < 32; ++i) {
		decTemperature <<= 1;
		decTemperature |= data[i];
	}
	
	// Checksum.
	for (; i < 40; ++i) {
		checksum <<= 1;
		checksum |= data[i];
	}
	
	lowEightBits = humidity + decHumidity + temperature + decTemperature;
	
	printk(KERN_INFO "Temperature: %i.%i C. Humidity: %i.%i%%.\n", temperature, decTemperature, humidity, decHumidity);
	printk(KERN_INFO "%s\n", (lowEightBits == checksum) ? "Correct." : "Wrong.");
}

// Function declaration.
enum hrtimer_restart hrTimerCallback(struct hrtimer *timer);
static irq_handler_t interruptHandler(unsigned int irq, void *dev_id, struct pt_regs *regs);

// Timer callback.
void lpTimerCallback(unsigned long data)
{
	// Variable declaration.
	ktime_t ktime;
	
	// Pull high.
	gpio_set_value(DHT_PIN, HIGH);
	
	// Setup high precision timer. Delay 20 us.
	ktime = ktime_set(0, 20L * 1e3L);	// 20 us = 20 * 10 ^ 3 ns.
	hrtimer_init(&hrTimer, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	hrTimer.function = &hrTimerCallback;
	hrtimer_start(&hrTimer, ktime, HRTIMER_MODE_REL);
}

// High resolution timer callback.
enum hrtimer_restart hrTimerCallback(struct hrtimer *timer)
{
	// Variable declaration.
	int result = 0;
	
	// Pull low.
	gpio_set_value(DHT_PIN, LOW);
	printk(KERN_INFO "THSensor_km: Sensor initial communication finished.\n");
	
	// Register interrupt.
	// Store IRQ number for later use.
	irqNumber = gpio_to_irq(DHT_PIN);
	// Request an interrupt and register handeler function.
	result = request_irq(irqNumber, (irq_handler_t)interruptHandler, IRQF_TRIGGER_EDGE, "THSensor_km", NULL);
	printk(KERN_INFO "THSensor_km: Interruption registered. IRQ number is %i.\n", irqNumber);
	
	// Cancel low precision timer.
	if (del_timer(&lpTimer)) {
		printk("Failed to cancel timer.\n");
	}
	
	// Set DHT pin direction as input.
	gpio_direction_input(DHT_PIN);
	// Make DHT pin visible in /sys/class/gpio. Prevent direction from changing (false).
	gpio_export(DHT_PIN, false);
	
	// Do not restart timer.
	return HRTIMER_NORESTART;
}

// GPIO interrupt Handler.
// Note that the same interrupt handler cannot be invoked concurrently,
// as the interrupt line is masked out until the function is complete.
// Input: 
//     irq    IRQ number.
//     dev_id device ID.
//     regs   hardware specific register values. (For debugging)
// Output:
//     Returns IRQ_HANDLED if successful, IRQ_NONE if failed.
static irq_handler_t interruptHandler(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
	// Data counter.
	static int i = 0;
	static struct timeval prevTimeVal;
	static struct timeval currTimeVal;
	
	// Get value to determine rising/falling edge.
	if (gpio_get_value(DHT_PIN) == HIGH) {
		// Rising edge.
		do_gettimeofday(&prevTimeVal);
	} else {
		// falling edge.
		do_gettimeofday(&currTimeVal);
		
		// Low: 26 - 28 us.
		// High: 70 us.
		// (28 + 70) / 2 = 49 us.
		data[i++] = ((currTimeVal.tv_sec - prevTimeVal.tv_sec) * 1000000L + currTimeVal.tv_usec) - prevTimeVal.tv_usec > 49;
	}
	
	// When data acquisation is done, start conversion.
	convertData();
	
	// Tell caller that interrupt has been handled correctly.
	return (irq_handler_t)IRQ_HANDLED;
}

static int __init initialization(void)
{
	// Declare variable at the very beginning of a scope to silence warning.
	int result = 0;
	
	// Check whether the GPIO is valid.
	if (!gpio_is_valid(DHT_PIN)){
		printk(KERN_INFO "THSensor_km: Initialization failed. Invalid LED GPIO.\n");
		return -ENODEV;
	}
	
	// Request GPIO pin.
	gpio_request(DHT_PIN, "sysfs");
	
	// Set direction as output, and value is high voltage.
	gpio_direction_output(DHT_PIN, HIGH);
	// Make LED GPIO visible in /sys/class/gpio. Enable direction change (true).
	gpio_export(DHT_PIN, true);
	
	// Pull low.
	gpio_set_value(DHT_PIN, LOW);
	
	// my_timer.function, my_timer.data
	setup_timer(&lpTimer, lpTimerCallback, 0);
	
	// Setup timer.
	// Delay 20 ms.
	result = mod_timer(&lpTimer, jiffies + msecs_to_jiffies(20));
	if (result) {
		printk("THSensor_km: Start timer failed.\n");
	}
	
	printk(KERN_INFO "THSensor_km: Initialization succeeded.");
	return result;
}

static void __exit finalization(void)
{
	if (hrtimer_cancel(&hrTimer)) {
		printk("THSensor_km: Failed to cancel high resolution timer.\n");
	}
	
	// Free interrupt.
	free_irq(irqNumber, NULL);
	
	// Unexport DHT GPIO.
	gpio_unexport(DHT_PIN);
	// Free DHT GPIO.
	gpio_free(DHT_PIN);
	
	// Message shown in kern.log.
	printk(KERN_INFO "THSensor_km: Exit.\n");
}

// Specify initialization and finalization function.
module_init(initialization);
module_exit(finalization);
