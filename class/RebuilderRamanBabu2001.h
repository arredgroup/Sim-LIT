/**
  @file RebuilderRamanBabu.h
  @class RebuilderRamanBabu
  @brief This class implements the error concealment technique presented in A. Raman and M. Babu (2001) "A low complexity error concelament scheme for mpeg-4 coded videoo sequences".
  @author Cristian Duran-Faundez
  @date 19/5/2015

**/


#ifndef CLASS_REBUILDERRAMANBABU2001
#define CLASS_REBUILDERRAMANBABU2001

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "Rebuilder.h"
#include "Images.h"
#include "DataBlock.h"
#include <iostream>
#include <vector>

using namespace std;

class RebuilderRamanBabu2001: public Rebuilder{

public:

	RebuilderRamanBabu2001(){};

	~RebuilderRamanBabu2001(){};

	virtual void hidden(Images* img, bool show_data){
	/** 
	* @brief This method implements the error concealment technique in A. Raman and M. Babu (2001) "A low complexity error concelament scheme for mpeg-4 coded videoo sequences".
	* @param img The image with errors to be concealed.
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
	            for(int j = 0; j < w; j=j+1) //the matrix is visited block by block
	            {
	                if(!matrix[i][j].isValid()) // ask if the block is correctly received
	                {
				vector<int> add(amount_channels);  //create a vector add with the number of channels
		                for(int x=0;x<amount_channels;x++)
		                	add[x]=0; //set add to zero
		                count = 0; //count is the number of correctly received neighbors to a lost one.
	                    // cálculo del promedio de los canales de los vecinos */
	                    for(int iBlock = max((int)i-1,0); iBlock <= min(i+1,h-1) ; iBlock++)   /**/
	                        for(int jBlock = max((int)j-1,0); jBlock <= min(j+1,w-1); jBlock++) /** 8-connected blocks */
	                            if((i != iBlock) || (j != jBlock)) //don't consider the block to be concealed 
	                                if(matrix[iBlock][jBlock].isValid()) // is the neighboring block was correctly received
	                                {
			                    vector<Pixel> *e = (vector<Pixel>*)matrix[iBlock][jBlock].getExtras(); //obtain the block's pixels
	                                    for(int r=0;r<img->getHeightBlock();r++){
	                                    	for(int c=0;c<img->getWidthBlock();c++){
				                                vector<int> chnls = e->at(r*(img->getWidthBlock())+c).getChannels(); //to obtain a vector with all the pixels of the block
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
	                			add[x]=(int)(add[x]/count); //calculus of the value of the pixel in all channels.
	                		}
	                		DataBlock* db = new DataBlock(amount_channels,add,img->getWidthBlock(),img->getHeightBlock());
	                		matrix[i][j]= *db;
	                    }
	                }
	            }
	        }
	    }while(isLossBlock(img));
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
