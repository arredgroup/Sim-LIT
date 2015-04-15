/**
  @file Channel.h
  @class Channel
  @brief Clase Abstracta que representa el Simulador de pérdida de paquetes
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_CHANNEL
#define CLASS_CHANNEL

#include <vector>
#include "Package.h"


class Channel{
	
	vector<string> extras; //!<@brief Vector que contiene todos los argumentos que requiera el funcionamiento de cada Simulador

public:

	virtual vector<Package> simulate(vector<Package> list)=0;
	/*
	*  @brief Método que realiza el proceso de simulador de la pérdida de paquetes
	*  @param list lista con los paquetes que serán simulados
	*  @return lista con los paquetes que llegaron a destino
	*/

	void setExtras(vector<string> ext){
	/*
	*  @brief Método que asigna los argumentos que tiene el simulador
	*  @param ext argumentos que serán asignados al simulador
	*/
		extras = ext;
	}

	vector<string> getExtras(){
	/*
	*  @brief Método que devuelve los argumentos que utiliza el simulador
	*  @return lista con los paquetes que llegaron a destino
	*/
		return extras;
	}

};


#endif