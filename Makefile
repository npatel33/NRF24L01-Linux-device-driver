ifneq ($(KERNELRELEASE),)
	obj-m += nrf24.o
	nrf24-objs := nrf.o gpio.o radio.o

else
	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
	PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm *.mod.c *.o *.ko *.symvers *.order 
endif
