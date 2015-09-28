/**
  @file RebuilderCreator.h
  @class RebuilderCreator
  @brief Clase que implementa la fábrica de Rebuilder
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_REBUILDERCREATOR
#define CLASS_REBUILDERCREATOR

#include "../class/Rebuilder.h"
#include "../class/RebuilderAverage.h"
#include "../class/RebuilderAveragePixel.h"
#include "../class/RebuilderAverageOptimized.h"
#include "../class/RebuilderRamanBabu2001.h"

class RebuilderCreator{
	
public:

	static Rebuilder* create(string fp){
	/*
	*  @brief Método que crea un determinado Rebuilder
	*  @param fp Nombre del Rebuilder que será creado
	*/
		if(!fp.compare(AVERAGE_RBD)){
			return new RebuilderAverage();
		}if(!fp.compare(AVERAGE_PIXEL_RBD)){
			return new RebuilderAveragePixel();
		}
		if(!fp.compare(AVERAGE_OPT_RBD)){
			return new RebuilderAverageOptimized();
		}
		if(!fp.compare(AVERAGE_RAMANBABU2001)){
			return new RebuilderRamanBabu2001();
		}
		return NULL;
	}

};

#endif
