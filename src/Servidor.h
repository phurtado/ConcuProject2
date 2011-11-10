#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include "Cola.h"
#include "Mensaje.h"
#include "MemoriaCompartida.h"
#include "Registro.h"
#include "common.h"
#include "BaseDeDatos.h"

class Servidor {

private:
	// Cola de mensajes del servidor
	Cola<Mensaje>* cola;
	// Atributo que indica la cantidad de memorias compartidas que usa el servidor
	// para almacenar la base de datos
	BaseDeDatos* bd;
		
public:

	/* Pre:
   * Post:
   */
	Servidor();

	/* Post:
 	 */
	~Servidor(); 

	/* Pre:
   * Post:
   */
	void escucharPeticiones();
};
#endif
