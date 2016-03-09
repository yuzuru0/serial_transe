#ifndef __read_config_h__
#define __read_config_h__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UNIT_INCH 0
#define UNIT_CM 1
#define UNIT_M 2
#define NO_PORT_EXIST -1

#define CONF_FILE "fastrek.conf"

typedef struct
{
	int unit_of_length;
	char ip_address[16];
	int ip_address_byte[4];
	int ip_port[4];
}fastrek_config;

int read_config(fastrek_config *config);
#endif
