/**
  @file Packetizer.h
  @class Packetizer
  @brief Clase que define la estructuras que debe tener el paquetizador
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef	CLASS_PACKETIZER
#define CLASS_PACKETIZER

#include "../Package.h"
#include "../DataType.h"
#include "../../structs.h"
#include <vector>

class Packetizer
{
public:
	Packetizer(){}
	~Packetizer(){}

	virtual vector<Package> imgToPackageList(vector<DataType *> list, int size, bool show_data, void* extras = NULL)=0;
	virtual vector<DataType *> packageListToImg(vector<Package> list, bool show_data, bool export_images, HEAD *header)=0;
	virtual int getSizeElements()=0;
};

#endif