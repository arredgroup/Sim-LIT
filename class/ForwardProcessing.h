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
#include "DataType.h"

using namespace std;

class ForwardProcessing{

	//static int order;

public:

	virtual int make(vector<DataType *> *list, int width, int height, vector<int> parameters, int nParameters, bool show_data) = 0;
	virtual int unmake(vector<DataType *> *list, bool show_data) = 0;
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