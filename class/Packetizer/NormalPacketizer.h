/**
  @file NormalPacketizer.h
  @class NormalPacketizer
  @brief Clase que define un Paquetizador básico
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_NORMALPACKETIZER
#define CLASS_NORMALPACKETIZER

#include <vector>
#include <iostream>
#include "../DataType.h"
#include "Packetizer.h"

using namespace std;

class NormalPacketizer: public Packetizer{

	int amount_elements; //!<@brief Cantidad de Elementos Paquetizados

public:

	~NormalPacketizer(){}

	NormalPacketizer(){
	/**
	* @brief Constructor de la clase NormalPacketizer	
	*/
	}

	vector<Package> imgToPackageList(vector<DataType *> list, int size, bool show_data, void* extras = NULL){
	/**
	 * @brief Método realiza el proceso de packetizado sobre el vector de DataType
	 * @param list vector de DataType
	 * @param size tamaño que tendrá cada paquete
	 */
		vector<Package> pkgs;
	    if(show_data)
			cout << "N-DATATYPE-SENDED: " << list.size() << "\n";
		int id=0;
		Package* aux = new Package(size,id);
		id+=1;
		amount_elements = list.size();
		for(int i=0;i < amount_elements; i+=1){
			DataType* element = list.at(i);
			if(!aux->put(element,i)){
				pkgs.push_back(*aux);
				aux = new Package(size,id);
				if(!aux->put(element,i)){
					aux = new Package(element->getSize()+1.0,id);
					aux->put(element,i);
				}
				id+=1;
			}
		}
		pkgs.push_back(*aux);
		if(show_data)
			cout <<"Nº PACKAGES " << pkgs.size()<<"\n";
		return pkgs;
	}

	vector<DataType *> packageListToImg(vector<Package> list, bool show_data, HEAD *header){
	/**
	 * @brief Método realiza el proceso de despacketizado sobre el vector de DataType
	 * @param list vector de DataType
	 */
		vector<DataType *> data(amount_elements,NULL);
		vector<DataType *> elements;
		vector<int> idElements;
		int pos=0;
		for(int i=0;(unsigned)i<list.size();i+=1){
			Package aux = list[i];
			elements = aux.getElements();
			idElements = aux.getIdElements();
			for (int j = 0; (unsigned)j < elements.size(); j++)
			{
				pos = idElements[j];
				data[pos]=elements[j];
			}
		}
		int good=0;
		for(int i=0;(unsigned)i<data.size();i+=1){
			if(data[i]!=NULL)
				good+=1;
		}
		amount_elements = good;
		if(show_data)	
			cout << "N-DATATYPE-RECEIVED: " << good << "\n";
		return data;
	}

	int getSizeElements(){
		return amount_elements;
	}

};

#endif