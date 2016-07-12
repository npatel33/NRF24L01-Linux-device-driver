# NRF24L01-Linux-device-driver
Linux kernel module to handle RF communication using nrf24l01+ in kernel space.
<br>

Requirements:-
* This module requires Master SPI driver already enabled in the kernel. If it is not enabled in your kernel then you need to recompile the kernel.
* This module is currently aimed to work on Raspberry Pi 2 but only few lines of code is board dependent. future commits will separate board dependent code.

<b>Note:- for Raspberry Pis, this module won't work on stock kernel because SPIdev is being registered to Master driver and to disable that cross compiling new kernel is necessary.</b>

Features:-
* API to communicate with the module. With API, one can make networking layer glued into the kernel module.
* With ioctl functions, networking can also be moved out of kernelspace to userspace.
<br>


Work to do:-
* Currently aiming to establish simple Network using Multiceiver feature of the module.
* Complete the ioctl functions.

-------My current testing setup--------- <br>
Kernel version: Cross compiled 3.18.16<br>
Board: Raspberry Pi 2<br>
SPI driver: spi-bcm2708
