/**
  @file TorusMixer.h
  @class TorusMixer
  @brief Clase que implementa el Método de Entrelazamiento usando Torus Automosphisms
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_MIXERFROMFILE
#define CLASS_MIXERFROMFILE

#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include "../ForwardProcessing.h"
#include "../DataType.h"

using namespace std;

class MixerFromFile: public ForwardProcessing{

	int proceed; //!<@brief Variable que indica si el método se realizó correctamente o no


public:

	MixerFromFile(){}

	~MixerFromFile(){}

	int make(vector<DataType *> *list, int width, int height, vector<int> parameters, int nParameters, bool show_data){
	/**
	 * @brief Método aplica el entrelazamiento utilizando el un Archivo con la secuencia de mezcla
	 * @param list Vector del tipo DataType que contiene la lista que se desea comprimir
	 * @param width Cantidad de Columnas que tiene la representación de la Imagen
	 * @param height Cantidad de Filas que tiene la representación de la Imagen
	 * @param parameters parámetros que requiere el compresor para funcionar
	 */
	 	if(nParameters!=1)
	 		return 2;
	 	if(parameters.size()!=1)
	 		return 3;
	 	char str[3];
		sprintf(str, "%d", parameters.at(0));
		ifstream read;
		ofstream save;
		string line;
        string path = "class/MixerFromFile/"+ string(str) +".mixer";
		read.open(path.c_str());
		save.open("files/mff.txt",ofstream::out| ofstream::trunc);
		if(save.is_open() && read.is_open()){
			save << list->size() << "\n";
			getline(read, line);
			vector<string> v = explode(line, ' ');
	        for(int i = 0; i < list->size() && i < v.size(); i++){
		        int iPos = i;
		        int fPos = atoi(v.at(i).c_str());
		        DataType * aux = list->at(fPos);
		        list->at(fPos)=list->at(iPos);
		        list->at(iPos)=aux;
		        save << iPos << "-" << fPos << "\n";
		    }
		    if(show_data)
		    	cout << "Mixer From File Applied to Image\n";
		    proceed=true;
		    	//this->set(1);
		}
	    save.close();
	    read.close();
	    return 0;
	}

	int unmake(vector<DataType *> *list, bool show_data){
	/**
	 * @brief Método deshace el entrelazamiento utilizando el Algoritmo Torus Automorphisms
	 */
	 	if(proceed){
			vector<string> array;
			ifstream file;
			string line, delimiter="-";
			int iPos, fPos;
        	string path = "files/mff.txt";
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
			    	cout << "Mixer From File undo to Image\n";
				file.close();
				proceed=false;
			}
		}
		return 0;
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

};


#endif