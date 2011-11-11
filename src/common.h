#ifndef COMMON_H_
#define COMMON_H_

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Protocolo de comunicación
typedef enum {CON = 0, DESCON = 1, LEERRG = 2,
							ALTARG = 3, MODRG = 4 , BAJARG = 5,
							CONOK = 6, DESCONOK = 7, LEERRGOK = 8,
							ALTARGOK = 9, MODRGOK = 10, BAJARGOK = 11} TCOM;

// Tamaño de las memorias compartidas donde se va a almacenar la BD
#define CANTREG 1000
#define TAMSHMEM sizeof(Registro) * CANTREG

// Datos con los cuales se va a crear las Memorias Compartidas
#define PATHSHMEM "main.cpp"

// Datos con los cuales se va a crear la Cola de Mensajes
#define PATHCOLA "MenuCliente.txt"
#define CHARCOLA 1

// Datos de la Base de Datos
#define PATHBD "baseDeDatos.db"

//Funciones útiles
//bool validPath(const char* path) {
//	struct stat buf;
//	if (lstat(path, &buf) == -1)
//		return true;
//	else
//		return false;
//}
//
//bool validDirectory(const char* path) {
//	struct stat buf;
//	lstat(path, &buf);
//
//	if(! S_ISDIR(buf.st_mode))
//		return false;
//	else
//		return true;
//}
//
//bool validRegularFile(const char* path) {
//	struct stat buf;
//	lstat(path, &buf);
//
//	if(!S_ISREG(buf.st_mode))
//		return false;
//	else
//		return true;
//}
#endif
