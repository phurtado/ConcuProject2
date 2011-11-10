#include "BaseDeDatos.h"

int BaseDeDatos::regToEl(int numReg) const {
	if (numReg > this->cantRegistros) {
		return -1;
	}
	else {
		// El número de registro es válido, busco el numEl correspondiente.
		int actual = numReg-1;
		bool encontrado = false;
		do {
			// Si el actual es 1, verifico si el próximo está marcado
			if (this->arrayMarcados[actual] == 1) {
				actual++;
			}
			else {
				// Encontré el elemento, seteo encontrado a true
				encontrado = true;
			}
		} while (encontrado == false);
		return actual;
	}
}

BaseDeDatos::BaseDeDatos(char* path) {
	this->cantRegistros = 0;
	// Creo la memoria compartida
	this->sharedMem = new MemoriaCompartida<Registro>();
	this->sharedMem->crear((char*) PATHSHMEM, 0, TAMSHMEM);

	// Debo levantar la base de datos del archivo, o creala en
	// caso de que no exista
	FILE* fd = fopen((char*) PATHBD, "r+");
	if (fd != NULL) {
		// La BD existe, leo registros de la BD y los cargo en la
		// memoria compartida.
		Registro aux;
		char buffer[sizeof(Registro)];
		// Obtengo la longitud del archivo en el que está la BD
		fseek(fd, 0, SEEK_END);
		int sizeFile = ftell(fd);
		fseek(fd, 0, SEEK_CUR);

		int bytesLeidos = 0;
		do {
			// Leo un registro
			fread(buffer, sizeof(aux.nombre), 1, fd);
			memcpy(aux.nombre, buffer, sizeof(aux.nombre));
			fread(buffer, sizeof(aux.dir), 1, fd);
			memcpy(aux.dir, buffer, sizeof(aux.dir));
			fread(buffer, sizeof(aux.tel), 1, fd);
			memcpy(aux.tel, buffer, sizeof(aux.tel));

			// Luego lo escribo en la memoria compartida
			this->sharedMem->escribir(aux, this->cantRegistros);
			// Incremento la cantidad de registros en la BD en memoria
			this->cantRegistros++;
			bytesLeidos += sizeof(aux);
		} while(bytesLeidos != sizeFile);
	}
	else {
		// Creo el archivo donde se va a almacenar la BD.
		fd = fopen((char*) PATHBD, "a");
	}
	// Cierro el fd de la BD.
	fclose(fd);

	// Dado que al principio no hay registros eliminados,
	// cantElementos es igual a cantRegistros y la estructura
	// arrayMarcados posee ceros.
	this->cantElementos = this->cantRegistros;
	this->arrayMarcados = (int*) malloc (sizeof(int) * this->cantElementos);
}

int BaseDeDatos::getCantRegistros() const {
	return this->cantRegistros;
}

int BaseDeDatos::leerRegistro(Registro & unReg, int numReg) {
	// Para calcular cual es el elemento que debo leer en la
	// memoria compartida, me fijo
	if (numReg > this->cantRegistros) {
		return -1;
	}
	else {
		unReg = this->sharedMem->leer(this->regToEl(numReg));
		return 0;
	}
}

void BaseDeDatos::agregarRegistro(Registro & unReg) {
	// Agrego el registro al final e incremento la cantidad
	// de registros de la BD.
	this->sharedMem->escribir(unReg, this->cantElementos);
	this->cantRegistros++;
	this->cantElementos++;
	this->arrayMarcados = (int*) realloc (this->arrayMarcados, sizeof(int) * this->cantElementos);
}

int BaseDeDatos::modificarRegistro(Registro & unReg, int numReg) {
	if (numReg > this->cantRegistros) {
		return -1;
	}
	else {
		this->sharedMem->escribir(unReg, this->regToEl(numReg));
		return 1;
	}
}

int BaseDeDatos::eliminarRegistro(int numReg) {
	if (numReg > this->cantRegistros) {
		return -1;
	}
	else {
		this->arrayMarcados[this->regToEl(numReg)] = 1;
		this->cantRegistros--;
		return 0;
	}
}

void BaseDeDatos::imprimirBD() const {
	int i = 0;
	int numReg = 1;
	cout << "Impresión de la BD" << endl;
	Registro regAux;
	for (i = 0; i < this->cantElementos; i++) {
		// Si el elemento i está marcado, entonces no lo muestro
		if (this->arrayMarcados[i] == 0) {
			regAux = this->sharedMem->leer(i);
			cout << numReg << " " << regAux.nombre << " " << regAux.dir << " " << regAux.tel << endl;
			numReg++;
		}
	}
}

BaseDeDatos::~BaseDeDatos() {
	// Persisto la BD en memoria
	int i = 0;
	Registro aux;
	FILE* fd = fopen((char*) PATHBD, "w+");
	for (i = 0; i < this->cantElementos; i++) {
		// Verifico si el elemento i está marcado o no. Si no está marcado,
		// lo persisto.
		if (this->arrayMarcados[i] == 0) {
			aux = this->sharedMem->leer(i);
			fwrite(&aux, sizeof(Registro), 1, fd);
		}
	}
	fclose(fd);

	// Elimino la memoria compartida
	this->sharedMem->liberar();
}
