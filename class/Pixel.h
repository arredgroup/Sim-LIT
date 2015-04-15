/**
  @file Pixel.h
  @class Pixel
  @brief Clase que representa un Pixel de una imagen
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_PIXEL
#define CLASS_PIXEL

#include "DataType.h"
#include <iostream>
#include <vector>
using namespace std;

class Pixel: public DataType
{
private:
	int amount_channels; //!<@brief Cantidad de Canales de la Imagen
	vector<int> channels;//!<@brief Canales de la Imagen
	double size;

public:

	~Pixel(){}

	Pixel(){
	/**
	 * @brief Constructor de la Clase Pixel por defecto
	 * 
	 */
		this->setValid(false);
        size=1;
	}

	Pixel(vector<int> nChannels, int aChannels){
	/**
	 * @brief Constructor de la Clase Pixel
	 * @param x1 La Posición del Pixel en el eje X
	 * @param y1 La Posición del Pixel en el eje Y
	 * @param nChannels Canales que posee el Pixel
	 * 
	 */
         amount_channels = aChannels;
         channels = nChannels;
         this->setValid(true);
         size=1;
	}

	vector<int> getChannels(){
	/**
	 * @brief Método que Devuelve Los canales del Pixel (1 si es Escala de Grises, 3 si es RGB)
	 * @return channels
	 */
		return channels;
	}

	void setChannels(vector<int> nChannels){
	/**
	 * @brief Método que Modifica los canales del Pixel
	 */
		channels = nChannels;
	}

	void setAmountChannels(int aChannels){
	/**
	 * @brief Método que Modifica la cantidad de canales del Pixel
	 */
		amount_channels = aChannels;
	}

	int* getContent(){
	/** 
	* @brief Función que devuelve en un arreglo de enteros todo el contenido del Pixel
	* return array
	*/
		int* array = (int*)malloc(sizeof(int)*(2+amount_channels));
		if(this->isValid()){
			array[0]=0;
		}
		else{
			array[0]=1;
		}
		array[1]=amount_channels;
		for (int i = 0; i < amount_channels; i++)
		{
			array[2+i]=channels[i];
		}
		return array;
	}

	int getAmountChannels(){
	/** 
	* @brief Obtiene cantidad de canales que tiene el Pixel 
	* return amount_channels
	*/
		return amount_channels;
	}

	int getIntensity(){
	/** 
	* @brief Método que obtiene la Intensidad del Pixel
	* return local_max
	*/
		int local_max=0;
		for(int x=0;x<amount_channels;x++){
			local_max+=channels[x];
		}
		return (int)(local_max/amount_channels);
	}

	double getSize(){
	/** 
	* @brief Obtiene el tamaño del Pixel Representado
	* return Devuelve 1 si es un Pixel de un canal (Escala de Grises) o cantidad de Canales * 1 (donde 1 es la cantidad de bytes que usa un entero)
	*/
		return size*amount_channels;
	}

	void setSize(double nsize){
		size=nsize;
	}
};

#endif