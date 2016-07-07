/**
  @file TibsCompressor.h
  @class TibsCompressor
  @brief Clase que implementa un tipo de compresión de Tibs 
  @author Christopher Arredondo Flores
  @date 01/01/2016

**/

#ifndef CLASS_TIBSCOMPRESSOR
#define CLASS_TIBSCOMPRESSOR

#include "../Images.h"
#include <cmath>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

class TibsCompressor: public ForwardProcessing{

public:

	TibsCompressor(){}

	~TibsCompressor(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_image){
	
		return 0;
	}

	int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_image){
		if(proceed){
			return 0;
		}
		return 1;
	}

};

#endif
