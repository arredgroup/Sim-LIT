/**
 @file RebuilderCDD.h
 @class RebuilderCDD
 @brief Clase que implementa un método de ocultamiento de errores utilizando ecuaciones de difusion de curva
 @author Mauricio Baeza y Christopher Carrera
 @date 13/7/2016
 **/


#ifndef CLASS_REBUILDERCDD
#define CLASS_REBUILDERCDD

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "Rebuilder.h"
#include <iostream>
#include <vector>

using namespace std;

class RebuilderCDD: public Rebuilder{
    
public:
    
    RebuilderCDD(){};
    
    ~RebuilderCDD(){};
    
    virtual void hidden(Images* img, bool show_data, bool export_images, HEAD *header){
        /**
         * @brief Método que oculta los pixeles dañados
         * @param img Imagen sobre la que se le aplicará el método de ocultamiento de errores
         *
         */
        img->clearBlocks(header);
        DataBlock** matrix = img->getMatrix();
        int amount_channels = img->getType();
        int w = img->getWidth()/img->getWidthBlock();
        int h = img->getHeight()/img->getHeightBlock();
        // matrices auxiliares para CDD
        float V[img->getWidth()][img->getHeight()][amount_channels];
        float U[img->getWidth()][img->getHeight()][amount_channels];
        float R[img->getWidth()][img->getHeight()][amount_channels]; 
        
        float gridUw2,gridUe2,gridUs2,gridUn2,w1,w2,w3,w4;
        float gridw2, gride2, grids2, gridn2;
        float a1,a2,a3,a4,k;
        int a=5;
        int n=1;
        //    // la cantidad de iteraciones para realizar el inpainting
        int iteraciones=400;
        for(int i = 0; i < h; i+=1){
        	for(int j = 0; j < w; j+=1){
                if(!matrix[i][j].isValid()){// si el bloque no es válido
                    for(int r=0;r<img->getHeightBlock();r+=1){
                        for(int c=0;c<img->getWidthBlock();c+=1){
                            for(int x=0;x<amount_channels;x+=1){
                               V[i][j][x]=255; 
                               U[i][j][x]=255;
                               R[i][j][x]=0;
                            }
                        }
                    } 
                }//comentar para quitar validacion de perdida
                else{
                    vector<Pixel> *e = (vector<Pixel>*)matrix[i][j].getExtras();//contiene todo los pixeles de un bloque expecificio
                    for(int r=0;r<img->getHeightBlock();r+=1){
                        for(int c=0;c<img->getWidthBlock();c+=1){
                            vector<int> chnls = e->at((r*img->getWidthBlock())+c).getChannels();//varible con los pixeles de cada canal
                            for(int x=0;x<amount_channels;x+=1){
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
            for(int i = 0; i < h; i+=1){
        	   for(int j = 0; j < w; j+=1){
                    for(int x=0;x<amount_channels;x+=1){
                        if (R[i][j][x]==0){
        //                if(i>0&&i<h-1&&j>0&&j<w-1){
                            gridw2=pow(V[i][j][x]-V[i-1][j][x], 2)+pow((V[i-1][j-1][x]-V[i-1][j+1][x])/2, 2);                    
                            gride2=pow(V[i][j][x]-V[i+1][j][x], 2)+pow((V[i+1][j-1][x]-V[i+1][j+1][x])/2, 2);
                            grids2=pow(V[i][j][x]-V[i][j-1][x], 2)+pow((V[i-1][j-1][x]-V[i+1][j-1][x])/2, 2);
                            gridn2=pow(V[i][j][x]-V[i][j+1][x], 2)+pow((V[i-1][j+1][x]-V[i+1][j+1][x])/2, 2);
                            a1 = 1/sqrt(a+gridw2);
                            a2 = 1/sqrt(a+gride2);
                            a3 = 1/sqrt(a+grids2);
                            a4 = 1/sqrt(a+gridn2);
                            k =(a1*V[i-1][j][x]+a2*V[i+1][j][x]+a3*V[i][j+1][x]+a4*V[i][j-1][x])/(a1+a2+a3+a4);
                            
                            gridUw2=pow(V[i][j][x]-V[i-1][j][x], 2)+pow((V[i-1][j-1][x]-V[i-1][j+1][x])/2, 2);
                            gridUe2=pow(V[i][j][x]-V[i+1][j][x], 2)+pow((V[i+1][j-1][x]-V[i+1][j+1][x])/2, 2);
                            gridUs2=pow(V[i][j][x]-V[i][j-1][x], 2)+pow((V[i-1][j-1][x]-V[i+1][j-1][x])/2, 2);
                            gridUn2=pow(V[i][j][x]-V[i][j+1][x], 2)+pow((V[i-1][j+1][x]-V[i+1][j+1][x])/2, 2);
                            w1 = k/sqrt(gridUw2+a);
                            w2 = k/sqrt(gridUe2+a);
                            w3 = k/sqrt(gridUs2+a);
                            w4 = k/sqrt(gridUn2+a);
                                //Aqui se van guardando los pixeles regenerados a partir de CDD inpainting
                            U[i][j][x]=(w1*V[i-1][j][x]+w2*V[i+1][j][x]+w3*V[i][j+1][x]+w4*V[i][j-1][x])/(w1+w2+w3+w4);                             
        //                }
        //                else{
        //                    cout<<"pixel de borde\n";
        //                }
                        }
                    }
                }
            }     
            for(int i = 0; i < h; i+=1){
        	   for(int j = 0; j < w; j+=1){
                    for(int x=0;x<amount_channels;x+=1){
                        V[i][j][x]=U[i][j][x];                
                    }           
                }
            }                   
            n+=1; 
        }
        for(int i = 0; i < h; i++){
    	   for(int j = 0; j < w; j++){
                vector<int> add; //
                for(int x=0;x<amount_channels;x+=1){
                    add.push_back(floor(V[i][j][x]));//
                }
                DataBlock* px = new DataBlock(amount_channels,add,img->getWidthBlock(),img->getHeightBlock());
                matrix[i][j]= *px;
            }
        }
        if(export_images){
            string path = header->folder+"/image_restored_by_CDD.bmp";
            img->save(path.c_str());
        }
        if(show_data)
            cout << "The Image has been restored by CDD\n";
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

