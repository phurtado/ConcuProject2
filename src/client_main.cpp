#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Cliente.h"
using namespace std;

void imprimirReg(Registro & reg);

int main() {
	// Como en el otro proyecto, al cliente le aparece un menú con el listado
	// de las opciones disponibles
	Cliente cliente;
	int opcion = 1;
	bool sigue = true;
	char buffer[100];
	FILE* fd = fopen("MenuCliente.txt", "r");

	while(sigue) {

		// Primero imprimo el menú
		fseek(fd, 0, SEEK_SET);
		do {
			// fscanf(fd,"%[^\n]\n", buffer);
			if (fgets(buffer, sizeof(buffer), fd) != NULL) {
				cout << buffer;
			}
		} while (!feof(fd));

		// Obtengo la opción elegida por el cliente
		cout << endl << "Número de opción: ";
		fscanf(stdin, "%s", buffer);
		cout << endl << endl;
		opcion = atoi(buffer);

		// Proceso la petición
		switch(opcion) {
			case 1:
				Registro reg;
				if (cliente.leerRegistro(reg) != -1) {
					// Imprimo el registro leído
					imprimirReg(reg);
				}
				break;
			case 2:
				cliente.agregarRegistro();
				break;
			case 3:
				cliente.modificarRegistro();
				break;
			case 4:
				cliente.eliminarRegistro();
				break;
			case 5:
				// El cliente desea salir, pongo sigue en false
				fclose(fd);
				sigue = false;
				break;
			default:
				cout << "La opción ingresada es inválida" << endl;
				break;
		}
	}
	return 0;			
}

void imprimirReg(Registro & reg) {
	cout << "Registro leído:" << endl;
	cout << "Nombre:" << reg.nombre << endl;
	cout << "Dirección:" << reg.dir << endl;
	cout << "Teléfono:" << reg.tel << endl;
}
