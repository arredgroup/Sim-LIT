/**
  @file MetricPsnr.h
  @class MetricPsnr
  @brief Clase que representa la Métrica PSNR
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_METRICPSNR
#define CLASS_METRICPSNR 

#include "Metric.h"

class MetricPsnr:public Metric{

public: 

	MetricPsnr(){
	/*
	*  @brief Método Constructor de la clase
	*/		
		this->name="PSNR";
	}

	~MetricPsnr(){}

	double calculate(Images org, Images rcv, bool show_data, HEAD *header){
	/*
	*  @brief Método que Calcula el PSNR de dos Imagenes
	*  @param org Imagen Original
	*  @param rcv Imagen Reconstruida
	   @return psnr
	*/
		double mse, psnr, del;
		//if(rcv.getWidthBlock()!=org.getWidthBlock() || rcv.getHeightBlock()!=org.getHeightBlock()){
			rcv.clearBlocks(header);
			org.clearBlocks(header);
		//}
	    mse = 0;
	    int h = org.getHeight();
	    int w = org.getWidth();
	    DataBlock** org_m = org.getMatrix();
	    DataBlock** rcv_m = rcv.getMatrix();
	    for (int i = 0; i < h; i=i+1)
	    {
	    	for (int j = 0; j < w; j=j+1)
	    	{
	    		DataBlock org_e = org_m[i][j];
	    		DataBlock rcv_e = rcv_m[i][j];
	    		del = abs(org_e.getIntensity() - rcv_e.getIntensity());
	        	mse = mse + (double)pow(del, 2);
	        }
	    }
	    mse = mse/(double)(h*w);
	    psnr = (double)(10.0 * log10(pow((double)maxIntensity(org), 2) / mse));

	    if(show_data)
	    	cout << "PSNR: " << psnr << "\n";
	    return psnr;
	}

	int maxIntensity(Images img){
	/* 
	*  @brief Calcula el valor máximo de intensidad de una Imagen
 	*  @param img Imagen sobre la que se calculará la intensidad máxima
	*/
	    int max;
	    DataBlock** matrix = img.getMatrix();
	    if(matrix[0][0].isValid())
	    	max = matrix[0][0].getIntensity();
	    else
	    	max = 0;
	    int h = img.getHeight()/img.getHeightBlock();
	    int w = img.getWidth()/img.getWidthBlock();
	    for (int i = 0; i < h; i=i+1)
	    {
	    	for (int j = 0; j < w; j=j+1)
	    	{
	    		if(matrix[i][j].getIntensity() > max)
	    			max=matrix[i][j].getIntensity();
	    	}
	    }
	    return max;
	}

	string getName(){
		return this->name;
	}

};

#endif