/**
  @file MixerFromFile.h
  @class MixerFromFile
  @brief Clase que implementa el Método de Entrelazamiento usando un Fichero .mixer
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_MIXERFROMFILE
#define CLASS_MIXERFROMFILE

#include <iostream>
#include <fstream> 
#include <sstream>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include "../ForwardProcessing.h"
#include "../../DataType.h"

using namespace std;

class MixerFromFile: public ForwardProcessing{

	int proceed; //!<@brief Variable que indica si el método se realizó correctamente o no


public:

	MixerFromFile(){
	}

	~MixerFromFile(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_images){
	/**
	 * @brief Método aplica el entrelazamiento utilizando el un Archivo con la secuencia de mezcla
	 * @param list Vector del tipo DataType que contiene la lista que se desea comprimir
	 * @param width Cantidad de Columnas que tiene la representación de la Imagen
	 * @param height Cantidad de Filas que tiene la representación de la Imagen
	 * @param parameters parámetros que requiere el compresor para funcionar
	 */
	 	if(nParameters!=1){
            cout << "(FILEMIX): Parameters number are incorrect! STOP!\n";
	 		return 1;
	 	}
		ifstream read;
		ofstream save;
		string line;

		int width = header->w;
		int height = header->h;

		vector<DataType *> newList(width*height,NULL);
		string file = (char *)parameters.at(0);
        string path ="class/MixerFromFile/" +  file;
		read.open(path.c_str());
		save.open("files/mff.txt",ofstream::out| ofstream::trunc);
		if(save.is_open() && read.is_open()){
			save << list->size() << "\n";
			/*getline(read, line);
			vector<string> v = explode(line, ' ');
	        for(int i = 0; i < list->size() && i < v.size(); i++){
		        int iPos = i;
		        int fPos = atoi(v.at(i).c_str());
		        DataType * aux = list->at(fPos);
		        list->at(fPos)=list->at(iPos);
		        list->at(iPos)=aux;
		        save << iPos << "-" << fPos << "\n";
		    }*/
		    for (int i = 0; i < list->size() && !read.eof(); i++)
		    {
		    	getline(read, line);
			    vector<string> v = explode(line,'-');
			    for (int j = 0; i < v.size(); ++j)
			    	remove(v[j].begin(), v[j].end(), ' ');
			    vector<string> posInit = explode(v[0],',');
			    int piX=atoi(posInit[0].c_str());
			    int piY=atoi(posInit[1].c_str());
			    vector<string> posEnd = explode(v[1],',');
			    int peX=atoi(posEnd[0].c_str());
			    int peY=atoi(posEnd[1].c_str());
			    newList.at((peX*width)+peY) = list->at((piX*width)+piY);
		        save << ((piX*width)+piY) << "-" << ((peX*width)+peY) << "\n";
			    
		    }
		    list = &newList;
		    if(show_data)
		    	cout << "Mixer From File Applied to Image\n";
		    proceed=true;
		    	//this->set(1);
		}
	    save.close();
	    read.close();
        if(export_images){
            Images aux(*list,header,show_data);
            string path = header->folder+"/image_FILEMIX_forwared.bmp";
            aux.save(path.c_str());
            aux.destroy();
        }
	    return 0;
	}

	int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_images){
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
				if(export_images){
            		Images aux(*list,header,show_data);
            		string path = header->folder+"/image_FILEMIX_unforwared.bmp";
            		aux.save(path.c_str());
            		aux.destroy();
        		}
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