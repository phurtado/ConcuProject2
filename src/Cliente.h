#ifndef CLIENTE_H_
#define CLIENTE_H__

#include "Cola.h"
#include "Mensaje.h"
#include "common.h"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
using namespace std;

class Cliente {

private:
	// Cola de mensajes para establecer la comunicación con el Servidor.
	Cola<Mensaje>* cola;
	


public:

	/* Pre:
	 * Post:
	 */
	Cliente();

	/* Post:
 	 */
	~Cliente();
};

#endif
