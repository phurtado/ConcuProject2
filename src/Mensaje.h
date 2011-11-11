#ifndef MENSAJE_H_
#define MENSAJE_H_

#include "common.h"
#include "Registro.h"

typedef struct {
	// Este campo va a ser el pid del cliente.
	long mtype;
	// Delimita que acción quiere realizar el cliente.
	TCOM comando;
	// Un registro de la BD
	Registro reg;
} Mensaje;
#endif
