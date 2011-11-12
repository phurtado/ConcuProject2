#ifndef CLIENTE_H_
#define CLIENTE_H__

#include "Cola.h"
#include "Mensaje.h"
#include "common.h"
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
using namespace std;

class Cliente {

private:
	// Pid del proceso Cliente
	int pid;
	// Cola de mensajes para establecer la comunicación con el Servidor.
	Cola<Mensaje>* cola;
	

public:

	/* Pre: -
	 * Post: Cliente se da de alta en el servidor.
	 */
	Cliente();

	/* Post: Cliente se desconecta del servidor y libera los recursos utilizados.
 	 */
	~Cliente();

	/* Pre: El registro es pasado por referencia por lo cual el mismo no debe
	 * poseer información importante.
	 * Post: Devuelve 0 en caso de que se haya leído el registro en la BD con
	 * éxito. Retorna -1 en caso contrario. En el parámetro pasado por
	 * referencia vuelve el registro leído.
	 */
	int leerRegistro(Registro & reg);

	/* Pre: -
	 * Post: Devuelve 0 en caso de que se haya agregado el registro en la BD
	 * con éxito. Retorna -1 en caso contrario.
	 */
	int agregarRegistro();

	/* Pre: -
	 * Post: Devuelve 0 en caso de que se haya modificado el registro en la BD
	 * con éxito. Retorna -1 en caso contrario.
	 */
	int modificarRegistro();

	/* Pre: -
	 * Post: Devuelve 0 en caso de que se haya eliminado el registro en la BD
	 * con éxito. Retorna -1 en caso contrario.
	 */
	int eliminarRegistro();

private:

	/* Pre: Los parámetros deben ser válidos. La variable msj será sobreescrita,
	 * por lo cual no debe poseer ningún valor relevante.
	 * Post: Retorna en msj el mensaje formado. Los parámetros numReg y reg
	 * quedan indefinidos.
	 */
	void crearMsj(Mensaje & msj, int pid, TCOM com) const;

	/* Pre: Los parámetros deben ser válidos. La variable msj será sobreescrita,
	 * por lo cual no debe poseer ningún valor relevante.
	 * Post: Retorna en msj el mensaje formado. El parámetro numReg
	 * queda indefinido.
	 */
	void crearMsj(Mensaje & msj, int pid, TCOM com, const Registro & reg) const;

	/* Pre: Los parámetros deben ser válidos. La variable msj será sobreescrita,
	 * por lo cual no debe poseer ningún valor relevante.
	 * Post: Retorna en msj el mensaje formado. El parámetro reg
	 * queda indefinido.
	 */
	void crearMsj(Mensaje & msj, int pid, TCOM com, int numReg) const;

	/* Pre: Los parámetros deben ser válidos. La variable msj será sobreescrita,
	 * por lo cual no debe poseer ningún valor relevante.
	 * Post: Retorna en msj el mensaje formado. El parámetro numReg
	 * queda indefinido.
	 */
	void crearMsj(Mensaje & msj, int pid, TCOM com, const Registro & reg, int numReg) const;

	/* Pre: El parámetro reg será sobreescrito, no debe tener información
	 * relevante. Se formará el registro pidiendo los datos por stdin.
	 * Post: Devuelve un registro válido.
	 */
	void obtenerReg(Registro & reg) const;

	/* Pre: Se pedirá este parámetro por stdin
	 * Post: Devuelve un número de registro válido (mayor a 0).
	 */
	int obtenerNumReg() const;
};

#endif
