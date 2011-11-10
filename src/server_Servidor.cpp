#include "Servidor.h"

Servidor::Servidor() {
	// Creo la Cola de Mensajes	 
	this->cola = new Cola<Mensaje>((char*) PATHCOLA, CHARCOLA);
	// Creo la Base de Datos
	// this->bd = new BaseDeDatos((char*) PATHBD);
}

void Servidor::escucharPeticiones() {
	
}

Servidor::~Servidor() {
	// Destruyo la cola
	delete this->cola;
	// delete this->bd;
}
