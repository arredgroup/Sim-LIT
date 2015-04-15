/**
  @file ChannelCreator.h
  @class ChannelCreator
  @brief Clase que implementa la fábrica de Métricas
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_CHANNELCREATOR
#define CLASS_CHANNELCREATOR

#include "../class/Channel.h"
#include "../class/ChannelFile.h"
#include "../class/ChannelErlang.h"

#include <string>
#include <cstring>
#include <cstdio>

class ChannelCreator{

public:

static Channel* create(string fp, vector<string> extras){
	/*
	*  @brief Método que crea una determinada Métrica
	*  @param fp Nombre de la métrica  que será creada
	*/
	if(!fp.compare(LOST_FROM_FILE)){
		return new ChannelFile(extras);
	}
	if(!fp.compare(LOST_BY_ERLANG)){
		return new ChannelErlang(extras);
	}
	return NULL;
}

};

#endif