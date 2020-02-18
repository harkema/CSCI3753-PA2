#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>  

#define BUFF_SIZE 1024
int main()
{
  bool keepRunning = true;
  char command;
  int bytesToRead;
  int whence;
  int file = open("/dev/pa2_character_device",  O_RDWR);
  char buffer[BUFF_SIZE];
  int offset;

  while(keepRunning)
  {
    printf("Press r to read from device\n");
  	printf("Press w to write to the device\n");
  	printf("Press s to seek into the device\n");
  	printf("Press e to exit from the device\n");
  	printf("Press anything else to keep reading or writing from the device\n");
    printf("Enter Command:\n");

    command = getchar();

    if(command == 'r')
    {
      printf("Enter the number of bytes you want to read: \n");
      scanf("%d", &bytesToRead);
      read(file, buffer, bytesToRead);
      printf("Data read from the device: %s\n", buffer);
      while(getchar() != '\n');

    }
    else if(command == 'w')
    {
      printf("Enter data you want to write to the device: \n");
      scanf("%s", buffer);
      write(file, buffer, BUFF_SIZE);
      while(getchar() != '\n');
    }
    else if(command == 's')
    {
      printf("Enter a whence value (0,1,2): \n");
      scanf("%d", &whence);

      printf("Enter an offset value: \n");
      scanf("%d", &offset);

      llseek(file, offset, whence);

      while(getchar() != '\n');
    }
    else if(command == 'e')
    {
      printf("Exiting program...\n");
      keepRunning = false;
    }
    else
    {
      printf("Error: Command not found. Exiting program...\n");
      keepRunning = false;
    }
  }

  close(file);
  return 0;
}

