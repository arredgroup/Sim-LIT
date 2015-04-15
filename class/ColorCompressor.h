/**
  @file ColorCompressor.h
  @class ColorCompressor
  @brief Clase que representa el Método de Compresión de Colores por Radio
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#ifndef CLASS_COLORCOMPRESSOR
#define CLASS_COLORCOMPRESSOR

#include "ForwardProcessing.h"
#include "Pixel.h"
#include "DataType.h"

class ColorCompressor: public ForwardProcessing{

	int radio; //!<@brief radio que se utilizará en la compresión (1 equivale a reducir en un bits el tamaño del Color)

public:

	ColorCompressor(){
	/*
	*  @brief Metodo Constructor de la clase ColorCompressor
	*/

	}

	~ColorCompressor(){}

	int make(vector<DataType *> *list, int width, int height, vector<int> parameters, int nParameters, bool show_data){
	/*
	*  @brief Método que implementa la compresión sobre un vector
	*  @param *list Vector con los Datos que se van a comprimir
	*  @param width Ancho de la Imagen original
	*  @param height Alto de la Imagen original
	*  @param parameters Parametros que requiere el Compresor para funcionar
	*  @param nParameters Cantidad de Parametros que posee parameters
	*  @return 0 si se realiza la compresión, 1 si no se realiza por falta de parámetros
	*/
		if(nParameters!=1)
			return 1; //Cantidad de parámetros incorrecta
		DataType *aux;
		radio=parameters[0];
		if(radio>7 || radio<1)
			return 5;//Valor de Argumentos no válidos
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				aux = list->at((i*height)+j);
				list->at((i*height)+j)=compress(aux);
			}
		}
		if(show_data)
			cout << "Color Compressor Applied to Image...\n";
		return 0;
	}

	int unmake(vector<DataType *> *list, bool show_data){
	/*
	*  @brief Método que deshace la compresión sobre un vector
	*  @param *list vector con la lista de elementos a ser descompimidos
	*  @return 0 si se Descomprimió correctamente
	*/
		DataType *aux;
		for (int i = 0; i < list->size(); i++)
		{
			if(list->at(i)!=NULL){
				aux = list->at(i);
				list->at(i)=uncompress(aux);
			}
		}
		if(show_data)
			cout << "Color Compressor undo to Image...\n";
		return 0;
	}

private:

	DataType * compress(DataType * element){
		/*
		*  @brief Método que comprime un elemento
		*  @param element Elemento que será comprimido
		*  @return new_element
		*/
		int* content = element->getContent();
		double sizeCompressed = (8-(float)radio)/8; 
		vector<Pixel> *array = (vector<Pixel>*)element->getExtras();
		for(int x=0;x<(content[1]*content[2]);x=x+1){
			if(array->at(x).isValid()){
				vector<int> data(array->at(x).getContent()[1]);
				for (int i = 0; i < array->at(x).getContent()[1]; i=i+1)
				{
					int value=array->at(x).getContent()[2+i];
					for(int j=1;j<=radio;j=j+1)
						value=value/2;
					data[i]=value;
				}
				//cout << "data: " << data[0] << "-" << data[1] << "-" << data[2] << "\n"; 					
				Pixel new_element(data,array->at(x).getContent()[1]);
				new_element.setSize(sizeCompressed);
				array->at(x)=new_element;
			}
		}
		return element;
	}

	DataType * uncompress(DataType * element){
		/*
		*  @brief Método que descomprime un elemento
		*  @param element Elemento que será descomprimido
		*  @return new_element
		*/
		int* content = element->getContent();
		//double sizeCompressed = (8-radio)/8;
		vector<Pixel> *array = (vector<Pixel>*)element->getExtras();
		for(int x=0;x<(content[1]*content[2]);x++){
			if(array->at(x).isValid()){
				vector<int> data(array->at(x).getContent()[1]);
				for (int i = 0; i < array->at(x).getContent()[1]; i++)
				{
					int value=array->at(x).getContent()[2+i];
					for(int j=1;j<=radio;j=j+1)
						value=value*2;
					data[i]=value;
				}
			}
		}
		return element;
	}


};

#endif
