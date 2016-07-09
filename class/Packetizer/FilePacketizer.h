/**
  @file FilePacketizer.h
  @class FilePacketizer
  @brief Clase que define un Paquetizador básico
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_FILEPACKETIZER
#define CLASS_FILEPACKETIZER

#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include <stdlib.h>
#include "../DataType.h"
#include "Packetizer.h"

using namespace std;

class FilePacketizer: public Packetizer{

	int amount_elements; //!<@brief Cantidad de Elementos Paquetizados

public:

	~FilePacketizer(){}

	FilePacketizer(){
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

		ifstream read;
		int id=0, sizeElement=0, nPkg=0;
		string path="files/demo.txt";
		if(extras!=NULL)
        	path =(char*)extras;
		if(show_data)
			cout << "N-DATATYPE-SENDED: " << list.size() << "\n";
		amount_elements = list.size();
		if(amount_elements>0)
			sizeElement=list.at(0)->getSize();
		int elements_by_pkg = floor(size / sizeElement);
		nPkg = ceil(amount_elements / elements_by_pkg);
		//nPkg = (sizeElement*amount_elements)/size; //Calcula la cantidad de paquetes
		vector<Package> pkgs;
		for (int i = 0; i < nPkg; ++i)
		{
			Package* aux = new Package();
			pkgs.push_back(*aux);
		}
		do{
			read.open(path.c_str());
		}while(!read.is_open());
		for (int i = 0; i < amount_elements && !read.eof(); i++)
		{
			string line;
			getline(read, line);
			vector<string> v = split(line,"-");
		    if(v.size()>0){
		    	int pi = atoi(v[0].c_str());
		    	int posPkg = atoi(v[1].c_str());
		    	if(pkgs[posPkg].getID()==-1){
					Package* aux = new Package(size,id);
					pkgs[posPkg] = *aux;
					id++;
		    	}
		    	pkgs[posPkg].put((DataType*)list[pi],pi);
		    }
		}
		read.close();
		if(show_data)
			cout <<"N-PACKAGES: " << pkgs.size()<<"\n";
		return pkgs;
	}

	vector<DataType *> packageListToImg(vector<Package> list, bool show_data, HEAD *header){
	/**
	 * @brief Método realiza el proceso de despacketizado sobre el vector de DataType
	 * @param list vector de DataType
	 */
	 	if(show_data)
	 		cout << "N-PACKAGE-RECEIVED: " << list.size() << "\n";
		vector<DataType *> data(amount_elements,NULL);
		vector<DataType *> elements;
		vector<int> idElements;
		int pos=0;
		for(int i=0;(unsigned)i<list.size();i++){
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
		for(int i=0;(unsigned)i<data.size();i++){
			if(data.at(i)!=NULL){
				good+=1;
			}
		}
		amount_elements = good;
		if(show_data)	
			cout << "N-DATATYPE-RECEIVED: " << good << "\n";
		return data;
	}

	int getSizeElements(){
		return amount_elements;
	}

private:

	std::vector<std::string> explode(std::string const & s, char delim)
	{
	    std::vector<std::string> result;
	    std::istringstream iss(s);

	    for (std::string token; std::getline(iss, token, delim); )
	    {
	        result.push_back(std::move(token));
	    }

	    return result;
	}

	vector<string> split(string s, string delimiter){
		vector<string> aux;
		size_t pos=0;
		string token;
		pos = s.find(delimiter);
		token = s.substr(0, pos);
	    aux.push_back(token);
		s.erase(0, pos + delimiter.length());
		pos = s.find(delimiter);
		token = s.substr(0, pos);
		aux.push_back(token);
		//s.erase(0, pos + delimiter.length());
		return aux;
	}

};

#endif