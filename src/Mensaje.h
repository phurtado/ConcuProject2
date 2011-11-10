#ifndef MENSAJE_H_
#define MENSAJE_H_

#include "common.h"

typedef struct {
	long mtype;
	TCOM comando;
	char* mensaje;	
} Mensaje;
#endif
