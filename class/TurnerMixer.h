/**
  @file TurnerMixer.h
  @class TurnerMixer
  @brief Clase que implementa el Método de entrelazamiento de Turner
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_TURNERMIXER
#define CLASS_TURNERMIXER

#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include "ForwardProcessing.h"
#include "TurnerMixer.h"

using namespace std;

class TurnerMixer: public ForwardProcessing{

	bool proceed; //!<@brief Variable que indica si el método se realizó correctamente o no


public:

	TurnerMixer(){}

	~TurnerMixer(){}

	int make(vector<DataType *> *list, int width, int height, vector<int> parameters, int nParameters, bool show_data){
	/**
	 * @brief Método aplica el entrelazamiento utilizando el Algoritmo de Turner And Peterson
	 * @param list Vector del tipo DataType que contiene la lista que se desea comprimir
	 * @param width Cantidad de Columnas que tiene la representación de la Imagen
	 * @param height Cantidad de Filas que tiene la representación de la Imagen
	 * @param parameters parámetros que requiere el compresor para funcionar
	 */
		if(nParameters != 3) return 1;
		int byteOffset = parameters[0];
        int packetOffset = parameters[1];
        int payload = parameters[2];
        if((byteOffset < 0) || (packetOffset < 0)) return 5;
		ofstream file;
		file.open("files/turner.txt",ofstream::out| ofstream::trunc);
		if(file.is_open()){
			file << height*width << "\n";
	        int i = 0, j = 0, k = 0, m = 0;
		    for(int h = 0; h < height; h=h+1)
		    {
		        for(int w = 0; w < width; w=w+1)
		        {
		            if(!(m%payload) && k)
		            {
		                i=i+1; 
		                m = 0;
		                j = i*packetOffset; // i: número de paquete
		            }

		            int ip = ((j+(m*byteOffset))/height)%height;
		            int jp = (j+(m*byteOffset))%width;
		            int iPos = ((h*width)+w);
		            int fPos = ((ip*width)+jp);
		            DataType * aux = list->at(fPos);
		            list->at(fPos)=list->at(iPos);
	                list->at(iPos)=aux;
	                file << iPos << "-" << fPos << "\n";
		            k=k+1; 
		            m=m+1;
		        }
		    }
		    if(show_data)
	        	cout << "Turner And Peterson Applied to Image\n";
	        proceed=true;
	        //this->set(1);
	    	file.close();
	    }
	    return 0;
	}

	int unmake(vector<DataType *> *list, bool show_data){
	/**
	 * @brief Método que deshace el entrelazamiento utilizando el Algoritmo de Turner And Peterson
	 */
	 	if(proceed){
			vector<string> array;
			ifstream file;
			string line, delimiter="-";
			int iPos, fPos;
        	string path = "files/turner.txt";
        	file.open(path.c_str());
			if(file.is_open()){
				getline(file, line);
				int width = atoi(line.c_str());
				for(int x=0;x<width;x++){
					getline(file, line);
					array.push_back(line);
				}
				for(int x=0;x<width;x++){
					string s = array.back();
					array.pop_back();
					size_t pos=0;
					string token;
					pos = s.find(delimiter);
					token = s.substr(0, pos);
				    iPos = atoi(token.c_str());
				    s.erase(0, pos + delimiter.length());
					pos = s.find(delimiter);
					token = s.substr(0, pos);
				    fPos = atoi(token.c_str());
				    s.erase(0, pos + delimiter.length());
		    		DataType * aux = list->at(fPos);
			        list->at(fPos)=list->at(iPos);
			        list->at(iPos)=aux;
				}
		        if(show_data)
		        	cout << "Turner And Petterson undo to Image\n";
				file.close();
				proceed=false;
		    }
		}
		return 0;
	}

};

#endif