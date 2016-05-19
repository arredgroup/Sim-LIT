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
		id++;
		amount_elements = list.size();
		for(int i=0;i<amount_elements;i++){
			if(!aux->put((DataType*)list[i],i)){
				pkgs.push_back(*aux);
				aux = new Package(size,id);
				id++;
				aux->put((DataType*)list[i],i);
			}
		}
		pkgs.push_back(*aux);
		//cout <<"Nº PACKAGES " << pkgs.size()<<"\n";
		return pkgs;
	}

	vector<DataType *> packageListToImg(vector<Package> list, bool show_data, bool export_images, HEAD *header){
	/**
	 * @brief Método realiza el proceso de despacketizado sobre el vector de DataType
	 * @param list vector de DataType
	 */
		vector<DataType *> data(amount_elements,NULL);
		vector<DataType *> elements;
		vector<int> idElements;
		int pos=0;
		for(int i=0;i<list.size();i++){
			Package aux = list[i];
			elements = aux.getElements();
			idElements = aux.getIdElements();
			for (int j = 0; j < elements.size(); j++)
			{
				pos = idElements[j];
				data[pos]=elements[j];
			}
		}
		int good=0;
		for(int i=0;i<data.size();i++){
			if(data[i]!=NULL){
				good+=1;
			}
		}
		amount_elements = good;
		if(show_data)	
			cout << "N-DATATYPE-RECEIVED: " << good << "\n";
		if(export_images){
            Images aux(data,header,show_data);
            string path = header->folder+"/image_received_unforwared.bmp";
            aux.save(path.c_str());
        	aux.destroy();
        }
		return data;
	}

	int getSizeElements(){
		return amount_elements;
	}

};

#endif