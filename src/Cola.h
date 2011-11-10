#ifndef COLA_H_
#define COLA_H_

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>

template <class T> class Cola {
	private:
		key_t	clave;
		int		id;

	public:
		Cola ( char* archivo,char letra );
		~Cola();
		int escribir ( T dato );
		int leer ( int tipo,T* buffer );
};

template <class T> Cola<T> :: Cola ( char* archivo,char letra ) {
	this->clave = ftok ( archivo,letra );
	if ( this->clave == -1 )
		perror ( "Error en ftok" );

	this->id = msgget ( this->clave,0766|IPC_CREAT );
	if ( this->id == -1 )
		perror ( "Error en msgget" );
}

template <class T> Cola<T> :: ~Cola () {
	msgctl ( this->id,IPC_RMID,NULL );
}

template <class T> int Cola<T> :: escribir ( T dato ) {
	int resultado = msgsnd ( this->id,(const void *)&dato,sizeof(T)-sizeof(long),0 );
	return resultado;
}

template <class T> int Cola<T> :: leer ( int tipo,T* buffer ) {
	int resultado = msgrcv ( this->id,(void *)buffer,sizeof(T)-sizeof(long),tipo,0 );
	return resultado;
}

#endif /* COLA_H_ */
