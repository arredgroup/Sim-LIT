/**
  @file RebuilderAverage.h
  @class RebuilderAverage
  @brief Clase que implementa un método de ocultamiento de errores utilizando el promedio de sus vecinos más cercanos
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/


#ifndef CLASS_REBUILDERAVERAGEPIXEL
#define CLASS_REBUILDERAVERAGEPIXEL

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "Rebuilder.h"
#include <iostream>
#include <vector>

using namespace std;

class RebuilderAveragePixel: public Rebuilder{

public:

	RebuilderAveragePixel(){};

	~RebuilderAveragePixel(){};

	virtual void hidden(Images* img, bool show_data, bool export_images, HEAD *header){
	/** 
	* @brief Método que oculta los pixeles dañados y los reemplaza por el promedio de sus vecinos más cercanos
	* @param img Imagen sobre la que se le aplicará el método de ocultamiento de errores
	*
	*/
		img->clearBlocks(header);
		DataBlock** matrix = img->getMatrix();
		int amount_channels = img->getType();
		int count;
		int neighbors=8;
		int w = img->getWidth()/img->getWidthBlock();
		int h = img->getHeight()/img->getHeightBlock();
		do{
			for(int i = 0; i < h; i++){
	            for(int j = 0; j < w; j++){
	                if((!matrix[i][j].isValid()) && amountNeighbors(img,i,j)==neighbors) // si el bloque no es válido
	                {
	                    vector<int> add(amount_channels);
		                for(int x=0;x<amount_channels;x++)
		                	add[x]=0;
		                count = 0;
	                    /* cálculo del promedio de los canales de los vecinos */
	                    for(int iBlock = max((int)i-1,0); iBlock <= min(i+1,h-1) ; iBlock++)   /**/
	                        for(int jBlock = max((int)j-1,0); jBlock <= min(j+1,w-1); jBlock++) /** 8 conectados */
	                            if((i != iBlock) || (j != jBlock))
	                                if(matrix[iBlock][jBlock].isValid()) // si el bloque vecino existe
	                                {
			                            vector<Pixel> *e = (vector<Pixel>*)matrix[iBlock][jBlock].getExtras();
	                                    for(int r=0;r<img->getHeightBlock();r++){
	                                    	for(int c=0;c<img->getWidthBlock();c++){
				                                vector<int> chnls = e->at((r*img->getWidthBlock())+c).getChannels();
				                                for(int x=0;x<amount_channels;x++){
				                					add[x]+=chnls[x];
				                				}
	                                    		count++;
	                                    	}
	                                    }
	                                }          	
	                    if(count!=0)
	                    {
	                        for(int x=0;x<amount_channels;x++){
	                			add[x]=(int)(add[x]/count);
	                			//cout << add[x] << "---";
	                		}
	                		//cout << i << "-" << j << "\n";
	                		DataBlock* px = new DataBlock(amount_channels,add,img->getWidthBlock(),img->getHeightBlock());
	                		matrix[i][j]= *px;
	                    }
	                }
	            }
	        }
	        neighbors=maxNeighbors(img,h,w);
	    }while(neighbors!=0 && isLossBlock(img));
	    if(export_images){
	    	string path = header->folder+"/image_received_restored.bmp";
            img->save(path.c_str());
	    }
	    if(show_data)
	    	cout << "The Image has been restored\n";
	}

private:

	int maxNeighbors(Images* img, int h, int w){
		int max = 0;
		DataBlock** matrix = img->getMatrix();
		for (int i = 0; i < h; i+=1){
			for(int j = 0; j < w; j+=1){
				if(max<amountNeighbors(img,i,j) && !matrix[i][j].isValid())
					max = amountNeighbors(img,i,j);
			}
		}
		return max;
	}

	int amountNeighbors(Images* img, int i, int j){
	/** 
	* @brief Método que calcula la cantidad de vecinos válidos que tiene un pixel de la imagen img en la posición (i,j)
	* @param img Imagen que contiene el pixel en la posición (i,j)
	* @param i fila en la que está ubicada el pixel
	* @param j columna en la que está ubicada el pixel
	* @return neighbors
	*/
		int w = img->getWidth()/img->getWidthBlock();
		int h = img->getHeight()/img->getHeightBlock();
		DataBlock** matrix=img->getMatrix();
		int neighbors=0;
		for(int iBlock = max((int)i-1,0); iBlock <= min(i+1,h-1) ; iBlock++)   /**/
	        for(int jBlock = max((int)j-1,0); jBlock <= min(j+1,w-1); jBlock++) /** 8 conectados */
	            if((i != iBlock) || (j != jBlock))
	                if(matrix[iBlock][jBlock].isValid()) // si el bloque vecino existe
	                {
	                    neighbors++;
	                }
	    //cout << "(" << i << "," << j << "):" << neighbors << "--\n";
	    return neighbors;
	}

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