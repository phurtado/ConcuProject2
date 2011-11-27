#ifndef MENSAJE_H_
#define MENSAJE_H_

#include "common.h"
#include "Registro.h"

typedef struct {
	// Este campo va a ser:
    //  1 si el mensaje lo envia el cliente
    //  PID del cliente si lo envia el server
	long mtype;
    // PID del cliente
    pid_t pid;
	// Delimita que acción quiere realizar el cliente.
	TCOM comando;
	// Registro de la BD que será leído/mod/borrado
	int numReg;
	// Un registro de la BD
	Registro reg;
} Mensaje;
#endif
