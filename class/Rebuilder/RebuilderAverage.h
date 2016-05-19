/**
  @file RebuilderAverage.h
  @class RebuilderAverage
  @brief Clase que implementa un método de ocultamiento de errores utilizando el promedio de sus vecinos más cercanos
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/


#ifndef CLASS_REBUILDERAVERAGE
#define CLASS_REBUILDERAVERAGE

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "Rebuilder.h"
#include <iostream>
#include <vector>

using namespace std;

class RebuilderAverage: public Rebuilder{

public:

	RebuilderAverage(){};

	~RebuilderAverage(){};

	virtual void hidden(Images* img, bool show_data, bool export_images, HEAD *header){
	/** 
	* @brief Método que oculta los pixeles dañados y los reemplaza por el promedio de sus vecinos más cercanos
	* @param img Imagen sobre la que se le aplicará el método de ocultamiento de errores
	*
	*/
		DataBlock** matrix = img->getMatrix();
		int amount_channels = img->getType();
		int count;
		int w = img->getWidth()/img->getWidthBlock();
		int h = img->getHeight()/img->getHeightBlock();
		do{
			for(int i = 0; i < h; i=i+1)    
			{
	            for(int j = 0; j < w; j=j+1)
	            {
	                if(!matrix[i][j].isValid()) // si el píxel no es válido
	                {
						vector<int> add(amount_channels);
		                for(int x=0;x<amount_channels;x++)
		                	add[x]=0;
		                count = 0;
	                    // cálculo del promedio de los canales de los vecinos */
	                    for(int iBlock = max((int)i-1,0); iBlock <= min(i+1,h-1) ; iBlock++)   /**/
	                        for(int jBlock = max((int)j-1,0); jBlock <= min(j+1,w-1); jBlock++) /** 8 conectados */
	                            if((i != iBlock) || (j != jBlock))
	                                if(matrix[iBlock][jBlock].isValid()) // si el bloque vecino existe
	                                {
			                            vector<Pixel> *e = (vector<Pixel>*)matrix[iBlock][jBlock].getExtras();
	                                    for(int r=0;r<img->getHeightBlock();r++){
	                                    	for(int c=0;c<img->getWidthBlock();c++){
				                                vector<int> chnls = e->at(r*(img->getWidthBlock())+c).getChannels();
				                                for(int x=0;x<amount_channels;x++){
				                					add[x]+=chnls[x];
				                				}
	                                    		count=count+1;
	                                    	}
	                                    }
	                                }

	                    if(count!=0)
	                    {
	                        for(int x=0;x<amount_channels;x=x+1){
	                			add[x]=(int)(add[x]/count);
	                		}
	                		DataBlock* db = new DataBlock(amount_channels,add,img->getWidthBlock(),img->getHeightBlock());
	                		matrix[i][j]= *db;
	                    }
	                }
	            }
	        }
	    }while(isLossBlock(img));
	    if(export_images){
	    	string path = header->folder+"/image_received_restored.bmp";
            img->save(path.c_str());
	    }
	    if(show_data)
	    	cout << "The Image has been restored\n";
	}

private:

	bool isLossBlock(Images* img){
	/** 
	* @brief Método que valida si existen pixeles perdidos en la imagen
	* @param img Imagen sobre la que se buscan los pixeles perdidos
	* @return true si hay pixeles perdidos en la imagen
	* @return false si no hay pixeles perdidos en la imagen
	*/
		DataBlock** matrix = img->getMatrix();
		int w=img->getWidth()/img->getWidthBlock();
		int h=img->getHeight()/img->getHeightBlock();
		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				if(!matrix[i][j].isValid())
					return true;
			}
		}
		return false;
	}

};
#endif