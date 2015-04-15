/**
  @file PacketizerCreator.h
  @class PacketizerCreator
  @brief Clase que implementa la fábrica de Packetizer
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_PACKETIZERCREATOR
#define CLASS_PACKETIZERCREATOR

#include "../class/Packetizer.h"
#include "../class/NormalPacketizer.h"

#include <string>
#include <cstring>
#include <cstdio>

class PacketizerCreator{

public:

static Packetizer* create(string fp){
	/*
	*  @brief Método que crea un determinado Packetizer
	*  @param fp Nombre del Packetizer que será creado
	*/
	if(!fp.compare(NORMAL_PAKETIZER)){
		return new NormalPacketizer();
	}
	return NULL;
}

};

#endif