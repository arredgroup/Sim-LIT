/**
  @file DsjalMixer.h
  @class DsjalMixer
  @brief Clase que implementa el Método de entrelazamiento de DSJ-AL
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_DSJALMIXER
#define CLASS_DSJALMIXER

#include <iostream>
#include <fstream> 
#include <stdlib.h>
#include <sstream>
#include <string>
#include "ForwardProcessing.h"

using namespace std;

class DsjalMixer: public ForwardProcessing{

    bool proceed; //!<@brief Variable que indica si el método se realizó correctamente o no

public:

    DsjalMixer(){
    /*
    *  @brief Método Constructor de la clase DsjalMixer
    */
    }

    ~DsjalMixer(){}

    int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_images){
    /**
     * @brief Método aplica el entrelazamiento utilizando el Algoritmo de Dsjal
     * @param list Vector del tipo DataType que contiene la lista que se desea comprimir
     * @param width Cantidad de Columnas que tiene la representación de la Imagen
     * @param height Cantidad de Filas que tiene la representación de la Imagen
     * @param parameters parámetros que requiere el compresor para funcionar
     */
        if(nParameters!=1){
            cout << "(DSJAL): Parameters number are incorrect! STOP!\n";
            return 1;
        }
        int step = atoi((char*)parameters[0]);
        if(step<=0){
            cout << "(DSJAL): Step Parameter is minor than 0! STOP!\n";
            return 1;
        }
        int i, j, k, _h_, _w_;
        DataType * aux; 
        _h_=0;
        _w_=0;
        int width = (header->w)/(header->wb);
        int height = (header->h)/(header->hb);
        int indexH[(height*width)];
        int indexW[(height*width)];
        for (i = 0, k=0; i < height; i=i+1)
        {
            for (j = 0; j < width; j=j+1, k=k+1)
            {
                indexH[k]=i;
                indexH[k]=j;
            }
        }
        ofstream file;
        string path = "files/Dsjal.txt";
        file.open(path.c_str(),ofstream::out| ofstream::trunc);
        if(file.is_open()){
            file << (height*width) << "\n";

            indexH[0] = 0; _h_ = indexH[0];
            indexW[0] = 0; _w_ = indexW[0];

            for(i = 0, k = 0; i < height; i=i+1)
                for(j = 0; j < width; j=j+1, k=k+1)
                {
                    indexW[k+1] = (indexW[k]+step)%width;
                    indexH[k+1] = indexH[k] + (int)((indexW[k]+step)/width);

                    /* si Yi+1 (index_h[k+1]) alcanza el máximo de filas, las próximas coordenadas será la posición siguiente al primer píxel guardado,y
                     * así sucesivamente.
                     */
                    if(indexH[k+1] >= height)
                    {
                        indexH[k+1] =  _h_ + (int)((_w_)/(width-1));    // x
                        _h_ = indexH[k+1];

                        indexW[k+1] = (_w_+1)%width; // y
                        _w_ = indexW[k+1];
                    }
                }

            indexH[0] = 0; // al finalizar indexW se hace con el valor de Width
            indexW[0] = 0; // por lo que se sobreescribe con valores iniciales.

            for(k=0;k<(width*height);k=k+1){
                    
                    int fPos = ((indexH[k]*width)+indexW[k]);
                    aux = list->at(fPos);
                    list->at(fPos)=list->at(k);
                    list->at(k)=aux;
                    file << k << "-" << fPos << "\n";
                }
            if(show_data)
                cout << "Dsjal Applied to Image\n";
            proceed=true;
            //this->set(1);
            file.close();
            if(export_images){
                Images aux(*list,header,show_data);
                string path = header->folder +"/image_DSJAL_forwared.bmp";
                aux.save(path.c_str());
                aux.destroy();
            }
        }
        return 0;
    }

    int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_images){
    /**
     * @brief Método que deshace el entrelazamiento utilizando el Algoritmo de Dsjal
     */
        if(proceed){
            vector<string> array;
            ifstream file;
            string line, delimiter="-";
            int iPos, fPos;
            string path = "files/Dsjal.txt";
            file.open(path.c_str());
            if(file.is_open()){
                getline(file, line);
                int width = atoi(line.c_str());
                for(int x=0;x<width;x++){
                    getline(file, line);
                    array.push_back(line);
                }
                for(int x=0;x<width;x=x+1){
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
                    cout << "Dsjal undo to Image\n";
                //this->set(-1);
                file.close();
                proceed=false;
                if(export_images){
                    Images aux(*list,header,show_data);
                    string path = header->folder +"/image_DSJAL_unforwared.bmp";
                    aux.save(path.c_str());
                    aux.destroy();
                }
            }
        }
        return 0;
    }

};

#endif