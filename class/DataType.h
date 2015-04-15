/**
  @file DataType.h
  @class DataType
  @brief Clase que representa un tipo de dato que será paquetizado
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_DATATYPE
#define CLASS_DATATYPE

#include <vector>
#include <iostream>

using namespace std;

class DataType{

	bool valid; //!<@brief Define si el Tipo de Dato es válido o no

public:

	DataType(){
	/*
	*  @brief Método Constructor de la clase DataType
	*/
		valid=false;
	}

	~DataType(){}
	
	virtual double getSize(){
	/**
	 * @brief Método que devuelve el tamaño del DataType
	 * @return 0
	 */
		return 0;
	};

	virtual int* getContent(){
	/**
	 * @brief Método que devuelve en un int* todo el contenido de DataType
	 * @return array
	 */
		int* array=NULL;
		return array;
	}

	virtual bool isValid(){
	/**
	 * @brief Método que indica si el DataType es Válido o no
	 * @return valid
	 */
		return valid;
	}

	virtual void setValid(bool nValid){
	/**
	 * @brief Método que actualiza el valor de la variable valid por nValid
	 * @param nValid Valor que toma en DataType (V o F)
	 */
		valid=nValid;
	}

	virtual void* getExtras(){
	/**
	* @brief Método que devuelve un valor determinado en cada clase heredera
	* @return NULL;
	*/
		return NULL;
	}

};


#endif