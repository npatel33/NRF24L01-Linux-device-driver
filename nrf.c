#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <mach/platform.h>
#include "nRF24L01.h"
#include "gpio.h"
#include "radio.h"

#define NRF_CE 25 //module chip enable pin

/* NRF registers*/
static const uint8_t  nrf_reg[]={
	CONFIG,
	EN_AA,
	EN_RXADDR,
	SETUP_AW,
	SETUP_RETR,
	RF_CH,
	RF_SETUP,
	STATUS,
	OBSERVE_TX,
	RPD,
	FIFO_STATUS
};

static char *nrf_reg_name[]={
	"CONFIG",
	"EN_AA",
	"EN_RXADDR",
	"SETUP_AW",
	"SETUP_RETR",
	"RF_CH",
	"RF_SETUP",
	"STATUS",
	"OBSERVE_TX",
	"RPD",
	"FIFO_STATUS"
};


unsigned int address[5]={0x00,0x00,0x00,0x00,0x00};

static dev_t dev;

static struct cdev *nrf_cdev;
static struct class* nrf_class = NULL;
static struct device* nrf_device = NULL;
struct spi_device *spi_device;

/******************************
 nrf module file operations
******************************/

static int nrf_open(struct inode *inode, struct file *fp)
{
	
	return 0;
}

static int nrf_release(struct inode *inode,struct file *fp)
{
	printk(KERN_INFO "device successfully closed\n");
	return 0;
}

static ssize_t nrf_write(struct file *fp, const char *data, size_t len, loff_t *offset)
{
	printk(KERN_INFO "%d bytes written!!\n",len);
	return len;
}


/* Displays NRF module register info*/
static ssize_t nrf_read(struct file *fp, char *buf, size_t len, loff_t *offset){
	
	uint8_t *ret;
	char s[64];
	int loop;
	int count=0;
	
	/*avoid repeated read*/
	if(*offset > 0)
		goto out;
	
	for(loop = 0; loop < 11; loop++){

		//ret = spi_w8r8(spi_device,nrf_reg[loop]);
		ret = nrf_xfer(nrf_reg[loop],1,NULL,R);
		mdelay(10);
		sprintf(s,"%s = 0x%02x\n",nrf_reg_name[loop],ret[0]);
		if(copy_to_user(buf+count,s,strlen(s))){
			return -EFAULT;
		}
		*offset += strlen(s);	
		count+= strlen(s);
	}
	return count;
	
	out:
	return 0;
}

struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = nrf_open,
	.release = nrf_release,
	.read = nrf_read,
	.write = nrf_write,
};

/******************************
 creates device class and device node
******************************/
static int reg_dev(void)
{
	int retval=0;

	retval = alloc_chrdev_region(&dev,0,1,"nrf");
	if(retval != 0){
		printk(KERN_ALERT "device registration failed!!\n");
		goto out;
	}else{
		printk(KERN_INFO "device registration is successful!!\n");	
	}

	nrf_cdev = cdev_alloc();
	cdev_init(nrf_cdev,&fops);
	nrf_cdev -> owner = THIS_MODULE;

	retval = cdev_add(nrf_cdev,MKDEV(MAJOR(dev),0),1);
	if(retval < 0){
		printk(KERN_ALERT "adding to cdev is failed!!\n");
		goto out;
	}

	nrf_class = class_create(THIS_MODULE,"radio");
	if(IS_ERR(nrf_class)){
		printk(KERN_ALERT "adding to class is failed!!\n");	
		retval = -1;
		goto out;
	}
	printk(KERN_INFO "device class is registerd!!\n");	

	nrf_device = device_create(nrf_class,NULL,MKDEV(MAJOR(dev),0),NULL,"nrf");
	if(IS_ERR(nrf_device)){
		printk(KERN_ALERT "registering device is failed!!\n");	
		retval = -ENODEV;
		goto out;
	}

	printk(KERN_INFO "device is registerd!!\n");	
	
	return 0;

	out:
	return retval;
}

/******************************
 register spi device and attach it to Master driver
******************************/
static int reg_spi_device(void)
{
	int retval = 0;
	
	struct spi_board_info spi_device_info = {
		.modalias = "nrf24l01+",
		.max_speed_hz = 5000000,
		.bus_num = 0,
		.chip_select = 1,
		.mode = 0,
	};

	struct spi_master *master;
	
	master = spi_busnum_to_master(spi_device_info.bus_num);
	if(!master){
		printk(KERN_ALERT "getting master device is failed!!\n");	
		retval = -ENODEV;
		goto out;
	}
	
	spi_device = spi_new_device(master,&spi_device_info);
	if(!spi_device){
		printk(KERN_ALERT "registering spi device is failed!!\n");	
		retval = -ENODEV;
		goto out;
	}
	
	spi_device->bits_per_word = 8;

	retval = spi_setup(spi_device);
	if(retval){
		spi_unregister_device(spi_device);
		goto out;
	}

	return 0;

	out:
	return retval;
}

/******************************
 Module initialization function
******************************/
static int __init nrf_init(void)
{
	int retval = 0;

	printk(KERN_INFO "hello from module!!\n");
	
	retval = reg_dev();
	if(retval != 0)
		goto out;
	
	retval = reg_spi_device();
	if(retval != 0)
		goto out;


	radio_init();

	return 0;
	
	out:
	return retval;
}

/******************************
 Module exit function
******************************/
static void __exit nrf_exit(void)
{
	printk(KERN_ALERT "Bye from module!!\n");
	nrf_power_down();
	spi_unregister_device(spi_device);
	device_destroy(nrf_class,MKDEV(MAJOR(dev),0));
	class_unregister(nrf_class);
	class_destroy(nrf_class);
	cdev_del(nrf_cdev);
	unregister_chrdev_region(MKDEV(MAJOR(dev),0),1);	
}


module_init(nrf_init);
module_exit(nrf_exit);
module_param_array(address,uint,NULL,0);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nisarg Patel");
MODULE_VERSION("v0.1");
MODULE_DESCRIPTION("Driver for 2.4 Ghz RF module nrf24L01+");
