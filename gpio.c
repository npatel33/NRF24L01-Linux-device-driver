#include "gpio.h"


struct GpioRegisters *GpioRegisters_s;

/**********************************
 Sets GPIO pin function
***********************************/
void SetGPIOFunction(int GPIO, int functionCode)
{
	int registerIndex = GPIO / 10;
	int bit = (GPIO % 10) * 3;
 
	unsigned oldValue = GpioRegisters_s-> GPFSEL[registerIndex];
	unsigned  mask = 0b111 << bit;
    	printk("Changing function of GPIO%d from %x to %x\n",GPIO,(oldValue >> bit) & 0b111,functionCode);

	GpioRegisters_s-> GPFSEL[registerIndex] = (oldValue & ~mask)|((functionCode << bit) & mask);
}

/******************************
 Set GPIO pin level
******************************/
void SetGPIOOutputValue(int GPIO, bool outputValue)
{
	if (outputValue)
		GpioRegisters_s-> GPSET[GPIO / 32] = (1 << (GPIO % 32));
	else
		GpioRegisters_s-> GPCLR[GPIO / 32] = (1 << (GPIO % 32));
}
