/**
  @file Package.h
  @class Package
  @brief Clase que Implementa la estructura de un Paquete
  @author Christopher Arredondo Flores
  @date 28/9/2014
**/

#ifndef CLASS_PACKAGE
#define CLASS_PACKAGE

#include <iostream>
#include <vector>

using namespace std;

class Package
{
private:

	int ID; //!<@brief Identificador del Paquete
	double size; //!<@brief Tamaño del Paquete
	vector<int> idElements; //!<@brief Vector que posee el un ID de cada elemento
	vector<DataType *> elements; //!<@brief vector que posee los elementos del paquete

public:

	Package(double nSize, int id){ 
	/**
	 * @brief Constructor de la Clase Package
	 * @param nSize Tamaño que tendrá el Paquete
	 * @param id ID que tendrá el Paquete
	 */
	 	//cout <<"PKG creado\n";
		size=nSize;
		ID=id;
	}

	Package(){
		ID=-1;
		size=0;
	}
	
	~Package(){}

	bool put(DataType* x, int pos){
	/**
	 * @brief Método que agrega Elementos al Paquete
	 * @param x Elemento DataType que se desea ingresar en el paquete
	 * @param pos Posición del elemento x en su lista original
	 * @return devuelve un valor verdadero si el elemento si se agregó correctamente 
	 */
		//cout << "\t SIZE: "<< x->getSize() << "\n";
		if((actualSize()+(x->getSize()))<=size){
			elements.push_back(x);
			idElements.push_back(pos);
			return true;
		}
		else{
			//cout << "NO HAY ESPACIO SUFICIENTE\n";
			return false;
		}
	}

	vector<DataType *> getElements(){
		return elements;
	}

	int getID(){
	/**
	 * @brief Método que devuelve el ID del Paquete
	 * @return ID 
	 */	
		return ID;
	}

	vector<int> getIdElements(){
	/**
	 * @brief Método que devuelve el vector con los ID de los elementos del paquete
	 * @return IdElements
	 */	
		return idElements;
	}


	double actualSize(){
	/**
	 * @brief Método que devuelve el tamaño actual del Paquete
	 * @return size
	 */	
		 double size=0;
		 for(int i=0;i<elements.size();i++){
		 	size+=elements[i]->getSize();
		 }
	return size;	
	}

};

#endif