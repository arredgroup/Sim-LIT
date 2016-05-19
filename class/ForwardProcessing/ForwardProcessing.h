/**
  @file ForwardProcessing.h
  @class ForwardProcessing
  @brief Clase abstracta que representa la estructura que debe tener todo procesamiento de imagen
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/

#ifndef CLASS_FORWARDPROCESSING
#define CLASS_FORWARDPROCESSING

#include <vector>
#include "../DataType.h"
#include "../Images.h"
#include "../../structs.h"

using namespace std;

class ForwardProcessing{

	//static int order;

public:

	virtual int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_image) = 0;
	virtual int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_image) = 0;
/*	static void set(int a){
		order=order+a;
	}
	static int get(){
		return order;
	}
	static void define(){
		order=0;
	} 
*/
};


#endif