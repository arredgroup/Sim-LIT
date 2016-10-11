/**
  @file RebuilderBicubico.h
  @class Rebuilder
  @brief Clase que implementa un método de ocultamiento de errores utilizando la interpolacion bicubica
  @author Mauricio Baeza y Christopher Carrera
  @date 3/8/2016

**/


#ifndef CLASS_REBUILDERBICUBICO
#define CLASS_REBUILDERBICUBICO

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#include "Rebuilder.h"
#include <iostream>
#include <vector>

using namespace std;

class RebuilderBicubico: public Rebuilder{

public:

	RebuilderBicubico(){};

	~RebuilderBicubico(){};

	virtual void hidden(Images* img, bool show_data, bool export_images, HEAD *header){
	/** 
	* @brief Método que oculta los pixeles dañados y los reemplaza por el promedio de sus vecinos más cercanos
	* @param img Imagen sobre la que se le aplicará el método de ocultamiento de errores
	*
	*/
//        img->clearBlocks(header);
        DataBlock** matrix = img->getMatrix();
        int amount_channels = img->getType();
        int w = img->getWidth()/img->getWidthBlock();
        int h = img->getHeight()/img->getHeightBlock();
        float reparador,f_i_j,df_dc,df_dr,d2f_dcdr;
        
        do{
            for(int i = 0; i < h; i++){
                for(int j = 0; j < w; j=j+1){
                    if(!matrix[i][j].isValid()){
                        vector<int> add(amount_channels);//vector donde se guardaran los valores de las bandas(o canales) de un pixel 
                        reparador=0;//valor de cada banda o canal
                        vector<Pixel> punto;//vector de pixeles para construir el bloque a reconstruir 
                        
                        for(int r=0;r<img->getHeightBlock();r++){
                            for(int c=0;c<img->getWidthBlock();c++){
                                for(int x=0;x<amount_channels;x++){                        
                                    if((i>0) & (i<h-1) & (j>0) & (j<w-1)){//bloques interiores
                                        //cout << "reparando bloques interiores\n";
                                        if ((r>0)&&(r<img->getWidthBlock()-1)&&(c>0)&&(c<img->getWidthBlock()-1)) {
//cout << "entre al centro del bloque del bloque\n";



f_i_j=(nn(img,i,j,c-1,x)+nn(img,i,j,c+1,x)+(ss(img,i,j,c-1,x))+(ss(img,i,j,c+1,x))+(ee(img,i,j,r-1,x))+(ee(img,i,j,r+1,x))+(oo(img,i,j,r-1,x))+(oo(img,i,j,r+1,x)));
                                        
df_dc= (((nn(img,i,j,c-1,x)-nn(img,i,j,c+1,x))/2)+((ss(img,i,j,c-1,x)-ss(img,i,j,c+1,x))/2));
                                        
df_dr= (((nn(img,i,j,c-1,x)-ss(img,i,j,c-1,x))/img->getWidthBlock()+2)+((nn(img,i,j,c+1,x)-ss(img,i,j,c+1,x))/img->getWidthBlock()+2));
                                        
d2f_dcdr= (ss(img,i,j,c+1,x)-nn(img,i,j,c+1,x)-ss(img,i,j,c-1,x)+nn(img,i,j,c-1,x));
                // reparador=(nn(img,i,j,c,x)*((img->getWidthBlock()+2)-c)  +(ss(img,i,j,c,x)*(c-1))+(ee(img,i,j,r,x)*(r-1))+(oo(img,i,j,r,x)*((img->getWidthBlock()+2)-r)))/((img->getWidthBlock()*2)+2);                           
reparador=  ((f_i_j)/8)+ ((df_dc)/16)+ ((df_dr)/16)+ ((d2f_dcdr)/64);
                                        }
                                        if ((r==0) && (c==0)) {
					//cout << "entre a la esquina del bloque\n";
                                            reparador= (nn(img,i,j,c,x)+oo(img,i,j,r,x))/2;
					
                                        }
                                        if ((r==0)&&(c==img->getWidthBlock()-1)) {
                                            reparador= ((nn(img,i,j,c,x)+ee(img,i,j,r,x))/2);
					
                                        }
                                        if ((r==img->getWidthBlock()-1)&&(c==0)) {
                                            reparador= ((ss(img,i,j,c,x)+oo(img,i,j,r,x))/2);
					
                                        }
                                        if ((r==img->getWidthBlock()-1)&&(c==img->getWidthBlock()-1)) {
                                            reparador= ((ss(img,i,j,c,x)+ee(img,i,j,r,x))/2);
					
                                        }
                                         if ((r==0)&&(c<img->getWidthBlock()-1)&&(c>0)) {
					
                                            reparador= (nn(img,i,j,c-1,x)+nn(img,i,j,c,x)+nn(img,i,j,c+1,x))/3;               
                                        
                                        }                                        
                                         
                                        if ((r==img->getWidthBlock()-1)&&(c<img->getWidthBlock()-1)&&(c>0)) {
					
                                            reparador= (ss(img,i,j,c-1,x)+ss(img,i,j,c,x)+ss(img,i,j,c+1,x))/3;               
                                        
                                        }
					if ((c==0)&&(r<img->getWidthBlock()-1)&&(r>0)) {
					
                                            reparador= (oo(img,i,j,r-1,x)+oo(img,i,j,r,x)+oo(img,i,j,r+1,x))/3;               
                                        
                                        }
					if ((c==img->getWidthBlock()-1)&&(r<img->getWidthBlock()-1)&&(r>0)) {
					
                                            reparador= (ee(img,i,j,r-1,x)+ee(img,i,j,r,x)+ee(img,i,j,r+1,x))/3;               
                                        
                                        }

                                                                                
                                        //reparador=(nn(img,i,j,c,x)+(ss(img,i,j,c,x))+(ee(img,i,j,r,x))+(oo(img,i,j,r,x)))/4;//i,j coordenadas del bloque, c=columna pixel dentro del bloque
                                                                                                                            //x= canal de la imagen; r= filas dentro del bloque.
                                                                                                                            //getWidthBlock= tamaño del bloque
                                    }
                                    else{
                                        if((i>0&&i<h-1) && (j==0 | j==w-1)){//primera y ultima columna sin conciderar las esquinas
                                            if(j==0){
                                            //cout << " reparando primera columna\n";
                                            reparador=(ss(img,i,j,c,x)+nn(img,i,j,c,x)+ee(img,i,j,r,x))/3;    
                                            }
                                            else{
                                            //cout << " reparando ultima columna\n";
                                            reparador=(ss(img,i,j,c,x)+nn(img,i,j,c,x)+oo(img,i,j,r,x))/3;                                                
                                            }                                            
                                        }
                                        else{
                                            if((i==0 | i==h-1) && (j>0&&j<w-1)){//primera y ultima fila sin conciderar las esquinas
                                                if(i==0){
                                                //cout << "reparando primera fila\n";
                                                reparador=(oo(img,i,j,r,x)+ee(img,i,j,r,x)+ss(img,i,j,c,x))/3;                                                    
                                                }
                                                else{
                                                //cout << "reparando ultima fila\n";
                                                reparador=(oo(img,i,j,r,x)+ee(img,i,j,r,x)+nn(img,i,j,c,x))/3;                                                    
                                                }                                                
                                            }
                                            else{
                                                if(i==0 & j==0){//esquina superios izquierda
                                                    //cout << "reparando esquina superios izquierda\n";
                                                    reparador=((ss(img,i,j,c,x))+(ee(img,i,j,r,x)))/2;
                                                }
                                                else{
                                                    if(i==0&j==w-1){//esquina superior derecha
                                                    //cout << "reparando esquina superior derecha\n";
                                                        reparador=((ss(img,i,j,c,x))+(oo(img,i,j,r,x)))/2;
                                                    }
                                                    else{
                                                        if(i==h-1 & j==0){//esquina inferiro izquierda
                                                    //cout << "reparando esquina inferiro izquierda\n";
                                                            reparador=((nn(img,i,j,c,x))+(ee(img,i,j,r,x)))/2;
                                                        }
                                                        else{//esquina inferior derecha 
                                                    //cout << "reparando esquina inferior derecha\n";
                                                         reparador=((nn(img,i,j,c,x))+(oo(img,i,j,r,x)))/2;   
                                                        }                                                    
                                                    }

                                                }

                                            }

                                        }

                                    }
                                    add[x]=floor(reparador);//actualizando el vector con el valor de la banda o canal "x"
                        }
                                
                            Pixel aux(add,amount_channels); //creando el pixel en su respectivo formato    
                            punto.push_back(aux);//creando el vector de pixel que con el cual se construira el bloque recuperado                             
                            }
                        }//aqui se pasa el vector de pixel al constructor DataBlock en que debuelve un bloque reparado
                        DataBlock* db = new DataBlock(punto,img->getWidthBlock(),img->getHeightBlock(),amount_channels);
	                
//            vector<Pixel> *e = (vector<Pixel>*)db->getExtras();
//            for(int r=0;r<img->getHeightBlock();r++){
//	                                    	for(int c=0;c<img->getWidthBlock();c++){
//				                                vector<int> chnls = e->at((r*img->getWidthBlock())+c).getChannels();
//				                                for(int x=0;x<amount_channels;x++){
//				                					cout<<"pixel desde bloque creado"<<chnls[x]<<"\n";
//				                				}
//                                                }
//            }
                        
                        matrix[i][j]= *db;//actualizando la matrix de bloques con el nuevo bloque reconstruido
                    }
	                }
	            }        
	    }while(isLossBlock(img));
	    if(export_images){
	    	string path = header->folder+"/image_restored_bicubica.bmp";
            img->save(path.c_str());
	    }
	    if(show_data)
	    	cout << "The Image has been restored by bicubic\n";
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
        int nn(Images* img, int i, int j,int jj, int x){
	/** 
	* @brief Método que devuelve el pixel inferior  del bloque superior contiguo al pixel perdido
	* @param img Imagen que contiene el pixel en la posición (i,j)
	* @param i fila en la que está ubicada el bloque afectado
	* @param j columna en la que está ubicada el el bloque afectado
        * @param jj ubicacion del pixel en el bloque con perdida
	* @param x banda de interes
	* @return pixel
	*/
            //cout << "bloque superior \n";
            DataBlock** matrix=img->getMatrix();
            int pixel;
            if(!matrix[i-1][j].isValid()){
                pixel=127;
            }
            else{
            vector<Pixel> *e = (vector<Pixel>*)matrix[i-1][j].getExtras();
            vector<int> chnls = e->at(((img->getHeightBlock()-1)*img->getWidthBlock())+jj).getChannels();                    
            pixel=chnls[x];
            }
            //cout << "pixel superior: " << pixel << "\n";
            return pixel;
        }
        int oo(Images* img, int i, int j,int ii, int x){
	/** 
	* @brief Método que devuelve el pixel derecho  del bloque izquierdo contiguo al pixel perdido
	* @param img Imagen que contiene el pixel en la posición (i,j)
	* @param i fila en la que está ubicada el bloque afectado
	* @param j columna en la que está ubicada el el bloque afectado
        * @param ii ubicacion del pixel en el bloque con perdida
	* @param x banda de interes
	* @return pixel
	*/
            //cout << "bloque izquierdo \n";
            DataBlock** matrix=img->getMatrix();
            int pixel;
            if(!matrix[i][j-1].isValid()){
                pixel=127;
            }
            else{
            vector<Pixel> *e = (vector<Pixel>*)matrix[i][j-1].getExtras();
            vector<int> chnls = e->at(((ii)*img->getWidthBlock())+(img->getWidthBlock()-1)).getChannels();                    
            pixel=chnls[x];
            }
            //cout << "pixel izquierdo: " << pixel << "\n";
            return pixel;
        }
        int ee(Images* img, int i, int j,int ii, int x){
	/** 
	* @brief Método que devuelve el pixel derecho  del bloque izquierdo contiguo al pixel perdido
	* @param img Imagen que contiene el pixel en la posición (i,j)
	* @param i fila en la que está ubicada el bloque afectado
	* @param j columna en la que está ubicada el el bloque afectado
        * @param ii ubicacion del pixel en el bloque con perdida
	* @param x banda de interes
	* @return pixel
	*/
            //cout << "bloque derecho \n";
            DataBlock** matrix=img->getMatrix();
            int pixel;
            if(!matrix[i][j+1].isValid()){
               pixel=127; 
            }
            else{
            vector<Pixel> *e = (vector<Pixel>*)matrix[i][j+1].getExtras();
            vector<int> chnls = e->at(((ii)*img->getWidthBlock())).getChannels();
            pixel=chnls[x];
            }
            //cout << "pixel derecho: " << pixel << "\n";
            return pixel;
        }
        int ss(Images* img, int i, int j,int jj, int x){
	/** 
	* @brief Método que devuelve el pixel superior  del bloque inferior contiguo al pixel perdido
	* @param img Imagen que contiene el pixel en la posición (i,j)
	* @param i fila en la que está ubicada el bloque afectado
	* @param j columna en la que está ubicada el el bloque afectado
        * @param jj ubicacion del pixel en el bloque con perdida
	* @param x banda de interes
	* @return pixel
	*/
            //cout << "bloque inferior\n";
            DataBlock** matrix=img->getMatrix();
            int pixel;
            if(!matrix[i+1][j].isValid()){//si no existe el bloque se devuelve 0
                pixel=127;
            }
            else{
            vector<Pixel> *e = (vector<Pixel>*)matrix[i+1][j].getExtras();//los elementos del bloque se recuperan en un vector
            vector<int> chnls = e->at(jj).getChannels();//se busca el elemento de interes
            pixel=chnls[x];//el correspondiente canal del elemento de interes 
            }
            //cout << "pixel inferior: " << pixel << "\n";
            return pixel;
        }   

};
#endif
