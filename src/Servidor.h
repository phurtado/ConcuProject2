#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include "Cola.h"
#include "Mensaje.h"
#include "MemoriaCompartida.h"
#include "Registro.h"
#include "common.h"
#include "BaseDeDatos.h"
#include <map>
#include <iostream>
#include <list>
using namespace std;

class Servidor {

private:
	// Cola de mensajes del servidor
	Cola<Mensaje>* cola;
	// Atributo que indica la cantidad de memorias compartidas que usa el servidor
	// para almacenar la base de datos
	BaseDeDatos* bd;
	// Lista que posee los clientes que están conectados al servidor.
	list<int>* listaClientesOn;
		
public:

	/* Pre:
   * Post:
   */
	Servidor();

	/* Post:
 	 */
	~Servidor(); 

	/* Pre: -
   * Post: Bloquea al servidor hasta que le llegue un mensaje. Cuando le llega un mensaje
   * procesa al mismo, y responde a quien al destinatario del mensaje.
   */
	void escucharMensaje();

private:

		/* Pre:
		 * Post:
		 */
		void procesarMensaje(Mensaje & msj);

		/* Pre:
		 * Post:
		 */
		void procesarConexion(Mensaje & msj);

		/* Pre:
		 * Post:
		 */
		void procesarDesconexion(Mensaje & msj);

		/* Pre:
		 * Post:
		 */
		void procesarLectura(Mensaje & msj);

		/* Pre:
		 * Post:
		 */
		void procesarAlta(Mensaje & msj);

		/* Pre:
		 * Post:
		 */
		void procesarMod(Mensaje & msj);

		/* Pre:
		 * Post:
		 */
		void procesarBaja(Mensaje & msj);

		/* Pre: -
		 * Post: Busca a un cliente en el array de Clientes conectados.
		 * Retorna el iterador en la lista donde se encuentra el elemento
		 */
		list<int>::iterator buscarCliente(int pid);

};
#endif
