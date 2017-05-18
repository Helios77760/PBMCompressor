#include <stdio.h>
#include "pointManagement.h"

unsigned int pointsfrombyte(byte b, point* arr, unsigned long long int bytex, unsigned long long int bytey)
{
	unsigned char mask = 0x80;
	unsigned int counter=0;
	for(int i=0;i<8;i++)
	{
		if(b & mask)
		{
			arr[counter].x = bytex*8+i;
			arr[counter].y = bytey;
			arr[counter++].next = NULL;
		}
		mask >>= 1;
	}
	return counter;
}