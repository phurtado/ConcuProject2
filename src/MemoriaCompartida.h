#ifndef MEMORIACOMPARTIDA_H_
#define MEMORIACOMPARTIDA_H_

#define SHM_OK			0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>
#include <stdlib.h>


template <class T> class MemoriaCompartida {

private:

	int 	shmId;
	T*		ptrDatos;
	int cantidadProcesosAdosados();

public:

	MemoriaCompartida ();
	~MemoriaCompartida ();
	/* tam es un atributo que indica el tamaño de la memoria compartida.
	 * El parámetro está en bytes */
	int crear ( char *archivo,char letra, size_t tam );
	/* Offset indica cual de los registros de la base de datos voy a leer */
	void liberar ();
	void escribir ( T dato, int offset );
	T leer (int offset);

};

template <class T> MemoriaCompartida<T> :: MemoriaCompartida () {
}

template <class T> MemoriaCompartida<T> :: ~MemoriaCompartida () {
}

template <class T> int MemoriaCompartida<T> :: crear ( char *archivo,char letra, size_t tam) {

	// generacion de la clave
	key_t clave = ftok ( archivo,letra );
	if ( clave == -1 )
		return ERROR_FTOK;
	else {
		// creacion de la memoria compartida
		this->shmId = shmget (clave, tam, 0766|IPC_CREAT);

		if ( this->shmId == -1 )
			return ERROR_SHMGET;
		else {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* ptrTemporal = shmat ( this->shmId,NULL,0 );

			if ( ptrTemporal == (void *) -1 ) {
				return ERROR_SHMAT;
			} else {
				this->ptrDatos = (T *) ptrTemporal;
				return SHM_OK;
			}
		}
	}
}


template <class T> void MemoriaCompartida<T> :: liberar () {
	// detach del bloque de memoria
	shmdt ( (void *) this->ptrDatos );

	int procAdosados = this->cantidadProcesosAdosados ();

	if ( procAdosados == 0 ) {
		shmctl ( this->shmId,IPC_RMID,NULL );
	}
}

template <class T> void MemoriaCompartida<T> :: escribir ( T dato, int offset ) {
	* (this->ptrDatos + offset) = dato;
}

template <class T> T MemoriaCompartida<T> :: leer (int offset) {
	return ( *(this->ptrDatos + offset) );
}

template <class T> int MemoriaCompartida<T> :: cantidadProcesosAdosados () {
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );
	return estado.shm_nattch;
}


#endif /* MEMORIACOMPARTIDA_H_ */
