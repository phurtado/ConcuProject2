#ifndef BASEDEDATOS_H_
#define BASEDEDATOS_H_

#include "MemoriaCompartida.h"
#include "Registro.h"
#include "common.h"
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

class BaseDeDatos {

private:
	// Indica la cantidad de registros (los marcados y los no) que
	// posee la BD. Los registros marcados son los que han sido
	// eliminados de la BD.
	int cantElementos;
	// Estructura que sirve para identificar la cantidad de registros
	// eliminados. En la posición i, si el valor del array vale 1 es
	// porque el elemento ha sido eliminado.
	int* arrayMarcados;
	// Indica la cantidad de registros que posee la BD.
	int cantRegistros;
	// Memoria compartida donde se almacenará la BD.
	MemoriaCompartida<Registro>* sharedMem;

public:

	/* Pre: El path debe ser válido.
 	 * Post: Crea una BD la cual se va a persistir en el archivo pasado en el
 	 * path. Si el archivo referenciado en path no existe, crea una BD vacía.
 	 * Si existe, carga la BD.
 	 */
	BaseDeDatos(char* path);

	/* Pre: -
 	 * Post: Persiste la BD en memoria y luego destruye a la instancia.
 	 */
	~BaseDeDatos();

	/* Pre: -
	 * Post: Devuelve la cantidad de registros que posee la BD.
	 */
	int getCantRegistros() const;

	/* Pre: El número de registro debe ser válido.
   * Post: Retorna -1 si el número de registro ingresado no era válido.
   */
	int leerRegistro(Registro & unReg, int numReg);

	/* Pre: El registro debe ser válido.
 	 * Post: Agrega un registro al final de la BD. Retorna
 	 * 0 si se logro realizar la operación, -1 en caso contrario.
 	 */
	 void agregarRegistro(Registro & unReg);

	/* Pre: El registro debe ser válido.
	 * Post: Se modifica un registro de la BD. Retorna
	 * 0 si se logró realizar la operación, -1 en caso contrario.
	 */
	int modificarRegistro(Registro & unReg, int numReg);

	/* Pre: -
	 * Post: Se elimina un registro de la BD. Retorna
	 * 0 si logró realizar la operación, -1 en caso contrario.
	 */
	int eliminarRegistro(int numReg);

	/* Pre: -
	 * Post: Imprime la base de datos.
	 */
	void imprimirBD() const;


private:

	/* Pre: Número de registro debe ser válido
	 * Post: Retorna el número de elemento que está asociado con el número
	 * de registro ingresado. En caso de que numReg no sea válido,retorna -1.
	 */
	int regToEl(int numReg) const;

};

#endif
