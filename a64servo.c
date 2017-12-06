
#define DRIVER_AUTHOR "Olimex Ltd. - Chris Boudacoff <chris@protonic.co.uk>"
#define DRIVER_DESC "a64-olinuxino software pwm driver for servos"

#include <linux/kernel.h>	/* We're doing kernel work */
#include <linux/module.h>	/* Specifically, a module */
#include <linux/fs.h>
#include <asm/uaccess.h>	/* for put_user */
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/io.h>
#include <linux/timer.h>
#include <linux/slab.h>
#include "a64servo.h"
#define SUCCESS 0
#define DEVICE_NAME DEVICE_FILE_NAME
#define BUF_LEN 512


static void a64servo_exit(void);
static int a64servo_init(void);
static long device_ioctl(struct file *f, unsigned int ioctl_num, unsigned long ioctl_param);
static int device_open(struct inode *inode, struct file *file);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *file, char __user * buffer,	size_t length, loff_t * offset);
static ssize_t device_write(struct file *file, const char __user * buffer, size_t length, loff_t * offset);

static void __iomem *pe_base;
static uint32_t porte;
static uint32_t porte_m;



static struct timer_list ch_start_timer;

static struct hrtimer ch0_timer;
static struct hrtimer ch1_timer;
static struct hrtimer ch2_timer;
static struct hrtimer ch3_timer;
static struct hrtimer ch4_timer;
static struct hrtimer ch5_timer;
static struct hrtimer ch6_timer;
static struct hrtimer ch7_timer;
static struct hrtimer ch8_timer;
static struct hrtimer ch9_timer;
static struct hrtimer ch10_timer;
static struct hrtimer ch11_timer;
static struct hrtimer ch12_timer;
static struct hrtimer ch13_timer;
static struct hrtimer ch14_timer;
static struct hrtimer ch15_timer;

//set to 0 = 1 000 000 nS
// min = 1000uS, max = 2000uS
static unsigned long ch_value[16] = {1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000,
									 1000000,1000000,1000000,1000000,1000000,1000000,1000000,1000000};

/* 
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions. 
 */
struct file_operations fops = {
	.read = device_read,
	.write = device_write,
	.unlocked_ioctl = device_ioctl,
	.open = device_open,
	.release = device_release,	/* a.k.a. close */
};
/* 
 * Is the device open right now? Used to prevent
 * concurent access into the same device 
 */
static int Device_Open = 0;
static int Major = MAJOR_NUM;	
/* 
 * The message the device will give when asked 
 */
static char Message[BUF_LEN];
static char Request[BUF_LEN];
/* 
 * How far did the process reading the message get?
 * Useful if the message is larger than the size of the
 * buffer we get to fill in device_read. 
 */
static char *Message_Ptr;
static char *Request_Ptr;
// Set enabled channel to LOW after timer expiried.
enum hrtimer_restart ch0_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<0);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;

}
enum hrtimer_restart ch1_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<1);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch2_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<2);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART; 
}
enum hrtimer_restart ch3_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<3);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch4_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<4);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch5_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<5);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART; 
}
enum hrtimer_restart ch6_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<6);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;	 
}
enum hrtimer_restart ch7_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<7);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch8_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<8);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch9_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<9);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch10_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<10);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch11_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<11);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch12_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<12);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch13_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<13);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch14_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<14);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}
enum hrtimer_restart ch15_timer_callback( struct hrtimer *timer )
{
		porte=readl(pe_base+PE_DATA) & ~(1<<15);
		writel(porte, pe_base + PE_DATA); 
		return HRTIMER_NORESTART;
}




void ch_start_timer_callback( unsigned long data )
{

 ktime_t ktime;
     /* do your timer stuff here */
   /* setup timer interval to 20 msecs */
  
		mod_timer(&ch_start_timer, jiffies + msecs_to_jiffies(20));  
// set enabled channels to HIGH  
		porte=readl(pe_base+PE_DATA)|porte_m;
		writel(porte, pe_base + PE_DATA); // start 1 ms impulse
// if channel is enabled start one shot timer
if (porte_m && (1<<0))
{
		ktime = ktime_set( 0, ch_value[0] );
        hrtimer_start( &ch0_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<1))
{
        ktime = ktime_set( 0, ch_value[1] );
        hrtimer_start( &ch1_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<2))
{
		ktime = ktime_set( 0, ch_value[2] );
        hrtimer_start( &ch2_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<3))
{
		ktime = ktime_set( 0, ch_value[3] );
        hrtimer_start( &ch3_timer, ktime, HRTIMER_MODE_REL );		

}
if (porte_m && (1<<4))
{		ktime = ktime_set( 0, ch_value[4] );
        hrtimer_start( &ch4_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<5))
{
		ktime = ktime_set( 0, ch_value[5] );
        hrtimer_start( &ch5_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<6))
{ 
        ktime = ktime_set( 0, ch_value[6] );
        hrtimer_start( &ch6_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<7))
{
		ktime = ktime_set( 0, ch_value[7] );
        hrtimer_start( &ch7_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<8))
{
		ktime = ktime_set( 0, ch_value[8] );
        hrtimer_start( &ch8_timer, ktime, HRTIMER_MODE_REL );		
}
if (porte_m && (1<<9))
{
		ktime = ktime_set( 0, ch_value[9] );
        hrtimer_start( &ch9_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<10))
{
		ktime = ktime_set( 0, ch_value[10] );
        hrtimer_start( &ch10_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<11))
{ 
        ktime = ktime_set( 0, ch_value[11] );
        hrtimer_start( &ch11_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<12))
{
		ktime = ktime_set( 0, ch_value[12] );
        hrtimer_start( &ch12_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<13))
{
		ktime = ktime_set( 0, ch_value[13] );
        hrtimer_start( &ch13_timer, ktime, HRTIMER_MODE_REL );		
}
if (porte_m && (1<<14))
{
		ktime = ktime_set( 0, ch_value[14] );
        hrtimer_start( &ch14_timer, ktime, HRTIMER_MODE_REL );
}
if (porte_m && (1<<15))
{
		ktime = ktime_set( 0, ch_value[15] );
        hrtimer_start( &ch15_timer, ktime, HRTIMER_MODE_REL );		
}		

}


/* 
 * This is called whenever a process attempts to open the device file 
 */
static int device_open(struct inode *inode, struct file *file)
{

	/* 
	 * We don't want to talk to two processes at the same time 
	 */
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	
	/*
	 * Initialize the message 
	 */
	porte=readl(pe_base+PE_DATA);

	sprintf(Message, "00:%03lu,01:%03lu,02:%03lu,03:%03lu,04:%03lu,05:%03lu,06:%03lu,07:%03lu,"
							"08:%03lu,09:%03lu,10:%03lu,11:%03lu,12:%03lu,13:%03lu,14:%03lu,15:%03lu\r",
	 ch_value[0]/1000-1000,ch_value[1]/1000-1000,ch_value[2]/1000-1000,ch_value[3]/1000-1000,ch_value[4]/1000-1000,
	 ch_value[5]/1000-1000,ch_value[6]/1000-1000,ch_value[7]/1000-1000,ch_value[8]/1000-1000,ch_value[9]/1000-1000,
	 ch_value[10]/1000-1000,ch_value[11]/1000-1000,ch_value[12]/1000-1000,ch_value[13]/1000-1000,ch_value[14]/1000-1000,ch_value[15]/1000-1000);
	Message_Ptr = Message;
	Request_Ptr = Request;
	try_module_get(THIS_MODULE);
	return SUCCESS;
}

static int device_release(struct inode *inode, struct file *file)
{
	/* 
	 * We're now ready for our next caller 
	 */
	Device_Open--;
	module_put(THIS_MODULE);
	return SUCCESS;
}

/* 
 * This function is called whenever a process which has already opened the
 * device file attempts to read from it.
 */
static ssize_t device_read(struct file *file,	/* see include/linux/fs.h   */
			   char __user * buffer,	/* buffer to be
							 * filled with data */
			   size_t length,	/* length of the buffer     */
			   loff_t * offset)
{
	/* 
	 * Number of bytes actually written to the buffer 
	 */
	int bytes_read = 0;

	/* 
	 * If we're at the end of the message, return 0
	 * (which signifies end of file) 
	 */
	if (*Message_Ptr == 0)
		return 0;

	/* 
	 * Actually put the data into the buffer 
	 */
	while (length && *Message_Ptr) {

		/* 
		 * Because the buffer is in the user data segment,
		 * not the kernel data segment, assignment wouldn't
		 * work. Instead, we have to use put_user which
		 * copies data from the kernel data segment to the
		 * user data segment. 
		 */
		put_user(*(Message_Ptr++), buffer++);
		length--;
		bytes_read++;
	}


	/* 
	 * Read functions are supposed to return the number
	 * of bytes actually inserted into the buffer 
	 */
	return bytes_read;
}

/* 
 * This function is called when somebody tries to
 * write into our device file. 
 */
static ssize_t
device_write(struct file *file,
	     const char __user * buffer, size_t length, loff_t * offset)
{
    int value;
    int ch;
    char *token = Request;
    char *chn = Request;
	int i;
	
	
	for (i = 0; i < length && i < BUF_LEN-2; i++)
		get_user(Request[i], buffer + i);
		
		
		Request_Ptr = Request;
			
			
		while((chn = strsep(&Request_Ptr,":")) != NULL)
		{
			token = strsep(&Request_Ptr,",");
			
	
			 if(kstrtoint(chn, 10, &ch) == 0)
					{
						ch &= 0x0f;
						if(kstrtoint(token, 10, &value) == 0)
						{
							if ((value>0) && (value < 1000)){
								 ch_value[ch]=(value+1000)*1000;
								 porte_m |= (1<<ch);
							}	 
							else
							{
							//disable pin
							porte_m &= ~(1<<ch);
							ch_value[ch] = 1000000;	
							}
						}	
					}
					
			
	
		}
				
	/* 
	 * Again, return the number of input characters used 
	 */
	return i;
}

/* 
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
 static long device_ioctl(struct file *file, unsigned int ioctl_num, unsigned long ioctl_param)
{
	int i;
	char *temp;
	char ch;

	/* 
	 * Switch according to the ioctl called 
	 */
	switch (ioctl_num) {
	case IOCTL_SET_MSG:
		/* 
		 * Receive a pointer to a message (in user space) and set that
		 * to be the device's message.  Get the parameter given to 
		 * ioctl by the process. 
		 */
		temp = (char *)ioctl_param;

		/* 
		 * Find the length of the message 
		 */
		get_user(ch, temp);
		for (i = 0; ch && i < BUF_LEN; i++, temp++)
			get_user(ch, temp);

		device_write(file, (char *)ioctl_param, i, 0);
		break;

	case IOCTL_GET_MSG:
		/* 
		 * Give the current message to the calling process - 
		 * the parameter we got is a pointer, fill it. 
		 */
		i = device_read(file, (char *)ioctl_param, 99, 0);

		/* 
		 * Put a zero at the end of the buffer, so it will be 
		 * properly terminated 
		 */
		put_user('\0', (char *)ioctl_param + i);
		break;

	case IOCTL_GET_NTH_BYTE:
		/* 
		 * This ioctl is both input (ioctl_param) and 
		 * output (the return value of this function) 
		 */
		return Message[ioctl_param];
		break;
	}

	return SUCCESS;
}

/* Module Declarations */



/* 
 * Initialize the module - Register the character device 
 */
static int __init a64servo_init(void)
{

	/* 
	 * Register the character device (atleast try) 
	 */
	Major = register_chrdev(MAJOR_NUM, DEVICE_NAME, &fops);

	/* 
	 * Negative values signify an error 
	 */

	if (Major < 0) {
	  printk(KERN_ALERT "SERVO: Registering char device failed with %d\n", Major);
	  return Major;
	}
	
//Atach 20mS timer interrupt	
  /* setup your timer to call my_timer_callback */
  setup_timer(&ch_start_timer, ch_start_timer_callback, 1);

  hrtimer_init( &ch0_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch0_timer.function = &ch0_timer_callback;
  hrtimer_init( &ch1_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch1_timer.function = &ch1_timer_callback;
  hrtimer_init( &ch2_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch2_timer.function = &ch2_timer_callback;
  hrtimer_init( &ch3_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch3_timer.function = &ch3_timer_callback;
  hrtimer_init( &ch4_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch4_timer.function = &ch4_timer_callback;
  hrtimer_init( &ch5_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch5_timer.function = &ch5_timer_callback;
  hrtimer_init( &ch6_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch6_timer.function = &ch6_timer_callback;
  hrtimer_init( &ch7_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch7_timer.function = &ch7_timer_callback;
  hrtimer_init( &ch8_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch8_timer.function = &ch8_timer_callback;
  hrtimer_init( &ch9_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch9_timer.function = &ch9_timer_callback;
  hrtimer_init( &ch10_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch10_timer.function = &ch10_timer_callback;
  hrtimer_init( &ch11_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch11_timer.function = &ch11_timer_callback;
  hrtimer_init( &ch12_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch12_timer.function = &ch12_timer_callback;
  hrtimer_init( &ch13_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch13_timer.function = &ch13_timer_callback;
  hrtimer_init( &ch14_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch14_timer.function = &ch14_timer_callback;
  hrtimer_init( &ch15_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
                ch15_timer.function = &ch15_timer_callback;                
                
                
  /* setup timer interval to 20 msecs */
  mod_timer(&ch_start_timer, jiffies + msecs_to_jiffies(20));

//TODO:	
	pe_base = ioremap(SW_PORTE_IO_BASE, 0x20);
	
//PE0-PE15 as outputs.	
	writel(0x11111111, pe_base + PE_CONFIG0);  //PE CONF0 7-0
	writel(0x11111111, pe_base + PE_CONFIG1);  //PE CONF1 15-8
	writel(0x00000000, pe_base + PE_CONFIG2);  //PE CONF2

	porte_m = 0;

	printk(KERN_INFO "SERVO: Driver loaded. To use the driver, create a dev file with\n");
	printk(KERN_INFO "SERVO: 'mknod /dev/%s c %d 0'.\n", DEVICE_NAME, MAJOR_NUM);


	return 0;
}

/* 
 * Cleanup - unregister the appropriate file from /proc 
 */
static void __exit a64servo_exit(void)
{
	/* 
	 * Unregister the device 
	 */

	del_timer(&ch_start_timer);
	hrtimer_cancel( &ch0_timer );
	hrtimer_cancel( &ch1_timer );
	hrtimer_cancel( &ch2_timer );
	hrtimer_cancel( &ch3_timer );
	hrtimer_cancel( &ch4_timer );
	hrtimer_cancel( &ch5_timer );
	hrtimer_cancel( &ch6_timer );
	hrtimer_cancel( &ch7_timer );
	hrtimer_cancel( &ch8_timer );
	hrtimer_cancel( &ch9_timer );
	hrtimer_cancel( &ch10_timer );
	hrtimer_cancel( &ch11_timer );
	hrtimer_cancel( &ch12_timer );
	hrtimer_cancel( &ch13_timer );
	hrtimer_cancel( &ch14_timer );
	hrtimer_cancel( &ch15_timer );	
				
	iounmap(pe_base); //free memory
	unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
	printk(KERN_INFO "SERVO: unregister_chrdev /dev/servo\n");
}

module_init(a64servo_init);
module_exit(a64servo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);	
MODULE_DESCRIPTION(DRIVER_DESC);	
MODULE_SUPPORTED_DEVICE("servo");
