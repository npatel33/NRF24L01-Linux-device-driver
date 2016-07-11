#ifndef _GPIO_H
#define _GPIO_H

#include <linux/init.h>
#include <linux/module.h>


/* Raspberry pi 2 model base address */
#define BCM2708_PERI_BASE    0x3F000000
#define GPIO_BASE            (BCM2708_PERI_BASE + 0x200000) /* GPIO controller */

#define NRF_CE 25 //module chip enable pin

/**********************************
 GPIO register structure for raspberry pi
 Note:- for other processors gpio structure may defer.
***********************************/
struct GpioRegisters{
	uint32_t GPFSEL[6];
	uint32_t Reserved1;
	uint32_t GPSET[2];
	uint32_t Reserved2;
	uint32_t GPCLR[2];
};

void SetGPIOFunction(int GPIO, int functionCode);
void SetGPIOOutputValue(int GPIO, bool outputValue);

#endif
