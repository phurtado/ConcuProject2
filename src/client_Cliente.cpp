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
		cout << "El registro fue agregado correctamente en la posicion " << msj.numReg << endl;
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
	msj.mtype = 1;
    msj.pid = this->pid;
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

int Cliente::leerEntrada(string &entrada) const {
	size_t i, size;
	char buffer[BUFSIZE];
	cin.clear();
	cin >> entrada;
	i = entrada.size();
	while(i > 0) {
		cin.get(buffer, BUFSIZE); // get no bloquea esperando una entrada del usuario
		i = cin.gcount();
		entrada.append(buffer, i);
	}
	// Reservo un espacio para el /0, de ahí el +1
	size = entrada.size() + 1;
	return size;
}

void Cliente::obtenerCampo(size_t tamCampo, string nombreCampo, string &campo) const {
	size_t size;
	bool cumpleCond;
	campo = "";
	do {
		size = leerEntrada(campo);
		cumpleCond = !(size > tamCampo);
		if (cumpleCond == false) {
			cout << nombreCampo << " ingresado muy largo, debe tener un maximo de " << tamCampo << 
			" caracteres. Intente nuevamente." << endl;
		}
	} while(!cumpleCond);
}

void Cliente::obtenerReg(Registro & reg) const {
	string entrada;
	cout << "Creación del registro" << endl;
	
	// Pido primero el nombre
	cout << "Ingrese un nombre" << endl;
	obtenerCampo(sizeof(reg.nombre), "Nombre", entrada);
	strcpy(reg.nombre, entrada.c_str());
	
	// Luego pido la dirección
	cout << "Ingrese una dirección" << endl;
	obtenerCampo(sizeof(reg.dir), "Direccion", entrada);
	strcpy(reg.dir, entrada.c_str());

	// Luego pido el teléfono
	cout << "Ingrese un teléfono" << endl;
	obtenerCampo(sizeof(reg.tel), "Telefono", entrada);
	strcpy(reg.tel, entrada.c_str());
}

int Cliente::obtenerNumReg() const {
	char buffer[10];
	int numReg = 0;
	do{
		cout << "Ingrese un número de registro a leer/modificar/eliminar de la BD" << endl;
		fscanf(stdin, "%s", buffer);
		numReg = atoi(buffer);
		if(numReg <= 0)
			cout << "A ingresado caracteres invalidos. Intente nuevamente" << endl;
	}
	while(numReg <= 0);
	return numReg;
}
