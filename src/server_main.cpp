#include <iostream>
#include "SIGINT_Handler.h"
#include "SignalHandler.h"
#include "Registro.h"
#include "Servidor.h"
#include "MemoriaCompartida.h"
#include "BaseDeDatos.h"

// pruebita
#include <sys/types.h>
#include <sys/ipc.h>
using namespace std;

int main() {
	// Variables del main
	Servidor servidor;

	// Primero registro el handler de SIGINT
	SIGINT_Handler sigint_handler;
	// Luego registro el handler
	SignalHandler :: getInstance()->registrarHandler ( SIGINT,&sigint_handler );

	// El servidor escucha mientras no se reciba la señal SIGINT
	cout << "Servidor escuchando peticiones del cliente" << endl;
	while (sigint_handler.getGracefulQuit() == 0) {
		cout << "lala";
	}

	// Salí del while, se recibió SIGNIT. Cierro el servidor.
	SignalHandler:: destruir();
	cout << "Se cierra el servidor" << endl;
	return 0;
}

