/**
  @file RebuilderCreator.h
  @class RebuilderCreator
  @brief Clase que implementa la fábrica de Rebuilder
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_REBUILDERCREATOR
#define CLASS_REBUILDERCREATOR

#include "../class/Rebuilder/Rebuilder.h"
#include "../class/Rebuilder/RebuilderAverage.h"
#include "../class/Rebuilder/RebuilderAveragePixel.h"
#include "../class/Rebuilder/RebuilderAverageOptimized.h"
#include "../class/Rebuilder/RebuilderRamanBabu2001.h"
#include "../class/Rebuilder/RebuilderCDD.h"
#include "../class/Rebuilder/RebuilderBilineal.h"
#include "../class/Rebuilder/RebuilderTVBloque.h"
#include "../class/Rebuilder/RebuilderBilinealPonde.h"
#include "../class/Rebuilder/RebuilderMsr.h"
#include "../class/Rebuilder/RebuilderBicubico.h"

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
        if(!fp.compare(AVERAGE_CDD)){
            return new RebuilderCDD();
       	}
        if(!fp.compare(AVERAGE_BILI)){
        	return new RebuilderBilineal();
       	}
        if(!fp.compare(AVERAGE_BILIPONDE)){
            return new RebuilderBilinealPonde();
       	}
        if(!fp.compare(AVERAGE_BILIPONDE2)){
            return new RebuilderMsr();
       	}
        if(!fp.compare(AVERAGE_TVBLOCK)){
            return new RebuilderTVBloque();
        }
        if(!fp.compare(AVERAGE_BIC)){
            return new RebuilderBicubico();
        }
		return NULL;
	}

};

#endif
