/**
  @file DataByte.h
  @class DataByte
  @brief Clase que representa un Byte de Información
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_DATABYTESTREAM
#define CLASS_DATABYTESTREAM

#include <iostream>
#include "DataType.h"

 using namespace std;

class DataByteStream: public DataType{

	 vector<unsigned char*> element; //!<@brief Elemento que Compone al Byte
	 vector<int> huffman_bytes;
	 vector<int> original_bytes;

public:

	DataByteStream(vector<unsigned char*> nElement, vector<int> oBytes, vector<int> nBytes){
	/**
	 * @brief Constructor de la Clase DataBlock
	 * @param nBlock Píxeles que contiene un DataBlock
	 * @param w Ancho del Bloque
	 * @param h Alto del Bloque
	 * @param nAmount Cantidad de Canales que tendrá el bloque
	 * 
	 */
	 	 huffman_bytes = nBytes;
	 	 original_bytes = oBytes;
         element = nElement;
         this->setValid(true);
	}


	DataByteStream(){
	/**
	 * @brief Constructor de la Clase DataBlock
	*/	
		this->setValid(false);
	}

	~DataByteStream(){
	}

	double getSize(){
	/*
	*  @brief Método que devuelve el tamaño del Elemento
	*  @return result
	*/
		double size = 0.0;
		for (int i = 0; (unsigned)i < huffman_bytes.size(); i+=1){
			size += (double)huffman_bytes[i];
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

	vector<unsigned char*> getHuffman(){
		return element;
	}

	vector<int> getOriginalBytes(){
		return original_bytes;
	}

	vector<int> getHuffmanBytes(){
		return huffman_bytes;
	}

};

#endif