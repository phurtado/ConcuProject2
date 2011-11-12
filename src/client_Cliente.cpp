#include "Cliente.h"

Cliente::Cliente() {
	// Creo la cola
	this->cola = new Cola<Mensaje>((char*) PATHCOLA, CHARCOLA);
	// Guardo el valor del pid del cliente
	this->pid = getpid();

	// Le envío un mensaje al servidor solicitandole la conexión
	Mensaje msj;
	this->crearMsj(msj, this->pid, CON);
	this->cola->escribir(msj);
	// Espero la respuesta
	this->cola->leer(this->pid, &msj);

	// Verifico que se haya podido realizar la conexión
	if (msj.comando == CONOK) {
		cout << "Cliente conectado al servidor" << endl;
	}
	else {
		// Esto no debería pasar, pero si pasa el Cliente se "debería"
		// cerrar TODO
		cout << "Cliente no se pudo conectar al servidor" << endl;
	}
}

Cliente::~ Cliente() {
	// Creo el mensaje de desconexión y lo mando
	Mensaje msj;
	this->crearMsj(msj, this->pid, DESCON);
	this->cola->escribir(msj);
	// Espero la respuesta del servidor
	this->cola->leer(this->pid, &msj);

	if (msj.comando == DESCONOK) {
		cout << "Cliente desconectado del servidor" << endl;
	}
	else {
		// Esto no debería pasar, pero si pasa el Cliente se "debería"
		// cerrar TODO
		cout << "El cliente no se pudo desconectar del servidor" << endl;
	}
}
int Cliente::leerRegistro(Registro & reg) {
	Mensaje msj;
	// Pido al cliente el número de registro a leer
	int numReg = this->obtenerNumReg();
	// Creo el mensaje
	this->crearMsj(msj, this->pid, LEERRG, numReg);
	// Ahora que tengo el mensaje, se lo envío al servidor
	this->cola->escribir(msj);
	// Espero la respuesta
	this->cola->leer(this->pid, &msj);

	if (msj.comando == ERROR) {
		// No se pudo leer el registro de la BD.
		cout << "El registro a leer no pudo ser leído" << endl;
		return -1;
	}
	else if (msj.comando == LEERRGOK) {
		cout << "La lectura del registro fue concretada exitosamente" << endl;
		// Copio el registro
		strcpy(reg.nombre, msj.reg.nombre);
		strcpy(reg.dir, msj.reg.dir);
		strcpy(reg.tel, msj.reg.tel);
		return 0;
	}
	else {
		// Esto no debería pasar
		return -2;
	}
}

int Cliente::agregarRegistro() {
	Mensaje msj;
	Registro reg;
	// Pido al cliente el registro a agregar
	this->obtenerReg(reg);
	// Armo el mensaje a enviar
	this->crearMsj(msj, this->pid, ALTARG, reg);
	// Envío el mensaje
	this->cola->escribir(msj);
	// Espero la respuesta del servidor
	this->cola->leer(this->pid, &msj);

	if (msj.comando == ERROR) {
		cout << "ERROR: El registro no pudo ser agregado" << endl;
		return -1;
	}
	else if (msj.comando == ALTARGOK) {
		cout << "El registro fue agregado correctamente" << endl;
		return 0;
	}
	else {
		// Esto no debería pasar
		return -2;
	}
}

int Cliente::modificarRegistro() {
	Mensaje msj;
	Registro reg;
	// Pido al cliente el número de registro a modificar
	int numReg = this->obtenerNumReg();
	// Luego le pido el registro
	this->obtenerReg(reg);
	// Armo el mensaje a enviar
	this->crearMsj(msj, this->pid, MODRG, reg, numReg);
	// Envío el mensaje
	this->cola->escribir(msj);
	// Espero la respuesta del servidor
	this->cola->leer(this->pid, &msj);

	if (msj.comando == ERROR) {
		cout << "ERROR: El registro no pudo ser modificado" << endl;
		return -1;
	}
	else if (msj.comando == MODRGOK) {
		cout << "El registro fue modificado correctamente" << endl;
		return 0;
	}
	else {
		// Esto no debería pasar
		return -2;
	}
}

int Cliente::eliminarRegistro() {
	Mensaje msj;
	// Pido al cliente el número de registro a eliminar de la BD
	int numReg = this->obtenerNumReg();
	// Creo el mensaje
	this->crearMsj(msj, this->pid, BAJARG, numReg);
	// Envío el mensaje
	this->cola->escribir(msj);
	// Espero la respuesta
	this->cola->leer(this->pid, &msj);

	if (msj.comando == ERROR) {
		cout << "ERROR: El registro no pudo ser eliminado" << endl;
		return -1;
	}
	else if (msj.comando == BAJARGOK) {
		cout << "El registro fue eliminado correctamente" << endl;
		return 0;
	}
	else {
		// Esto no debería pasar
		return -2;
	}
}

void Cliente::crearMsj(Mensaje & msj, int pid, TCOM com) const {
	msj.mtype = this->pid;
	msj.comando = com;
}

void Cliente::crearMsj(Mensaje & msj, int pid, TCOM com, const Registro & reg) const {
	this->crearMsj(msj, pid, com);
	msj.reg = reg;
}

void Cliente::crearMsj(Mensaje & msj, int pid, TCOM com, int numReg) const {
	this->crearMsj(msj, pid, com);
	msj.numReg = numReg;
}

void Cliente::crearMsj(Mensaje & msj, int pid, TCOM com, const Registro & reg, int numReg) const {
	this->crearMsj(msj, pid, com, reg);
	msj.numReg = numReg;
}

void Cliente::obtenerReg(Registro & reg) const {
	char buffer[200];
	size_t size;
	bool cumpleCond;
	cout << "Creación del registro" << endl;

	// TODO Copy-Paste innecesario, dsp modularizar
	// TODO No me toma los espacios, dsp verlo

	// Pido primero el nombre
	cout << "Ingrese un nombre" << endl;
	do {
		fscanf(stdin, "%s", buffer);
		// Reservo un espacio para el /0, de ahí el +1
		size = strlen(buffer)+1;
		cumpleCond = !(size > sizeof(reg.nombre));
		if (cumpleCond == false) {
			cout << "El nombre ingresado es muy largo, ingrese un nombre "
			"con menos caracteres" << endl;
		}
	} while(!cumpleCond);
	strcpy(reg.nombre, buffer);

	// Luego pido la dirección
	cout << "Ingrese una dirección" << endl;
	do {
		fscanf(stdin, "%s", buffer);
		// Reservo un espacio para el \0, de ahí el +1
		size = strlen(buffer)+1;
		cumpleCond = !(size > sizeof(reg.dir));
		if (cumpleCond == false) {
			cout << "La dirección ingresado es muy largo, ingrese una dirección "
			"con menos caracteres" << endl;
		}
	} while(!cumpleCond);
	strcpy(reg.dir, buffer);

	// Luego pido el teléfono
	cout << "Ingrese un teléfono" << endl;
	do {
		fscanf(stdin, "%s", buffer);
		// Reservo un espacio para el /0, de ahí el +1
		size = strlen(buffer)+1;
		cumpleCond = !(size > sizeof(reg.tel));
		if (cumpleCond == false) {
			cout << "El teléfono ingresado es muy largo, ingrese un teléfono "
			"con menos caracteres" << endl;
		}
	} while(!cumpleCond);
	strcpy(reg.tel, buffer);
}

int Cliente::obtenerNumReg() const {
	cout << "Ingrese un número de registro a leer/modificar/eliminar de la BD" << endl;
	char buffer[10];
	fscanf(stdin, "%s", buffer);
	// TODO validar que sea un número
	int numReg = atoi(buffer);
	return numReg;
}
