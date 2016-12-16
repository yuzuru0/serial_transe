#ifndef __read_config_h__
#define __read_config_h__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UNIT_INCH 0
#define UNIT_METER 1
#define UNIT_CM 2
#define UNIT_MM 3
#define UNIT_DEG 0
#define UNIT_RAD 1
#define NO_PORT_EXIST -1

#define CONF_FILE "/etc/fastrek.conf"

typedef struct
{
	int unit_of_length;
	int unit_of_angle;
	char ip_address[16];
	int ip_address_byte[4];
	int ip_port[4];
}fastrek_config;

int read_config(fastrek_config *config);
#endif
