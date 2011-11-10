#include "MemoriaCompartida.h"
#include "BaseDeDatos.h"
#include "Registro.h"

int main() {
	BaseDeDatos unaBD((char*) PATHBD);
	cout << "Imprimo la BD persistida" << endl;
	unaBD.imprimirBD();
	cout << endl;

	Registro aux;
	strcpy(aux.nombre,"Ezequiel");
	strcpy(aux.dir,"Mazza 935");
	strcpy(aux.tel,"4696-2187");
	unaBD.agregarRegistro(aux);
	strcpy(aux.nombre,"Pepe");
	unaBD.agregarRegistro(aux);
	strcpy(aux.dir, "Boquerón 101");
	unaBD.agregarRegistro(aux);
	// Elimino un registro
	unaBD.eliminarRegistro(2);
	// Modifico un registro
	strcpy(aux.tel, "0800-BD");
	unaBD.modificarRegistro(aux, 1);
	cout << "Cantidad de Registros en la BD: " << unaBD.getCantRegistros() << endl;
	cout << "Imprimo la BD a persistir" << endl;
	unaBD.imprimirBD();
	return 0;
}
