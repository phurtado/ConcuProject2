#include "Servidor.h"

Servidor::Servidor() {
	// Creo la Cola de Mensajes	 
	this->cola = new Cola<Mensaje>((char*) PATHCOLA, CHARCOLA);
	// Creo la Base de Datos
	// this->bd = new BaseDeDatos((char*) PATHBD);

	// Creo la lista.
	this->listaClientesOn = new list<int>;
}

void Servidor::escucharMensaje() {
	// Escucho si llega alguna petición en la cola de mensajes
	// Tomo el primer mensaje que haya en la cola
	Mensaje msj;
	this->cola->leer(0, &msj);
	// Recibí un mensaje, lo proceso
	this->procesarMensaje(msj);
	// Luego de procesarlo, lo envío al cliente
	this->cola->escribir(msj);
}

Servidor::~Servidor() {
	// TODO Desconecto a todos los clientes, mandandoles una signal.
	// Destruyo la cola
	delete this->cola;
	// Destruyo la BD
	// delete this->bd;
}

void Servidor::procesarMensaje(Mensaje & msj) {
	// Proceso el mensaje llamando a la función correspondiente
	// del hash de funciones.
	switch (msj.comando) {
		case CON:
			this->procesarConexion(msj);
			break;
		case DESCON:
			this->procesarDesconexion(msj);
			break;
		case LEERRG:
			this->procesarLectura(msj);
			break;
		case ALTARG:
			this->procesarAlta(msj);
			break;
		case MODRG:
			this->procesarMod(msj);
			break;
		case BAJARG:
			this->procesarBaja(msj);
			break;
		default:
			break;
	}
}

void Servidor::procesarConexion(Mensaje & msj) {
	// Verifico que el cliente no esté conectado al servidor
	if (this->buscarCliente(msj.mtype) == this->listaClientesOn->end()) {
		// El cliente no está conectado. Primero lo agrego  a la lista
		this->listaClientesOn->push_back(msj.mtype);
		// Luego reescribo el mensaje
		cout << "Cliente " << msj.mtype << ": Procesando petición de Conexión" << endl;
		msj.comando = CONOK;
	}
}

void Servidor::procesarDesconexion(Mensaje & msj) {
	// Verifico que el cliente esté conectado al servidor
	list<int>::iterator it = this->buscarCliente(msj.mtype);

	if (it != this->listaClientesOn->end()) {
		// Encontre al cliente, lo borro de la lista
		this->listaClientesOn->erase(it);
		// Reescribo el mensaje a enviarle al cliente
		cout << "Cliente " << msj.mtype << ": Procesando petición de Desconexión" << endl;
		msj.comando = DESCONOK;
	}
}

void Servidor::procesarLectura(Mensaje & msj) {
}

void Servidor::procesarAlta(Mensaje & msj) {
}

void Servidor::procesarMod(Mensaje & msj) {
}

void Servidor::procesarBaja(Mensaje & msj) {
}

list<int>::iterator Servidor::buscarCliente(int pid) {
	// Busco al cliente en la lista
	bool encontrado = false;
	list<int>::iterator it = this->listaClientesOn->begin();
	while ((encontrado == false) && (this->listaClientesOn->end() != it)) {
		if (*it == pid) {
			// Encontré al cliente
			encontrado = true;
			break;
		}
		else {
			// Avanzo el iterador
			it++;
		}
	}
	// Si lo encontré el iterador no apunta al final
	return it;
}
