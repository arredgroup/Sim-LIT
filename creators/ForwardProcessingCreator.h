/**
  @file ForwardProcessingCreator.h
  @class ForwardProcessingCreator
  @brief Clase que implementa la fábrica de ForwardProcessing
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_FORWARDPROCESSINGCREATOR
#define CLASS_FORWARDPROCESSINGCREATOR

#include "../class/ForwardProcessing.h"
#include "../class/TorusMixer.h"
#include "../class/TurnerMixer.h"
#include "../class/DsjalMixer.h"
#include "../class/MixerFromFile/MixerFromFile.h"
#include <string>
#include <cstring>
#include <cstdio>

class ForwardProcessingCreator{

public:

static ForwardProcessing* create(string fp){
	/*
	*  @brief Método que crea un determinado ForwardProcessing
	*  @param fp Nombre del ForwardProcessing que será creado
	*/
	if(!fp.compare(TORUS_MIX)){
		return new TorusMixer();
	}
	if(!fp.compare(TURNER_MIX)){
		return new TurnerMixer();
	}
	if(!fp.compare(DSJAL_MIX)){
		return new DsjalMixer();
	}
	if(!fp.compare(FILE_MIX)){
		return new MixerFromFile();
	}
	return NULL;
}

};

#endif