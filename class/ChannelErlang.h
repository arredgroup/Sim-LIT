/**
  @file ChannelErlang.h
  @class ChannelErlang
  @brief Clase que representa el Simulador utilizando Erlang
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_CHANNELERLANG
#define CLASS_CHANNELERLANG

#include <iostream>
#include <string>
#include <vector>
#include "Package.h"
#include "Channel.h"

class ChannelErlang: public Channel{

public:

	ChannelErlang(vector<string> extras){
	/*
	*  @brief Constructor de la clase Erlang
	*  @param ext argumentos que serán asignados al simulador
	*/
		this->setExtras(extras);
	}

	vector<Package> simulate(vector<Package> list){
	/*
	*  @brief Método que realiza la simulación utilizando la Distribución de probabilidad de Erlang
	*  @param list lista con los paquetes que serán simulados
	*  @return lista con los paquetes que llegaron a destino
	*/
		int size = list.size();
		vector<string> ext = this->getExtras();
		if(ext.size()==0)
			return list;
		int N = atoi(ext[0].c_str());
		int lambda = atoi(ext[1].c_str());
		for (int i = 0; i < size; i = i+1){
			if(erlang(N,lambda/(list.size()-i))>0.5)
				list = deletePackageById(i,list);
		}
		return list;
	}

private:

	vector<Package> deletePackageById(int ID, vector<Package> list){
		/*
		*  @brief Método que elimina un Paquete de la lista por su ID
		*  @param ID identificador del paquete que se eliminará
		*  @param list Lista de Paquetes del cual se eliminará uno por el ID
		*  @return list
		*/
		for (int i = 0; i < list.size(); i++)
		{
			if(list[i].getID()==ID){
				list.erase(list.begin()+i);
				i--;
			}
		}
		return list;
	}

	double erlang(int N, int A){
		/*
		*  @brief Método Calcula el valor de Erlang sobre un determinado Valor
		*  @param A cantidad total de tráfico ofrecido en erlangs
		*  @param N es el número de recursos tales como servidores o circuitos en un grupo
		*  @return double el valor calculado de Erlang
		*/
		double num, den;
		num = (pow(A,N))/(factorial(N));
		den = 0;
		for(int i=0;i<N;i=i+1){
			den = den + (pow(A,i)/factorial(i));
		}
		return num/den;
	}
	
	double factorial(int x){
		if(x<=0)
		  return 1;
		return x*factorial(x-1);
	}
	
	double pow(int a, int b){
		if(b<=0)
		  return 1;
		return a*pow(a,b-1);
	}
};

#endif
