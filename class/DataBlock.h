/**
  @file DataBlock.h
  @class DataBlock
  @brief Clase que representa un Bloque de Píxeles
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_DATABLOCK
#define CLASS_DATABLOCK

#include <iostream>
#include <vector>
#include "DataType.h"
#include "Pixel.h"

 using namespace std;

class DataBlock: public DataType{

	vector<Pixel> block; //!<@brief Datos que forman el DataBlock
	int width; //!<@brief Cantidad de Píxeles por Ancho del bloque
	int height; //!<@brief Cantidad de Píxeles por Alto del bloque
	int amount_channels; //!<@brief Cantidad de Canales que tiene el bloque

public:

	DataBlock(vector<Pixel> nBlock, int w, int h, int nAmount){
	/**
	 * @brief Constructor de la Clase DataBlock
	 * @param nBlock Píxeles que contiene un DataBlock
	 * @param w Ancho del Bloque
	 * @param h Alto del Bloque
	 * @param nAmount Cantidad de Canales que tendrá el bloque
	 * 
	 */
         block = nBlock;
         width = w;
         height = h;
         amount_channels = nAmount;
         this->setValid(true);
	}

	DataBlock(int nAmount, vector<int> chns, int w, int h){
	/**
	 * @brief Constructor de la Clase DataBlock
	 * @param nAmount Cantidad de Canales que tendrá el bloque
	 * @param chns Datos que contiene un DataBlock
	 * @param w Ancho del Bloque
	 * @param h Alto del Bloque
	 * 
	 */
        width = w;
        height = h;
        for (int i = 0; i < width*height; i=i+1)
        {
        	Pixel aux(chns,nAmount);
        	block.push_back(aux);
        }
        this->setValid(true);
	}

	DataBlock(){
	/**
	 * @brief Constructor de la Clase DataBlock
	*/	
		width=1;
		height=1;
		amount_channels=0;	
		this->setValid(false);
	}

	~DataBlock(){
	}

	double getSize(){
	/*
	*  @brief Método que devuelve el tamaño del Elemento
	*  @return result
	*/
		double result = 0;
		for (int i = 0; i < (width*height); i=i+1)
		{
			result+= block[i].getSize();
		}
		return result;
	}

	int* getContent(){
	/** 
	* @brief Función que devuelve en un arreglo de enteros todo el contenido de DataBlock
	* return array
	*/
		int* array = (int*)malloc(sizeof(int)*3);
		if(this->isValid()){
			array[0]=0;
		}
		else{
			array[0]=1;
		}
		array[1]=height;
		array[2]=width;
		/*for (int i = 0; i < (width*height); i++)
		{
			for (int j = 0; j < amount_channels; j++)
			{
				array[3+(i*amount_channels)+j]=block[i].getChannels()[j];
			}
		}*/
		return array;
	}

	int getIntensity(){
	/** 
	* @brief Método que obtiene la Intensidad del Bloque de Píxeles
	* return local_max
	*/
	     int local_max = 0;
	     for (int i = 0; i < (width*height); ++i)
	     {
	     	local_max = local_max + block[i].getIntensity();	     
	     }
	     
	     local_max = (int)(local_max/(width*height));
	     return local_max;
	}

	void setAmountChannels(int aChannels){
	/**
	 * @brief Método que Modifica la cantidad de canales del Bloque
	 */
		amount_channels = aChannels;
		for(int i=0;i<(height*width);i++){
			block[i].setAmountChannels(amount_channels);
		}
	}

	void setChannels(vector<int> nChannels){
	/**
	 * @brief Método que Modifica los canales del Pixel
	 */
		for(int i=0;i<(height*width);i++){
			block[i].setChannels(nChannels);
		}
	}

	void* getExtras(){
	/**
	 * @brief Método que Obtiene el los Datos del Bloque
	 */
		return (void*)&block;
	}

};

#endif