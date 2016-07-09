/**
  @file DataTibsStream.h
  @class DataTibsStream
  @brief Clase que representa un Stream de Información JPG
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_DATATIBSSTREAM
#define CLASS_DATATIBSSTREAM

#include <iostream>
#include "DataType.h"

 using namespace std;

class DataTibsStream: public DataType{

	 vector<vector<unsigned char>> element; //!<@brief Elemento que Compone al Byte

public:

	DataTibsStream(vector<vector<unsigned char>> nElement){
	/**
	 * @brief Constructor de la Clase DataBlock
	 * @param nBlock Píxeles que contiene un DataBlock
	 * @param w Ancho del Bloque
	 * @param h Alto del Bloque
	 * @param nAmount Cantidad de Canales que tendrá el bloque
	 * 
	 */
         element = nElement;
         this->setValid(true);
	}


	DataTibsStream(){
	/**
	 * @brief Constructor de la Clase DataBlock
	*/	
		this->setValid(false);
	}

	~DataTibsStream(){
	}

	double getSize(){
	/*
	*  @brief Método que devuelve el tamaño del Elemento
	*  @return result
	*/
		double size = 0;
		for (int i = 0; (unsigned)i < element.size(); i+=1){
			size += (double)element[i].size();
		}
		return size;
	}

	int* getContent(){
	/** 
	* @brief Función que devuelve en un arreglo de enteros todo el contenido de DataBlock
	* return array
	*/
		int* array = (int*)malloc(sizeof(int));
		if(this->isValid()){
			array[0]=0;
		}
		else{
			array[0]=1;
		}
		return array;
	}

	void* getExtras(){
	/**
	 * @brief Método que Obtiene el Dato del Byte
	 */
		return (void*)&element;
	}

	vector<unsigned char> getBlock(){
		return element;
	}


};

#endif