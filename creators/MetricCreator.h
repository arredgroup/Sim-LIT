/**
  @file MetricCreator.h
  @class MetricCreator
  @brief Clase que implementa la fábrica de Métricas
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_METRICCREATOR
#define CLASS_METRICCREATOR

#include "../class/Metric/Metric.h"
#include "../class/Metric/MetricPsnr.h"

#include <string>
#include <cstring>
#include <cstdio>

class MetricCreator{

public:

static Metric* create(string fp){
	/*
	*  @brief Método que crea una determinada Métrica
	*  @param fp Nombre de la métrica  que será creada
	*/
	if(!fp.compare(METRIC_PSNR)){
		return new MetricPsnr();
	}
	return NULL;
}

};

#endif