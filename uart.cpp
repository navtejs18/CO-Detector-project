#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>
#include<wiringPi.h>
#include<stdlib.h>
#define 	DELAY_TIME	50000000	
using namespace std;



void my_delay_func(long DELAY)
{
	for (long jj=0; jj <= DELAY; jj++);
}


int function()
{
  
   int file, count;
   char buf[10];
   char msg_cmd0[100] = "AT";
   char msg_cmd1[100] = "AT+CSCA=\"+15149931123\"";
   char msg_cmd2[100] = "AT+CMGS=\"+14377796894\"";
   char msg[100]="CO is reached at dangerous level";
   char temp = 0;
   



   // open UART channel, specify protocol
  // printf("opening communication channel...\n\r");
   if ((file = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY))<0){
      perror("UART: Failed to open the device.\n\r");
      return -1;
   }
   struct termios options;
   tcgetattr(file, &options);
   options.c_cflag = B115200 | CS8 | CREAD | CLOCAL;
   options.c_iflag = IGNPAR | ICRNL;
   tcflush(file, TCIFLUSH);
   tcsetattr(file, TCSANOW, &options);

   // send mesg cmd0
   //printf("sending AT for OK confirmation... \n\r");
   if ((count = write(file,msg_cmd0,strlen(msg_cmd0)))<0){
	perror("UART: failed to get OK confirmation\n\r");
	return -1;
   }
    write(file, "\r", 1);
  my_delay_func(DELAY_TIME);


   // send msg cmd1 to gsm module: (open message centre)
   printf("sending first command message...\n\r");
   if ((count = write(file,msg_cmd1,strlen(msg_cmd1)))<0){
      perror("UART: Failed to write to the output\n");
      return -1;
   }
   write(file, "\r", 1);
   my_delay_func(DELAY_TIME);

  

 // send msg cmd2 to gsm module (specifies recipient)
  // printf("sending second command message...\n\r"); 
   if ((count = write(file,msg_cmd2,strlen(msg_cmd2)))<0){
      perror("UART: Failed to write to the output\n");
      return -1;
   }
   write(file, "\r", 1);
   my_delay_func(DELAY_TIME);

 // send the actual msg 
   printf("sending the actual message...\n\r"); 

	temp = strlen(msg);
	msg[temp] = (char) 26;

	msg[temp+1] = '\0';
	printf("%s",msg);
    if ((count = write(file,msg,strlen(msg)))<0){
	    perror("UART: Failed to write to the output\n");
	   return -1;
}


 close(file);
   
   return 0;

}

