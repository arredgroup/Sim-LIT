/**
  @file ChannelFile.h
  @class ChannelFile
  @brief Clase que representa el Simulador utilizando Un Fichero 
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_CHANNELFILE
#define CLASS_CHANNELFILE

#include <iostream>
#include <string>
#include <fstream> 
#include <sstream>
#include <vector>
#include "Channel.h"

class ChannelFile: public Channel{

public:

	ChannelFile(vector<string> extras){
	/*
	*  @brief Constructor de la clase Erlang
	*  @param ext argumentos que serán asignados al simulador
	*/
		setExtras(extras);
	}

	vector<Package> simulate(vector<Package> list){
	/*
	*  @brief Método que genera la pérdida de paquetes sobre list
	*  @param list Vector con los paquetes que se someterán a la simulación
	*  @return list;
	*/
		ifstream fp;
		if(getExtras().size()==0){
			cout << "File hasn't especific\n";
			return list;
		}
		string file = this->getExtras()[0];
		file = "../../"+file;
		fp.open(file.c_str());
		//cout << "Simulating Loss...\n";
		vector<string> array;
		string line, delimiter=" ";
		if(fp.is_open()){
			while(getline(fp, line)){
				array.push_back(line);
			}
			for (int i = 0; i < array.size(); i++)
			{
				string s = array.at(i);
				size_t pos=0;
				string token;
				pos = s.find(delimiter);
				token = s.substr(0, pos);
			    int element = atoi(token.c_str());
			    s.erase(0, pos + delimiter.length());
				pos = s.find(delimiter);
				token = s.substr(0, pos);
			    int opc = atoi(token.c_str());
			    if(opc==0)
					list = deletePackageById(element,list);
			}
			fp.close();
		}
		else{
			cout << "File Can't open\n";
		}
		return list; 
	}

    

private:

	vector<Package> deletePackageById(int ID, vector<Package> list){
		/*
		*  @brief Método que elimina un Paquete de la lista por su ID
		*  @param ID identificador del paquete que se eliminará
		*  @param list Lista de Paquetes del cual se eliminará uno por el ID
		*  @return list
		*/
		for (int i = 0; i < list.size(); i++)
		{
			if(list[i].getID()==ID){
				list.erase(list.begin()+i-1);
				i--;
			}
		}
		return list;
	}


};

#endif
