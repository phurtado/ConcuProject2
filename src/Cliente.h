#ifndef CLIENTE_H_
#define CLIENTE_H__

#include "Cola.h"
#include "Mensaje.h"
#include "common.h"

class Cliente {

private:
	Cola<Mensaje>* cola;
	
public:

	/* Pre:
	 * Post:
	 */
	Cliente();

	/* Post:
 	 */
	~Cliente();

	/* Pre:
 	 * Post:
 	 */
	void conexionServidor();

	/* Pre:
 	 * Post:
 	 */
	void desconexionServidor();
};

#endif
