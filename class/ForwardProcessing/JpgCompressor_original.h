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
#include <vector>
#include <string>
#include "../Libs/jpg/stb_image.c"
#include "../Libs/jpg/jpge.cpp"
#include "../Libs/jpg/jpgd.cpp"
#include "ForwardProcessing.h"
#include "../Pixel.h"
#include "../DataByte.h"
#include <ctype.h>

//using namespace std;


class JpgCompressor: public ForwardProcessing{

    bool proceed; //!<@brief Variable que indica si el método se realizó correctamente o no
    int subsampling;
    int actual_comps;
    uint8 image_data;
    uint8* header_compressed;
    int buf_size;
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
        if(nParameters!=2){
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
        int buf_size = header->w * header->h * 3; // allocate a buffer that's hopefully big enough (this is way overkill for jpeg)
        if (buf_size < 1024) {
            buf_size = 1024;
        }
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

        header_compressed = dst_stream.get_buffer();

        int flag=0;
        vector<DataType *> new_list;
        uint8* aux = header_compressed;
        for (int i = 0; i < dst_stream.get_size(); i+=1)
        { /*
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOI && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOF0 && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_DHT && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_DQT && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }*/
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOS && static_cast<unsigned>(*(aux+i))==0xFF){   
            i+=2;         
            flag=0;
          }/*
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_APP0 && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }*/
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_EOI && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=2;
            flag=1;
          }
          if(flag==0 && i < dst_stream.get_size()){
          DataByte *nuevo = new DataByte(static_cast<unsigned>(*(aux+i)));
            new_list.push_back(nuevo);
          }
        }
        if(show_data)
            cout << "JPEG Compression Applied to Image\n";
        proceed = true;
        *list = new_list;
        return 0;
    }



    int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_images){
    /**
     * @brief Método que deshace la compresión utilizando el Algoritmo de JPEG
     */
        int c=0;
        int flag = 0;
        uint8* aux = header_compressed;
        for (int i = 0; i < buf_size && c < list->size(); i+=1)
        { /*
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOI && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOF0 && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_DHT && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_DQT && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }*/
          if(i+1 < buf_size && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOS && static_cast<unsigned>(*(aux+i))==0xFF){   
            i+=2;         
            flag=0;
          }/*
          if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_APP0 && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=1;
            flag=1;
          }*/
          if(i+1 < buf_size && static_cast<unsigned>(*(aux+i+1))==jpge::M_EOI && static_cast<unsigned>(*(aux+i))==0xFF){
            i+=2;
            flag=1;
          }
          if(flag==0 && i < buf_size){
            if(list->at(c)->isValid())
              *(aux+i) = *(uint8 *) list->at(c)->getExtras();
            else
              *(aux+i) = (unsigned int)0;
            c++;
          }
        }
        if(proceed){
          int uncomp_width = 0, uncomp_height = 0, uncomp_actual_comps = 0, uncomp_req_comps = 3;
          uint8 *pUncomp_image_data;
          pUncomp_image_data = jpgd::decompress_jpeg_image_from_memory((const stbi_uc *)aux, buf_size, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);
            
            
        }
        proceed=false;
        return 0;
    }

  private:

    void generateuint8(vector<DataType *> *list, uint8 *image_data){
      // SE ASUME QUE EN ESTE VECTOR DE BLOQUES, TODOS TIENEN EL MISMO TAMAÑO
      if(list->size()<=0){
        cout << "(JPG) The List of Images DataType isn't defined \n";
        return;
      }
      //static_cast<char>(i)
      vector<Pixel> *aux = (vector<Pixel>*)list->at(0)->getExtras();
      if(aux->size()<=0){
        cout << "(JPG) The List of Pixel from Images DataType isn't defined \n";
        return;
      }
      actual_comps = aux->at(0).getAmountChannels();
      int size = aux->size() * actual_comps * list->size();
      image_data = new uint8[size];
      for (int i = 0; i < list->size(); i+=1)
      {//CADA BLOQUE...
        aux = (vector<Pixel>*)list->at(i)->getExtras();
        for (int j = 0; j < aux->size(); j+=1)
        {
            Pixel px = aux->at(j);
            for(int c=0;c<actual_comps;c+=1)
              image_data[((j*actual_comps)*aux->size())+c]=static_cast<unsigned char>(px.getChannels().at(c));
        }
      }
    }



};



#endif