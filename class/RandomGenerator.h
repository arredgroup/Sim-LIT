/**
  @file RandomGenerator.h
  @class RandomGenerator
  @brief Clase que representa un Generador de Paquetes Aleatorio
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_RANDOMGENERATOR
#define CLASS_RANDOMGENERATOR

#include <string>
#include <fstream>
#include <stdlib.h> 
#include <time.h>  

using namespace std;

class RandomGenerator{

public:

	static void generate(int MAX, int amount_pkgs){
		/*
		* @brief Método que Genera un archivo denominado loss con una cantidad MAX de paquetes perdidos
		* @param MAX cantidad de paquetes a ser perdidos
		* @param amount_pkgs Cantidad total de Paquetes 
		*/
		ofstream file;
		file.open("files/loss_random.loss",ofstream::out| ofstream::trunc);
		srand(time(NULL));
		//file << amount_pkgs<< "\n";
		int x, count=0;
		for (int i = 0; i < amount_pkgs; i++)
		{
				x = rand()%2;
				if(x==0){
					if(count<MAX){
						count++;
					}else{
						x=1;
					}
				}
			file << i << " " << x << "\n";
		}
		file.close();
	}

};

#endif