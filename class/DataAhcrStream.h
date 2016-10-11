/**
  @file DataTibsStream.h
  @class DataTibsStream
  @brief Clase que representa un Stream de Información JPG
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_DATAAHCRSTREAM
#define CLASS_DATAAHCRSTREAM

#include <iostream>
#include "DataType.h"

 using namespace std;

class DataAhcrStream: public DataType{

	 vector<double> element; //!<@brief Elemento que Compone al Byte
	 vector<int> positions;

public:

	DataAhcrStream(vector<double> nElement, vector<int> nPositions){
	/**
	 * @brief Constructor de la Clase DataBlock
	 * @param nBlock Píxeles que contiene un DataBlock
	 * @param w Ancho del Bloque
	 * @param h Alto del Bloque
	 * @param nAmount Cantidad de Canales que tendrá el bloque
	 * 
	 */
         element = nElement;
         positions = nPositions;
         this->setValid(true);
	}


	DataAhcrStream(){
	/**
	 * @brief Constructor de la Clase DataBlock
	*/	
		this->setValid(false);
	}

	~DataAhcrStream(){
	}

	double getSize(){
	/*
	*  @brief Método que devuelve el tamaño del Elemento
	*  @return result
	*/
		double size = element.size();
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

	vector<double> getBlock(){
		return element;
	}

	vector<int> getPositions(){
		return positions;
	}


};

#endif