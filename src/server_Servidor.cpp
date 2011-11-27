#include "Servidor.h"

Servidor::Servidor() {
	// Creo la Cola de Mensajes	 
	this->cola = new Cola<Mensaje>((char*) PATHCOLA, CHARCOLA);
	// Creo la Base de Datos
	this->bd = new BaseDeDatos((char*) PATHBD);

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
	delete this->bd;
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
    // Los clientes esperan mensajes con mtype igual a su PID.
    msj.mtype = msj.pid;
}

void Servidor::procesarConexion(Mensaje & msj) {
	// Verifico que el cliente no esté conectado al servidor
	if (this->buscarCliente(msj.pid) == this->listaClientesOn->end()) {
		// El cliente no está conectado. Primero lo agrego  a la lista
		this->listaClientesOn->push_front(msj.pid);
		// Luego reescribo el mensaje
		cout << "Cliente " << msj.pid << ": Conexión exitosa" << endl;
		msj.comando = CONOK;
	} else {
		// El cliente está conectado, no le puedo dar el alta. Envío un
		// mensaje de error.
		msj.comando = ERROR;
	}
}

void Servidor::procesarDesconexion(Mensaje & msj) {
	// Verifico que el cliente esté conectado al servidor
	list<int>::iterator it = this->buscarCliente(msj.pid);

	if (it != this->listaClientesOn->end()) {
		// Encontre al cliente, lo borro de la lista
		this->listaClientesOn->erase(it);
		// Reescribo el mensaje a enviarle al cliente
		cout << "Cliente " << msj.pid << ": Desconexión exitosa" << endl;
		msj.comando = DESCONOK;
	}
	else {
		// No encontré al cliente, mando un mensaje de error
		msj.comando = ERROR;
	}
}

void Servidor::procesarLectura(Mensaje & msj) {
	// Primero verifico que el cliente esté conectado a la BD
	if (this->buscarCliente(msj.pid) != this->listaClientesOn->end()) {
		// Cliente válido. Leo el registro de la BD.
		Registro reg;
		if (this->bd->leerRegistro(reg, msj.numReg) == -1) {
			// El número de registro no era válido
			msj.comando = ERROR;
			cout << "Cliente " << msj.pid << ": Lectura fallida" << endl;
		}
		else {
			// Número de registro válido
			msj.comando = LEERRGOK;
			// Agrego el registro al mensaje
			msj.reg = reg;
			cout << "Cliente " << msj.pid << ": Lectura exitosa" << endl;
		}
	}
	else {
		// El cliente no está conectado al servidor. Envío un mensaje de error.
		cout << "Cliente " << msj.pid << ": Cliente no conectado al servidor" << endl;
		msj.comando = ERROR;
	}
}

void Servidor::procesarAlta(Mensaje & msj) {
	// Primero verifico que el cliente esté conectado a la BD
	if (this->buscarCliente(msj.pid) != this->listaClientesOn->end()) {
		// Cliente válido, agrego el registro a la BD

		// TODO Dado que el registro viene con formato válido, se puede
		// asumir que el alta siempre va a ser válida.
		// HIPOTESIS: La BD entra en la Memoria Compartida (ratificada por Julia)
		this->bd->agregarRegistro(msj.reg);

		cout << "Cliente " << msj.pid << ": Alta exitosa" << endl;
		// Envío respuesta exitosa al cliente
		msj.comando = ALTARGOK;
	}
	else {
		// El cliente no está conectado al servidor. Le envío un mensaje de error.
		cout << "Cliente " << msj.pid << ": Cliente no conectado al servidor" << endl;
		msj.comando = ERROR;
	}
}

void Servidor::procesarMod(Mensaje & msj) {
	// Primero verifico que el cliente esté conectado a la BD
	if (this->buscarCliente(msj.pid) != this->listaClientesOn->end()) {
		// Encontré al cliente, realizo la modificación
		if (this->bd->modificarRegistro(msj.reg, msj.numReg) == -1) {
			// El número de registro no es válido, retorno un mensaje de error.
			cout << "Cliente " << msj.pid << ": Modificación fallida" << endl;
			msj.comando = ERROR;
		}
		else {
			// Se pudo realizar la modificación correctamente
			cout << "Cliente " << msj.pid << ": Modificación exitosa" << endl;
			msj.comando = MODRGOK;
		}
	}
	else {
		// El cliente no está conectado al servidor. Le envío un mensaje de error.
		cout << "Cliente " << msj.pid << ": Cliente no se encuentra en el servidor" << endl;
		msj.comando = ERROR;
	}
}

void Servidor::procesarBaja(Mensaje & msj) {
	// Primero verifico que el cliente esté conectado a la BD
	if (this->buscarCliente(msj.pid) != this->listaClientesOn->end()) {
		// Encontré al cliente, realizo la modificación
		if (this->bd->eliminarRegistro(msj.numReg) == -1) {
			// El número de registro no es válido, retorno un mensaje de error.
			cout << "Cliente " << msj.pid << ": Baja fallida" << endl;
			msj.comando = ERROR;
		}
		else {
			// Se pudo realizar la modificación correctamente
			cout << "Cliente " << msj.pid << ": Baja exitosa" << endl;
			msj.comando = BAJARGOK;
		}
	}
	else {
		// El cliente no está conectado al servidor. Le envío un mensaje de error.
		cout << "Cliente " << msj.pid << ": Cliente no se encuentra en el servidor" << endl;
		msj.comando = ERROR;
	}
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
