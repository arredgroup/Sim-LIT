/**
  @file RebuilderTV.h
  @class RebuilderTV
  @brief Clase que implementa un método de ocultamiento de errores utilizando el promedio de sus vecinos más cercanos
  @author Mauricio Baeza y Christopher Carrera
  @date 13/7/2016
 * parese que se ha creado la matrices V,U

**/


#ifndef CLASS_REBUILDERTVBLOQUE
#define CLASS_REBUILDERTVBLOQUE

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "Rebuilder.h"
#include <iostream>
#include <vector>

using namespace std;

class RebuilderTVBloque: public Rebuilder{

public:

	RebuilderTVBloque(){};

	~RebuilderTVBloque(){};

	virtual void hidden(Images* img, bool show_data, bool export_images, HEAD *header){
	/** 
	* @brief Método que oculta los pixeles dañados y los reemplaza por el promedio de sus vecinos más cercanos
	* @param img Imagen sobre la que se le aplicará el método de ocultamiento de errores
	*
	*/            
            img->clearBlocks(header);
            DataBlock** matrix = img->getMatrix();
            int amount_channels = img->getType();
            int w = img->getWidth()/img->getWidthBlock();
            int h = img->getHeight()/img->getHeightBlock();
            // estas son las matrices auxiliares de TV inpainting
            float V[img->getWidth()][img->getHeight()][amount_channels];
            float U[img->getWidth()][img->getHeight()][amount_channels];
            float R[img->getWidth()][img->getHeight()][amount_channels];   

    float gridUw2,gridUe2,gridUs2,gridUn2,w1,w2,w3,w4;
    int n=1;
    int iteraciones=500;
    for(int i = 0; i < h; i++){
	for(int j = 0; j < w; j++){
       if((!matrix[i][j].isValid())){// si el bloque no es válido
                    for(int r=0;r<img->getHeightBlock();r++){
                        for(int c=0;c<img->getWidthBlock();c++){
                            for(int x=0;x<amount_channels;x++){
                               V[i][j][x]=0; 
                               U[i][j][x]=0;
                               R[i][j][x]=0;
                            }
                        }
                    } 
       }//comentar para quitar validacion de perdida
       else{
           vector<Pixel> *e = (vector<Pixel>*)matrix[i][j].getExtras();//contiene todo los pixeles de un bloque expecificio
                    for(int r=0;r<img->getHeightBlock();r++){
                        for(int c=0;c<img->getWidthBlock();c++){
                            vector<int> chnls = e->at((r*img->getWidthBlock())+c).getChannels();//varible con los pixeles de cada canal
                            for(int x=0;x<amount_channels;x++){
                               V[i][j][x]=chnls[x]; 
                               U[i][j][x]=chnls[x];
                               R[i][j][x]=255;
                            }
                        }
                    }
                }
       }
       }      
    
    //ahora se aplica el inpainting a la imagen binaria
    while (n<=iteraciones) {
    for(int i = 0; i < h; i++){
	for(int j = 0; j < w; j++){
            for(int x=0;x<amount_channels;x++){
                            if (R[i][j][x]==0){
                                // se generan los vectores de inpainting al rededor del pixel en cuestion
                                gridUw2=pow(V[i][j][x]-V[i-1][j][x], 2)+(1.0/16)*pow(V[i-1][j+1][x]+V[i][j+1][x]-V[i-1][j-1][x]-V[i][j-1][x], 2);
                                gridUe2=pow(V[i][j][x]-V[i+1][j][x], 2)+(1.0/16)*pow(V[i][j+1][x]+V[i+1][j+1][x]-V[i][j-1][x]-V[i+1][j-1][x], 2);
                                gridUs2=pow(V[i][j][x]-V[i][j-1][x], 2)+(1.0/16)*pow(V[i+1][j][x]+V[i+1][j-1][x]-V[i-1][j][x]-V[i-1][j-1][x], 2);
                                gridUn2=pow(V[i][j][x]-V[i][j+1][x], 2)+(1.0/16)*pow(V[i+1][j][x]+V[i+1][j+1][x]-V[i-1][j][x]-V[i-1][j+1][x], 2);
                                w1 = 1/sqrt(1+gridUw2);
                                w2 = 1/sqrt(1+gridUe2);
                                w3 = 1/sqrt(1+gridUs2);
                                w4 = 1/sqrt(1+gridUn2);
                                //Aqui se van guardando los pixeles regenerados a partir de TV inpainting
                                U[i][j][x] =(w1*V[i-1][j][x]+w2*V[i+1][j][x]+w3*V[i][j+1][x]+w4*V[i][j-1][x])/(w1+w2+w3+w4);
                                
                            }
                            }
                        }
                    }                     
    n++;
    for(int i = 0; i < h; i++){
	for(int j = 0; j < w; j++){
            for(int x=0;x<amount_channels;x++){
                V[i][j][x]=U[i][j][x];                
            }           
        }
    }    
        }
    for(int i = 0; i < h; i++){
	for(int j = 0; j < w; j++){
            vector<int> add(amount_channels); //
            for(int x=0;x<amount_channels;x++){
                add[x]=floor(V[i][j][x]);//
            }
            DataBlock* px = new DataBlock(amount_channels,add,img->getWidthBlock(),img->getHeightBlock());
            matrix[i][j]= *px;
           }
       }
    if(export_images){
	    	string path = header->folder+"/image_restored_by_tvBlock.bmp";
            img->save(path.c_str());
	    }
	    if(show_data)
	    	cout << "The Image has been restored by TVBlock\n";
	}

private:

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
