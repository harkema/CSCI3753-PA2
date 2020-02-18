#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<linux/slab.h>
#include<linux/uaccess.h>

#define BUFFER_SIZE 1024

/* Define device_buffer and other global data structures you will need here */

#define DEVICE_BUFFER 1024
static int timesOpened = 0;
static int timesClosed = 0;
static char *kptr;

ssize_t pa2_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	/* *buffer is the userspace buffer to where you are writing the data you want to be read from the device file*/
	/* length is the length of the userspace buffer*/
	/* offset will be set to current position of the opened file after read*/
	/* copy_to_user function: source is device_buffer and destination is the userspace buffer *buffer */
	
	/*max bytes that can  be read given constraints of source file*/
	int maxBytesFromDevice =  DEVICE_BUFFER - *offset;
	
	/*max bytes that be written given constraints of userspace buffer*/
	int maxBytesToUser = 0;

       int bytesRead = 0;

	/*Check if we're reading from  is greater than the length of the user space where it will be written to*/
       /*Else if maxBytesToRead is  less than the length of the user space, the maximum space to read from remains maxBytesToRead*/ 
	if(maxBytesFromDevice > length)
	{
	 maxBytesToUser =  length;
	}
	else
	{
	 maxBytesToUser = maxBytesFromDevice;
	}

	if(maxBytesToUser == 0)
	{
	printk(KERN_ALERT "Source file is empty - nothing to read");
	}

	bytesRead = maxBytesToUser - copy_to_user(buffer, kptr + *offset,  maxBytesToUser);

	*offset += bytesRead;

	printk(KERN_ALERT "Read %d bytes", bytesRead);
	printk(KERN_ALERT "Offset is now %d", offset); 
	return 0;
}



ssize_t pa2_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/* length is the length of the userspace buffer*/
	/* current position of the opened file*/
	/* copy_from_user function: destination is device_buffer and source is the userspace buffer *buffer */
	 /*max bytes that can  be read given constraints of source file (userspace)*/
        int maxBytesFromUser =  length;

        /*max bytes that be written given constraints of device buffer (kernel)*/
       int maxBytesToDevice = DEVICE_BUFFER - *offset;

       int bytesWritten = 0;

        /*Check if we're reading from  is greater than the length of the device where it will be written to*/
       /*Else if maxBytesToRead is  less than the length of the kernel, the maximum space to read from remains maxBytesToRead*/ 
	if(maxBytesToDevice > length)
        {
         maxBytesFromUser =  length;
        }
        else
        {
         maxBytesFromUser = maxBytesToDevice;
        }


	bytesWritten = maxBytesFromUser - copy_from_user(kptr + *offset, buffer,  maxBytesFromUser);

        *offset += bytesWritten;

        printk(KERN_ALERT "Read %d bytes", bytesWritten);
        printk(KERN_ALERT "Offset is now %d", offset); 
        return 0;

	return length;
}


int pa2_char_driver_open (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	printk(KERN_ALERT "Device has been opened\n");
	timesOpened++;
	printk(KERN_ALERT "Device has been opened %d times\n", timesOpened);
	return 0;
}

int pa2_char_driver_close (struct inode *pinode, struct file *pfile)
{
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	printk(KERN_ALERT "Device has been closed\n");
	timesClosed++;
	printk(KERN_ALERT "Device has been closed %d times\n",  timesClosed);
	return 0;
}

loff_t pa2_char_driver_seek (struct file *pfile, loff_t offset, int whence)
{
	loff_t newOffset = 0;
	/* Update open file position according to the values of offset and whence */
	if(whence == 0)
	{
	 newOffset =  offset;
	}
	else if(whence == 1)
       {
	newOffset = pfile->f_pos + offset;
       }
      else if(whence == 2)
      {
	newOffset =  DEVICE_BUFFER - offset;
      }

     if(newOffset < 0)
    {
     printk(KERN_ALERT "Error: Attempting to seek before start of buffer");
    }

    if(newOffset > DEVICE_BUFFER)
   {
     printk(KERN_ALERT "Error: Attempting to seek after end of buffer");
   }

   pfile->f_pos = newOffset; 

   printk("New Offset is  %d\n", newOffset); 

   return newOffset;
}

struct file_operations pa2_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
 	.read = pa2_char_driver_read,
	.write = pa2_char_driver_write,
	.open = pa2_char_driver_open,
	.release = pa2_char_driver_close,
	.llseek = pa2_char_driver_seek, 


};

static int pa2_char_driver_init(void)
{
	/* print to the log file that the init function is called.*/
	printk(KERN_ALERT "inside %s function\n",__FUNCTION__);	
	/* register the device */
	/*GFP_KERNEL  => safe to sleep*/
	kptr = kmalloc(DEVICE_BUFFER,  GFP_KERNEL);
	register_chrdev(307,"pa2_character_device", &pa2_char_driver_file_operations);
	return 0;
}

static void pa2_char_driver_exit(void)
{
	/* print to the log file that the exit function is called.*/
        printk(KERN_ALERT "inside %s function\n",__FUNCTION__);
	/* unregister  the device using the register_chrdev() function. */
	if(kptr)
	{
		kfree(kptr);
	}
	unregister_chrdev(307, "pa2_character_device");
	
}

/* add module_init and module_exit to point to the corresponding init and exit function*/
module_init(pa2_char_driver_init);
module_exit(pa2_char_driver_exit);
