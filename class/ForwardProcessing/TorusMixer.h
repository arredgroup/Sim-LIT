/**
  @file TorusMixer.h
  @class TorusMixer
  @brief Clase que implementa el Método de Entrelazamiento usando Torus Automosphisms
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_TORUSMIXER
#define CLASS_TORUSMIXER

#include <iostream>
#include <fstream> 
#include <sstream>
#include <stdlib.h>
#include <string>
#include "ForwardProcessing.h"
#include "../DataType.h"

using namespace std;

class TorusMixer: public ForwardProcessing{

	int proceed; //!<@brief Variable que indica si el método se realizó correctamente o no


public:

	TorusMixer(){}

	~TorusMixer(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_images){
	/**
	 * @brief Método aplica el entrelazamiento utilizando el Algoritmo Torus Automorphisms
	 * @param list Vector del tipo DataType que contiene la lista que se desea comprimir
	 * @param width Cantidad de Columnas que tiene la representación de la Imagen
	 * @param height Cantidad de Filas que tiene la representación de la Imagen
	 * @param parameters parámetros que requiere el compresor para funcionar
	 */
	 	int width = header->w;
	 	int height = header->h;
		if(nParameters!=2){
            cout << "(TORUS): Parameters number are incorrect! STOP!\n";
            return 1;
        }
		if(width!=height){
            cout << "(TORUS): Image not have the same high-width i.e.: (200px * 200px)!\n";
            return 1;
        }
		ofstream file;
        //string format = ".txt";
        //string path = "files/torus" + /*this->get() +*/ format;
		int n = atoi((char*)parameters[1]);
		if(n){
            cout << "(TORUS): n Parameter is minor than 0!\n";
			return 1;
		}
		int k = atoi((char*)parameters[0]);
		unsigned int M[2][2], TorusMatrice[2][2];

	    // Inicialización de la matriz con sus variables
	    TorusMatrice[0][0] = 1;
	    TorusMatrice[0][1] = 1;
	    TorusMatrice[1][0] = k;
	    TorusMatrice[1][1] = k+1;

	    // Procedimiento matemático
	    for(int count=1; count < n; count++)
	    {
	        M[0][0]=(TorusMatrice[0][0]+TorusMatrice[0][1]*k)%width;
	        M[0][1]=(TorusMatrice[0][0]+TorusMatrice[0][1]*(k+1))%width;
	        M[1][0]=(TorusMatrice[1][0]+TorusMatrice[1][1]*k)%width;
	        M[1][1]=(TorusMatrice[1][0]+TorusMatrice[1][1]*(k+1))%width;

	        TorusMatrice[0][0] = M[0][0];
	        TorusMatrice[0][1] = M[0][1];
	        TorusMatrice[1][0] = M[1][0];
	        TorusMatrice[1][1] = M[1][1];
	    }

		file.open("files/torus.txt",ofstream::out| ofstream::trunc);
		if(file.is_open()){
			file << width*height << "\n";
	        for(int i = 0; i < width; i++){
	            for(int j = 0; j < width; j++){
	                int iPos = ((i*width)+j);
	                int ip = (TorusMatrice[0][0]*i + TorusMatrice[0][1]*j)%width;
		            int jp = (TorusMatrice[1][0]*i + TorusMatrice[1][1]*j)%width;
		          	int fPos = ((ip*width)+jp);
		            DataType * aux = list->at(fPos);
	                list->at(fPos)=list->at(iPos);
	                list->at(iPos)=aux;
		            file << iPos << "-" << fPos << "\n";
		        }
		    }
		    if(show_data)
		 		cout << "Torus Automorphisms Applied to Image\n";
		   	proceed=true;
	   		file.close();
	   		if(export_images){
           		Images aux(*list,header,show_data);
            	string path = header->folder+"/image_TORUS_forwared.bmp";
            	aux.save(path.c_str());
            	aux.destroy();
        	}
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
        	string path = "files/torus.txt";
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
			    	cout << "Torus Automorphisms undo to Image\n";
				file.close();
				proceed=false;
				if(export_images){
           			Images aux(*list,header,show_data);
            		string path = header->folder+"/image_TORUS_unforwared.bmp";
            		aux.save(path.c_str());
            		aux.destroy();
        		}
			}
		}
		return 0;
	}

};


#endif