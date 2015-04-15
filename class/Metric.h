/**
  @file Metric.h
  @class Metric
  @brief Clase Abstracta que representa la estructura que debe tener la métrica que se desee implementar
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_METRIC
#define CLASS_METRIC

#include <string>

class Metric{

public:

	string name;

	Metric(){}

	~Metric(){}

	virtual double calculate(Images org, Images rcv, bool show_data)=0;
	virtual string getName()=0;

};

#endif