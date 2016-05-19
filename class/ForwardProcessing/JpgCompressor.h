/**
  @file JpgCompressor.h
  @class JpgCompressor
  @brief Clase que implementa el Método de compresión de JPG
  @author Christopher Arredondo Flores
  @date 01/01/2016

**/

#ifndef CLASS_JPGCOMPRESSOR
#define CLASS_JPGCOMPRESSOR


#include <iostream>
#include <fstream> 
#include <sstream>
#include <stdlib.h>
#include <string>
#include "../Libs/jpg/stb_image.c"
#include "../Libs/jpg/jpge.cpp"
#include "../Libs/jpg/jpgd.cpp"
#include "ForwardProcessing.h"
#include "../Pixel.h"
#include <ctype.h>

//using namespace std;


class JpgCompressor: public ForwardProcessing{

    bool proceed; //!<@brief Variable que indica si el método se realizó correctamente o no
    int subsampling;
    int actual_comps;
    uint8 image_data;
    jpge::jpeg_encoder encoder;

public:

    JpgCompressor(){
    /*
    *  @brief Método Constructor de la clase JpgCompressor
    */
    }

    ~JpgCompressor(){}

    int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_images){
    /**
     * @brief Método aplica compresión utilizando el Algoritmo de Jpg
     * @param list Vector del tipo DataType que contiene la lista que se desea comprimir
     * @param width Cantidad de Columnas que tiene la representación de la Imagen
     * @param height Cantidad de Filas que tiene la representación de la Imagen
     * @param parameters parámetros que requiere el compresor para funcionar
     */
        subsampling = -1;
        actual_comps = 0;
        if(nParameters!=1){
            cout << "(JPEG): Parameters number are incorrect! STOP!\n";
            return 1;
        }
        float quality_factor = atoi((char*)parameters[0]);
        //int subsampling = parameters[1];
        if(quality_factor<1 || quality_factor>100){
            cout << "(JPEG): Compression Quality must be between 1 and 100\n";
            return 1;
        }

       // Imagen aux = new Imagen(list, actual_comps, width, height, wblock, hblock, false);

        if (!strcasecmp((char*)parameters[1], "h1v1"))
          subsampling = jpge::H1V1;
        if (!strcasecmp((char*)parameters[1], "h2v1"))
          subsampling = jpge::H2V1;
        if (!strcasecmp((char*)parameters[1], "h2v2"))
          subsampling = jpge::H2V2;
        if(subsampling==-1){
            cout << "(JPEG): Compression Subsampling are not defined\n";
            return 1;
        }
        //const int req_comps = 3; // request RGB image

        jpge::params params;
        params.m_quality = quality_factor;
        params.m_subsampling = (subsampling < 0) ? ((actual_comps == 1) ? jpge::Y_ONLY : jpge::H2V2) : static_cast<jpge::subsampling_t>(subsampling);
        int buf_size = header->w * header->h * 3;
        void *pBuf = malloc(buf_size);

        jpge::memory_stream dst_stream(pBuf, buf_size);

        generateuint8(list, &image_data);

        if (!encoder.init(&dst_stream, header->w, header->h, params)) {
            cout << "(JPEG): Encoder hasn't been initialized\n";
            return 1;
        }

        if (!encoder.read_image(&image_data, header->w, header->h, actual_comps)) {
            cout << "(JPEG): Encoder hasn't read image\n";
            return 1;
        }

        if (!encoder.compress_image()) {
            cout << "(JPEG): Encoder hasn't compress image\n";
            return 1;
        }

        //encoder.deinit();

        if(show_data)
            cout << "JPEG Compression Applied to Image\n";
        proceed = true;
        return 0;
    }

    int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_images){
    /**
     * @brief Método que deshace la compresión utilizando el Algoritmo de JPEG
     */
        if(proceed){

        }
        proceed=false;
        return 0;
    }

  private:

    void generateuint8(vector<DataType *> *list, uint8 *image_data){
      // SE ASUME QUE EN ESTE VECTOR DE BLOQUES, TODOS TIENEN EL MISMO TAMAÑO
      if(list->size()<=0)
        return;
      //static_cast<char>(i)
      vector<Pixel> *aux = (vector<Pixel>*)list->at(0)->getExtras();
      actual_comps = aux->at(0).getAmountChannels();
      uint8* v[aux->size() * actual_comps*(list->size())];
      image_data = *v;
      for (int i = 0; i < list->size(); ++i)
      {//CADA BLOQUE...
        aux = (vector<Pixel>*)list->at(i)->getExtras();
        for (int j = 0; j < aux->size(); ++j)
        {
            Pixel px = aux->at(j);
            for(int c=0;c<actual_comps;c++)
              image_data[((j*actual_comps)*aux->size())+c]=static_cast<unsigned char>(px.getChannels().at(c));
        }
      }
    }



};



#endif