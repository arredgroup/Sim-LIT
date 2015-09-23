/**
  @file Images.h
  @class Images
  @brief Clase que representa una Imagen
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_IMAGES
#define CLASS_IMAGES

#include <string>
#include "DataBlock.h"
#include "DataType.h"
#include "CImg.h"
#include <cstddef>
#include <vector>

using namespace std;
using namespace cimg_library; 

class Images
{
private:
	DataBlock** matrix; //!<@brief Matriz de Bloques de Píxeles (DataBlock) que componen la imagen
	int w; //!<@brief Cantidad de columnas que tiene la imagen
	int h; //!<@brief Cantidad de filas que tiene la imagen
	int type; //!<@brief define la cantidad de canales que tiene la imagen
	int wblock; //!<@brief Cantidad de Columnas que tiene cada bloque de la imagen
	int hblock; //!<@brief Cantidad de Filas que tiene cada bloque de la imagen

public:
    ~Images(){}

    Images(vector<DataType *> list, int nType, int nwidth, int nheight, int nwblock, int nhblock, bool show_data){
    /*
	*  @brief Método Constructor de la Clase Images
	*  @param list Lista de Elementos con los cuales se creará la nueva Imagen
	*  @param nType Cantidad de canales que tendrá la Imagen
	*  @param nwidth Cantidad de Columnas que tendrá la nueva imagen
	*  @param nheight Cantidad de Filas que tendrá la nueva imagen
	*/
    	type=nType;
    	w=nwidth;
    	h=nheight;
    	wblock=nwblock;
		hblock=nhblock; 
		if((w%wblock!=0 || h%hblock!=0) || wblock!=hblock){
			wblock=1;
    		hblock=1;
    	}
	    matrix = new DataBlock*[h/hblock];
	    for (int i = 0; i < h/hblock; i=i+1)
	    {
	    	matrix[i] = new DataBlock[w/wblock];
	    	for (int j = 0; j < w/wblock; j=j+1)
	    	{	
	    		int pos = (i*(int)(h/hblock))+j;
	    		if(list.at(pos)!=NULL && list.at(pos)->isValid()){
			   		vector<Pixel> *e = (vector<Pixel>*)list.at(pos)->getExtras();
			    	vector<Pixel> lblock(wblock*hblock);
			    	for(int k=0;k<(wblock*hblock);k++){
			    		int* content = e->at(k).getContent();
				   		if(content[0]==0){
				    		vector<int>channels;
				    		for(int	x=0;x<content[1];x++){
					    			channels.push_back(content[2+x]);
				    		}
			    			Pixel transformed(channels, type);					    			
			    			lblock[k]=transformed;
				    	}
				    	else{
				    		Pixel transformed;
				    		lblock[k]=transformed;
				    	}
			    	}
		    		DataBlock block(lblock, wblock,hblock,type);
		    		matrix[i][j] = block;
		    	}
	    	}
	    }
	    if(show_data)
	    	cout << "Image Generated...\n";
	    printf("SIZE: %lu \n", list.size());
    }

	Images(string url, int nwblock, int nhblock, bool show_data){
		/*
		* @brief Método Constructor de la clase Images
		* @param url Fuente de origen desde donde se importará la Imagen
		*/
		wblock=nwblock;
		hblock=nhblock;
		CImg<> img(url.c_str());
		const CImg<unsigned char> R = img.get_shared_channel(0),
                        	  	G = img.get_shared_channel(1),
                          		B = img.get_shared_channel(2);
		if (R==G && R==B) {
		    type = 1;
		} else {
			type=img.spectrum();
		}
		cout<<"TIPO: "<<type<<"\n";
	    w = img.width();
	    h = img.height();
	    if((w%wblock!=0 || h%hblock!=0) || wblock!=hblock){
	    	wblock=1;
	    	hblock=1;
	    }
	    //cout << "W:"<<w<<"---"<<"H:"<<h<<"\n";
	    matrix = new DataBlock*[(int)h/hblock];
	    for (int i = 0; i < h/hblock; i=i+1)
	    {
	    	//cout << "img ciclo for 1: " << i << "\n";
	    	matrix[i] = new DataBlock[(int)w/wblock];
	    	for (int j = 0; j < w/wblock; j=j+1)
	    	{	
	    		vector<Pixel> list(hblock*wblock);
			    for(int f=0;f<hblock;f++){
			  		for(int c=0;c<wblock;c++){
						vector<int> channels(type);
					    for(int x=0;x<type;x++){
					    	channels[x]=(int)img(((j*wblock)+c),((i*hblock)+f),0,x);
					    }
					    Pixel aux(channels,type);
					    list[(f*wblock)+c]=aux;
					}
				}
	    		DataBlock* aux = new DataBlock(list,wblock,hblock,type);
	    		matrix[i][j] = *aux;
	    	}
	    }
	    if(show_data)
	    	cout << "Image Loaded...\n";
	}

	int getWidthBlock(){
	/*
		@brief Método que devuelve la cantidad de Columnas de cada bloque
	*/
		return wblock;
	}

	int getHeightBlock(){
	/*
		@brief Método que devuelve la cantidad de Filas de cada Bloque
	*/
		return hblock;
	}

	int getWidth(){
	/*
		@brief Método que devuelve la cantidad de Columnas de la Imagen
	*/
		return w;
	}

	int getHeight(){
	/*
		@brief Método que devuelve la cantidad de Filas de la Imagen
	*/
		return h;
	}

	DataBlock** getMatrix(){
	/*
		@brief Método que devuelve la Matriz de Píxeles de la Imagen
	*/
		return matrix;
	}

	int getType(){
	/*
		@brief Método que devuelve la cantidad de canales que posee la Imagen
	*/
		return type;
	}

	vector<DataType *> toList(){
	/*
		@brief Método que devuelve en un Vector la matriz de DataBlockes de la Imagen
	*/
		vector<DataType *> list;
		for (int i = 0; i < h/hblock; i++)
		{
			for (int j = 0; j < w/wblock; j++)
			{
				DataBlock* aux = &matrix[i][j];
				list.push_back(aux);
			}
		}
		return list;
	}

	void save(const char* destiny){	
	/*
		@brief Método que Genera una Imagen en formato BMP de la Imagen
		@param destiny Destino donde se guardará la Imagen
	*/
		CImg<> img(w,h,1,type,0);
		for (int i = 0; i < h/hblock; i++){
			for (int j = 0; j < w/wblock; j++){	
				if(matrix[i][j].isValid()){
					vector<Pixel> *e = (vector<Pixel>*)matrix[i][j].getExtras();
					for(int r=0;r<hblock;r++){
						for(int c=0;c<wblock;c++){
							vector<int> v_rgb = (e->at((r*hblock)+c)).getChannels();
							int* rgb = (int*)malloc(sizeof(int)*v_rgb.size());
							for(int x=0;x<v_rgb.size();x++){
								rgb[x]=v_rgb[x];
							}
							img.draw_point((j*wblock)+c,(i*hblock)+r,0,rgb);
						}
					}
				}
			}
		}
		img.normalize(0, 255);
		img.save(destiny);
	}

private:
	vector<Pixel> getPixelPerBlock(CImg<> img, int x, int y){
	/*
	  @brief Método que importa los canales de un determinado DataBlock a la Imagen
	  @param img Imagen de la cual se extraerán los canales del DataBlock
	  @param x Nº de Columna del DataBlock
	  @param y Nº de Fila del DataBlock
	*/	
	  	vector<Pixel> list(hblock*wblock);
	    for(int i=0;i<hblock;i++){
	  		for(int j=0;j<wblock;j++){
				vector<int> channels(type);
			    for(int c=0;c<type;c++){
			    	channels[c]=(int)img(((y*wblock)+j),((x*hblock)+i),0,c);
			    }
			    Pixel aux(channels,type);
			    list[(i*wblock)+j]=aux;
			}
		}
		return list;
	}
};

#endif