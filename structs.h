
#ifndef STRUCTS
#define STRUCTS

#include <stdio.h>
#include <string>

typedef struct image_head{
	int w; //Ancho Imagen
	int h; //Alto Imagen
	int hb;//Alto Bloque
	int wb;//Ancho Bloque
	int chnls;//Cantidad de Canales
	string folder;
}HEAD;

#endif