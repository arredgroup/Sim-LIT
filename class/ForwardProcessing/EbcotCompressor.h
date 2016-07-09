/**
  @file EbcotCompressor.h
  @class EbcotCompressor
  @brief Clase que implementa un tipo de compresión de EBCOT 
  @author Christopher Arredondo Flores
  @date 01/01/2016

**/

#ifndef CLASS_EBCOTCOMPRESSOR
#define CLASS_EBCOTCOMPRESSOR

#include "../Images.h"
#include <cmath>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

class EbcotCompressor: public ForwardProcessing{

	bool proceed;

	vector< vector< vector <double> > > image;
	int image_width;
	int image_height;

public:

	EbcotCompressor(){}

	~EbcotCompressor(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_image){
		image_width = (header->w*header->wb);
		image_height = (header->h*header->hb);
		readImage(list,header);
		proceed = true;
		return 0;
	}

	int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_image){
		if(proceed){
			return 0;
		}
		return 1;
	}

private:

	void readImage(vector<DataType *> *list, HEAD* header){
		image.resize(image_height);
		for (int i = 0; (unsigned)i < image.size(); i+=1){
			image[i].resize(image_width);
		}
		int w_block = 0;
		int h_block = 0;
		int chnl_block = 0;
		int pos = 0;
		for (int i = 0; i < header->h; i+=1){
			for (int j = 0; j < header->w; j+=1){
				pos = (i*(header->h))+j;
				int* head_block = list->at(pos)->getContent();
				h_block = head_block[1];
				w_block = head_block[2];
				chnl_block = head_block[3];
				//Here, we read each block inside in the list
				vector<Pixel> *block = (vector<Pixel>*)list->at(pos)->getExtras();
				for(int f = 0; f < h_block; f+=1){
					for(int c = 0;c < w_block; c+=1){
						Pixel px = block->at((f*h_block)+c);
						vector<int> aux = px.getChannels();
						for (int x = 0; x < chnl_block && (unsigned)x < aux.size(); x+=1){
							image[(i*h_block)+f][(j*w_block)+c].push_back(aux[x]);
						}
					}
				}
			}
		}
	}

};

#endif