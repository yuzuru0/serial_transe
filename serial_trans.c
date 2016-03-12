// Fastrek用Uart-Wifi変換 
// Serial Transmitter for Raspberry Pi


#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "read_config.h"

#define UART_DEVICE "/dev/ttyAMA0"
#define UART_SPEED 115200 

typedef struct
{
	int number;
	int flag;
	double pos[6];
}fastrak_data;

fastrak_data position_data[4];

#define	POS_X	0
#define POS_Y	1
#define POS_Z	2
#define POS_A	3
#define POS_B	4
#define POS_C	5

void *thread_uart_comm(void *pParam);
void *thread_inet_comm(void *pParam);

int uart_open(char const *device, int bps, struct termios *saveattr)
{
	int fd;
	int speed;
	struct termios attr;

	fd = open(device, O_RDWR | O_NOCTTY | O_NONBLOCK);
	if (fd < 0) 
	{
		return -1;
	}

	tcgetattr(fd, &attr);
	*saveattr = attr;

	speed = B9600;
	switch (bps) 
	{
//	case      50:	speed =      B50;		break;
//	case      75:	speed =      B75;		break;
//	case     110:	speed =     B110;		break;
//	case     134:	speed =     B134;		break;
//	case     150:	speed =     B150;		break;
// 	case     200:	speed =     B200;		break;
	case     300:	speed =     B300;		break;
	case     600:	speed =     B600;		break;
	case    1200:	speed =    B1200;		break;
//	case    1800:	speed =    B1800;		break;
	case    2400:	speed =    B2400;		break;
	case    4800:	speed =    B4800;		break;
	case    9600:	speed =    B9600;		break;
	case   19200:	speed =   B19200;		break;
	case   38400:	speed =   B38400;		break;
	case   57600:	speed =   B57600;		break;
	case  115200:	speed =  B115200;		break;
	case  230400:	speed =  B230400;		break;
//	case  460800:	speed =  B460800;		break;
//	case  500000:	speed =  B500000;		break;
//	case  576000:	speed =  B576000;		break;
//	case  921600:	speed =  B921600;		break;
//	case 1000000:	speed = B1000000;		break;
//	case 1152000:	speed = B1152000;		break;
//	case 1500000:	speed = B1500000;		break;
//	case 2000000:	speed = B2000000;		break;
//	case 2500000:	speed = B2500000;		break;
//	case 3000000:	speed = B3000000;		break;
//	case 3500000:	speed = B3500000;		break;
//	case 4000000:	speed = B4000000;		break;
	}

	cfsetispeed(&attr, speed);
	cfsetospeed(&attr, speed);
	cfmakeraw(&attr);

	attr.c_cflag |= CS8 | CLOCAL | CREAD;
	attr.c_iflag = 0;
	attr.c_oflag = 0;
	attr.c_lflag = 0;
	attr.c_cc[VMIN] = 1;
	attr.c_cc[VTIME] = 0;

	tcsetattr(fd, TCSANOW, &attr);

	return fd;
}

void uart_close(int fd, struct termios *saveattr)
{
	tcsetattr(fd, TCSANOW, saveattr);
	close(fd);
}

int sdata_split(char data[],fastrak_data *posdata)
{
	int ret = 0,i;
	char number_data[4];
	char split_data[6][8];
	char *endptr;

	posdata->flag =0;
	
	strncpy(number_data,data,3);
	number_data[3] = '\0';

	for(i=0;i<6;i++)
	{
		strncpy(split_data[i],data + 3 +i*7 ,7);
		split_data[i][7] = '\0'; 
	}
//	printf("%s",number_data);
//	for(i=0;i<6;i++)
//		printf("%s",split_data[i]);
//	printf(" testdata\n");

	posdata->number = strtol(number_data,&endptr,10);
	if(*endptr !='\0')
		if(*endptr =='k')
			posdata->flag =1;
		else
			ret = -1;
	
	for(i=0;i<6;i++)
	{
		posdata->pos[i] = strtod(split_data[i],&endptr);
		if(*endptr !='\0')
		{
			ret = -1;
		}
	}
/*
	printf("%d%d ",posdata->number,posdata->flag);
	for(i=0;i<6;i++)
		printf("%5.2f ",posdata->pos[i]);
	printf(" testdata \n");
*/
	return ret;
}

int main(void)
{

	fastrek_config config;
	pthread_t thread_uart, thread_inet;

	if(read_config(&config)!=0)
		printf("config error\n");
		
	pthread_create(&thread_uart, NULL, thread_uart_comm, (void*)&config);
	pthread_create(&thread_inet, NULL, thread_inet_comm, (void*)&config);

	pthread_join(thread_uart,NULL);
	pthread_join(thread_inet,NULL);


	return 0;
}

void *thread_uart_comm(void *pParam)
{
	fastrek_config *config = (fastrek_config *)pParam;
	int fd;
	char c='P';
	char str_data[100];
	int word_size;
	int i;
	fastrak_data posdata;
	struct termios stdinattr;
	struct termios uartattr;

	char test_data[] = "02   12.01   4.15  -5.70-155.01  63.10 -69.68";

	if (fd < 0)
		return ;

	// set to non canonical mode for stdin
	fd = uart_open(UART_DEVICE, UART_SPEED, &uartattr);
	tcgetattr(0, &stdinattr);
	stdinattr.c_lflag &= ~ICANON;
	stdinattr.c_lflag &= ~ECHO;
//	stdinattr.c_lflag &= ~ISIG;
	stdinattr.c_cc[VMIN] = 0;
	stdinattr.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &stdinattr);

	write(fd, &c, 1);
	usleep(500000);
	while (1)
	{
		word_size = read(fd, &str_data, 100);
		if(word_size <0)
		{
			printf("read error\n");
			tcflush(fd,TCIFLUSH);	//エラー時に受信バッファクリア
			tcflush(fd,TCOFLUSH);	//エラー時に送信バッファクリア
			sleep(5);
			write(fd, &c, 1);
		}
		else
		{
//			printf("%d\t",word_size);
			str_data[word_size-2] = '\0';	//CR+LF消去

			printf("%s\n",str_data);
			
//			sdata_split(str_data,&posdata);
//			sdata_split(test_data,&position_data[0]);
			sdata_split(str_data,&position_data[0]);
//			for(i=0;i<word_size-2;i++)
//				printf("%x ",str_data[i]);
//			printf("\n");
		write(fd, &c, 1);
		}
		sleep(1);

	}

	uart_close(fd, &uartattr);

}
void *thread_inet_comm(void *pParam)
{
	fastrek_config *config = (fastrek_config *)pParam;
	int i;
	while(1)
	{
	printf("%d\n",position_data[0].number);

	for(i=0;i<6;i++)
		printf("%f\t",position_data[0].pos[i]);

	printf("\n");
	usleep(1000);
	}
}
