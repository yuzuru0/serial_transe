#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_config.h"


int read_config(fastrek_config *config)
{
	char input[256],*p,*endptr;
	int i,j;
	FILE *fp;

	fp = fopen(CONF_FILE,"rt");
	if(fp == NULL)
	{
		printf("Configuration file not found. Using dafault value\n");
		config->unit_of_length = UNIT_METER;
		config->unit_of_angle = UNIT_DEG;
		config->ip_address_byte[0] = 192;
		config->ip_address_byte[1] = 168;
		config->ip_address_byte[2] = 0;
		config->ip_address_byte[3] = 1;
		sprintf(config->ip_address,"%d.%d.%d.%d",config->ip_address_byte[0],config->ip_address_byte[1],config->ip_address_byte[2],config->ip_address_byte[3]);
		config->ip_port[0] = 15000;
		config->ip_port[1] = 15001;
		config->ip_port[2] = 15002;
		config->ip_port[3] = 15003;
		return 1;
	}


	fscanf(fp,"%1s[01]]",input);
	fscanf(fp,"%*[^\n]");
	if(input[0] == '0' )
	{
		config->unit_of_length=UNIT_INCH;
	}
	else if(input[0] == '1')
	{
		config->unit_of_length=UNIT_METER;
	}
	else if (input[0] == '2')
	{
		config->unit_of_length=UNIT_CM;
	}
	else if (input[0] == '3')
	{
		config->unit_of_length=UNIT_MM;
	}
	else
	{
		printf("Unknown error(can not detamine the unit of length)\n");
		return -1;
	}


	fscanf(fp,"%1s[01]]",input);
	fscanf(fp,"%*[^\n]");
	if(input[0] == '0' )
	{
		config->unit_of_angle=UNIT_DEG;
	}
	else if(input[0] == '1')
	{
		config->unit_of_length=UNIT_RAD;
	}
	else
	{
		printf("Unknown error(can not detamine the unit of angle)\n");
		return -1;
	}

	fscanf(fp,"%15s[0123456789.]",input);
	fscanf(fp,"%*[^\n]");

	p=strtok(input,".");
	for(i=0;i<4;i++)
	{
		config->ip_address_byte[i] = strtol(p,&endptr,10);
		if(*endptr !='\0'|| config->ip_address_byte[i]>255 || config->ip_address_byte[i]<0 ||p==NULL)
		{
			printf("IP address Error\n");
			return -1;
		}
		p=strtok(NULL,".");
		if(p==NULL && i!=3)
		{
			printf("IP address Error\n");
			return -1;
		}

	}
	sprintf(config->ip_address,"%d.%d.%d.%d",config->ip_address_byte[0],config->ip_address_byte[1],config->ip_address_byte[2],config->ip_address_byte[3]);

	for(i=0;i<4;i++)
	{
		input[0] = 'e';
		fscanf(fp,"%255s[0123456789]",input);
		fscanf(fp,"%*[^\n]");
		config->ip_port[i] = strtol(input,&endptr,10);
		if(*endptr!='\0')
		{
			if(i==0)
			{
				printf("ポート番号は1つ以上指定してください\n");
				return -1;
			}
			else
				config->ip_port[i] = NO_PORT_EXIST;
		}
	}

	return 0;
}
