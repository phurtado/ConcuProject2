#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main() {
	// Como en el otro proyecto, al cliente le aparece un menú con el listado
	// de las opciones disponibles
	int opcion = 1;
	bool sigue = true;
	char buffer[100];
	FILE* fd = fopen("MenuCliente.txt", "r");

	while(sigue) {

			// Primero imprimo el menú
			fseek(fd, 0, SEEK_SET);
			do {
				fscanf(fd,"%[^\n]\n", buffer);
				cout << buffer << endl;
			} while (!feof(fd));

			// Obtengo la opción elegida por el cliente
			cout << endl << "Número de opción: ";
			fscanf(stdin, "%s", buffer);
			opcion = atoi(buffer);

			// Proceso la petición
			switch(opcion) {
				case 1:
					break;
				case 2:
					break;
				case 3:
					break;
				case 4:
					break;
				case 5:
					break;
				case 6:
					// El cliente desea salir, pongo sigue en false
					fclose(fd);
					sigue = false;
					break;
				default:
					break;
			}
	}
	
	return 0;			
}
