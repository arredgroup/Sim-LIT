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
#include "../DataTibsStream.h"
#include "../../structs.h"
#include "TorusMixer.h"
#include <cmath>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

class TibsCompressor: public ForwardProcessing{

	bool proceed;

	vector< vector< vector <double> > > image;
	vector< vector< vector< vector <unsigned char> > > > blocks;
	vector< DataType *> new_list;
	int image_width;
	int image_height;
	int channels;
	HEAD hdr;
	TorusMixer tm;

	vector<bool> valids;

public:

	TibsCompressor(){}

	~TibsCompressor(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_image){
		if(nParameters!=2){
            cout << "(TiBS): Parameters number are incorrect! STOP!\n";
            return 1;
        }
		image_width = (header->w*header->wb);
		image_height = (header->h*header->hb);
		channels = header->chnls;

        hdr.w = ceil(image_width/2);
		hdr.h = ceil(image_height/2);
		hdr.hb = 1;
		hdr.wb = 1;
		hdr.chnls = channels;

		readImage(list,header);
		generateExtraPixels();
		encode();
		generateStream();
		if(tm.make(&new_list, &hdr, parameters, nParameters, false, false)!=0){
			cout << "(TiBS): Torus Can't be applied!! STOP!\n";
			return 1;
		}
		*list = new_list;
		proceed = true;
		return 0;
	}

	int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_image){
		if(proceed){
			if(tm.unmake(list,&hdr,false,false)!=0){
				cout << "(TiBS): Torus Can't be undo!! STOP!\n";
				return 1;
			}
			readList(list);
			decode();
			clearExtraPixels();
			generateImage();
			*list = new_list;
			return 0;
		}
		return 1;
	}

private:

	void readList(vector<DataType *> *list){
		blocks.clear(); 
		valids.resize(ceil(image_height/2)*ceil(image_width/2), false);
		blocks.resize(ceil(image_height/2));
		for (int i = 0; (unsigned)i < blocks.size(); i+=1){
			blocks[i].resize(ceil(image_width/2));
			for(int j = 0; (unsigned)j < blocks[i].size(); j+=1){
				DataTibsStream* aux = (DataTibsStream*)list->at(i*blocks.size()+j);
				if(aux!=NULL && aux->isValid()){
					blocks[i][j] = aux->getBlock();
					valids[(i*blocks.size())+j] = true;
				}
			}
		}
	}

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

	void generateExtraPixels(){
		if((image_width%2)!=0){
			for (int i = 0; (unsigned)i < image.size(); i+=1){
				int j=image[i].size();
				image[i].resize(j+1);
				vector <double> rgb;
				for(int x =0; (unsigned)x < image[i][j-1].size();x+=1){
					rgb.push_back(image[i][j-1][x]);
				}
				image[i][j+1]=rgb;
			}
		}
		if((image_height%2)!=0){
			int i=image.size();
			image.resize(i+1);
			vector< vector <double>> aux;
			for (int j = 0; (unsigned)j < image[i-1].size(); j+=1){
				vector <double> rgb;
				for(int x =0; (unsigned)x < image[i-1][j].size();x+=1){
					rgb.push_back(image[i-1][j][x]);
				}
				aux.push_back(rgb);
			}
			image[i+1]=aux;
		}
	}

	void encode(){
		int min_e, min_p, pos, e, p;
		unsigned char px_0, px_1, px_2,px_3, d_0, d_1, d_2;
		blocks.resize(round(image_height/2));
		for (int i = 0; (unsigned)i < blocks.size(); i+=1){
			blocks[i].resize(round(image_width/2));
			for(int j = 0; (unsigned)j < blocks[i].size(); j+=1){
				blocks[i][j].resize(channels);
			}
		}

		for (int f = 0, x = 0; f < image_height; f+=2,x+=1){
			for( int c = 0, y = 0;c < image_width; c+=2,y+=1){
				for(int i=0; i < channels;i+=1){
					px_0 = image[f][c][i];
					px_1 = image[f][c+1][i];
					px_2 = image[f+1][c][i];
					px_3 = image[f+1][c+1][i];

					d_0 = px_1; 
					d_1 = px_2; 
					d_2 = px_3;
					WatermarkSet(d_0, d_1, d_2, 0);
					min_e=abs(d_0-px_0)+abs(d_0-px_1)+abs(d_1-px_2)+abs(d_2-px_3);
					min_p=abs(d_1-d_0)+abs(d_2-d_0);
					
					d_0=px_0; 
					d_1=px_2; 
					d_2=px_3;
					WatermarkSet(d_0, d_1, d_2, 1);
					e=abs(d_0-px_0)+abs(d_2-px_1)+abs(d_1-px_2)+abs(d_2-px_3);
					p=abs(d_1-d_0)+abs(d_2-d_0);
					if ((e==min_e) && (p < min_p)){	
						pos=1;  
						min_p=p;	
					}
					if (e<min_e){	
						pos=1; 
						min_e=e;  
						min_p=p; 
					}

					d_0=px_0; 
					d_1=px_1; 
					d_2=px_3;
					WatermarkSet(d_0, d_1, d_2, 2);
					e=abs(d_0-px_0)+abs(d_1-px_1)+abs(d_0-px_2)+abs(d_2-px_3);
					p=abs(d_1-d_0)+abs(d_2-d_0);
					if ((e==min_e) && (p < min_p)){	
						pos=2;  
						min_p=p;	
					}
					if (e<min_e){	
						pos=2; 
						min_e=e;  
						min_p=p; 
					}
					
					d_0=px_0; 
					d_1=px_1; 
					d_2=px_2;
					WatermarkSet(d_0, d_1, d_2, 3);
					e=abs(d_0-px_0)+abs(d_1-px_1)+abs(d_2-px_2)+abs(d_2-px_3);
					p=abs(d_1-d_0)+abs(d_2-d_0);
					if ((e==min_e) && (p < min_p)){	
						pos=3;  
						min_p=p;	
					}
					if (e<min_e){	
						pos=3; 
						min_e=e;  
						min_p=p; 
					}

					vector<unsigned char> block(3);
					
					if (pos==0){
					      WatermarkSet(px_1, px_2, px_3, 0);		// on marque la perte de px_0
					      block[0]=px_1;
					      block[1]=px_2;
					      block[2]=px_3;
					   }
					if (pos==1){
					      WatermarkSet(px_0, px_2, px_3, 1);		// on marque la perte de px_1
					      block[0]=px_0;
					      block[1]=px_2;
					      block[2]=px_3;
					   }
					if (pos==2){
					      WatermarkSet(px_0, px_1, px_3, 2);		// on marque la perte de px_2
					      block[0]=px_0;
					      block[1]=px_1;
					      block[2]=px_3;
					   }
					if (pos==3){
					      WatermarkSet(px_0, px_1, px_2, 3);		// on marque la perte de px_3
					      block[0]=px_0;
					      block[1]=px_1;
					      block[2]=px_2;
					}
					blocks[x][y][i]=block;
					//cout << "[" << 	(double)blocks[x][y][i][0] << "," << (double)blocks[x][y][i][1] << "," << (double)blocks[x][y][i][2] << "] ";

				}
			}
		}
	}

	void decode(){

		image.clear();
		image.resize(image_height+(image_height%2));
		for (int i = 0; (unsigned)i < image.size(); i+=1){
			image[i].resize(image_width+(image_width%2),vector<double>(channels));
		}
		int pos;
		unsigned char d_0, d_1, d_2;

		for (int f = 0, x = 0; (unsigned)x < blocks.size(); f+=2,x+=1){
			for( int c = 0, y = 0; (unsigned)y < blocks[x].size(); c+=2,y+=1){
				if(valids[(x*blocks.size())+y] && blocks[x][y].size()==channels){
					for(int i=0; i < channels;i+=1){
						d_0 = blocks[x][y][i][0];
						d_1 = blocks[x][y][i][1];
						d_2 = blocks[x][y][i][2];

						//cout << "[" << 	(double)blocks[x][y][i][0] << "," << (double)blocks[x][y][i][1] << "," << (double)blocks[x][y][i][2] << "] ";
						pos = WatermarkGet(d_0, d_1, d_2);

					     // on décode sur le pixel de gauche
					     // cas 1: perte de x0 (à remplacer par x1)

					    if (pos == 0){
						  image[f][c][i]=d_0;
						  image[f][c+1][i]=d_0;
						  image[f+1][c][i]=d_1;
						  image[f+1][c+1][i]=d_2;
						}
					     // cas 2: perte de x1 (à remplacer par x3)
					    if (pos == 1){
						  image[f][c][i]=d_0;
						  image[f][c+1][i]=d_2;
						  image[f+1][c][i]=d_1;
						  image[f+1][c+1][i]=d_2;
						}
					     // cas 3: perte de x2 (à remplacer par x0)
					    if (pos == 2){
						  image[f][c][i]=d_0;
						  image[f][c+1][i]=d_1;
						  image[f+1][c][i]=d_0;
						  image[f+1][c+1][i]=d_2;
						}
					     // cas 2: perte de x3 (à remplacer par x2)
					    if (pos == 3){
						  image[f][c][i]=d_0;
						  image[f][c+1][i]=d_1;
						  image[f+1][c][i]=d_2;
						  image[f+1][c+1][i]=d_2;
						}
					}
				}
				else{
					for(int i=0; i < channels;i+=1){
					  	image[f][c][i]=-1;
					  	image[f][c+1][i]=-1;
					  	image[f+1][c][i]=-1;
					  	image[f+1][c+1][i]=-1;
					}
				}
			}
		}
	}

	unsigned char SetLSB(unsigned char val){
	  val = val | 0x01;
	  return val;
	}

	unsigned char ResetLSB(unsigned char val){
	  val = val & 0xFE;
	  return val;
	}

	unsigned char GetLSB(unsigned char val){
	  val = (val & 0x01);
	  return val;
	}
/*
	int hepx_2dec (unsigned char *hex, int n) // Conversion d'un nombre codé sur n octets (du poids fort au poids faible) en décimal
	{
		int value = 0;

		for (int i = n - 1; i >= 0; i--)	{  value = value * 256 + hex[i];  }
		return value;
	}

	void dec2hex (int value, unsigned char *hex, int n) // Conversion d'un entier en nombre codé sur n octets (du poids fort au poids faible)
	{	
		for (int i = 0; i < n; i++) hex[i]=0;

		for (int i = 0; i < n; i++)
		   {
	  	     hex[i] =  value % 256;
	  	     value = value / 256;
		   }
	}
*/
	unsigned char WatermarkGet(unsigned char a, unsigned char b, unsigned char c){
	  unsigned char code, position;
	  
	  code=(GetLSB(a) * 4) + (GetLSB(b) * 2) + GetLSB(c);

	  switch (code)
	  	{
		 case 0: position = 0; break;
		 case 7: position = 0; break;
		 case 1: position = 1; break;
		 case 6: position = 1; break;	 
		 case 2: position = 2; break;
		 case 5: position = 2; break;
		 case 3: position = 3; break;
		 case 4: position = 3; break;
		}
	  return position;
	}

	void WatermarkSet(unsigned char &a, unsigned char &b, unsigned char &c, int position){
	  unsigned char code;
	  
	  code=(GetLSB(a) * 4) + (GetLSB(b) * 2) + GetLSB(c);

	  switch (position)
	      	{
		 /* On doit tatouer que le pixel effacé est px_0 */
		 case 0:
		 	switch (code)
			      {
				case 1: c=ResetLSB(c);	break;
				case 2: b=ResetLSB(b);	break;
				case 3: a=SetLSB(a);	break;
				case 4: a=ResetLSB(a);	break;
				case 5: b=SetLSB(b);	break;
				case 6: c=SetLSB(c);	break;
			      }	
			break;      

		 /* On doit tatouer que le pixel effacé est px_1 */
		 case 1:
		 	switch (code)
			      {
				case 0: c=SetLSB(c);	break;
				case 2: a=SetLSB(a);	break;
				case 3: b=ResetLSB(b);	break;
				case 4: b=SetLSB(b);	break;
				case 5: a=ResetLSB(a);	break;
				case 7: c=ResetLSB(c);	break;
			      }	
			break;      

		 /* On doit tatouer que le pixel effacé est px_2 */
		 case 2:
		 	switch (code)
			      {
				case 0: b=SetLSB(b);	break;
				case 1: a=SetLSB(a);	break;
				case 3: c=ResetLSB(c);	break;
				case 4: c=SetLSB(c);	break;
				case 6: a=ResetLSB(a);	break;
				case 7: b=ResetLSB(b);	break;
			      }	
			break;      
	  
		 /* On doit tatouer que le pixel effacé est px_3 */
		 case 3:
		 	switch (code)
			      {
				case 0: a=SetLSB(a);	break;
				case 1: b=SetLSB(b);	break;
				case 2: c=SetLSB(c);	break;
				case 5: c=ResetLSB(c);	break;
				case 6: b=ResetLSB(b);	break;
				case 7: a=ResetLSB(a);	break;
			      }	
			break;
		}	
	  return;
	}

	void generateStream(){
		for (int i = 0; i < blocks.size(); i+=1){
			for (int j = 0; j < blocks[i].size(); j+=1){
				DataTibsStream* stream = new DataTibsStream(blocks[i][j]);
				new_list.push_back(stream);
			}
		}
	}

	void clearExtraPixels(){
		if((image_width%2)!=0 || (image_height%2)!=0){
			image.resize(image_height);
			for (int i = 0; (unsigned)i < image.size(); i+=1){
				image[i].resize(image_width);
			}
		}
	}

	void generateImage(){
		new_list.clear();
		new_list.resize(image_width*image_height);
		for (int i = 0, x = 0; (unsigned)i < image.size(); i+=1, x+=2){
			for (int j = 0, y = 0; (unsigned)j < image[i].size(); j+=1, y+=2){
				vector<int> channels;
				int flag = 0;
				for(int c = 0; (unsigned)c < image[i][j].size(); c+=1){
					if((int)image[i][j][c]==-1)
						flag = 1;
					channels.push_back((int)image[i][j][c]);
				}
				DataBlock* block=NULL;
				if(flag!=1)
					 block = new DataBlock(image[i][j].size(),channels,1,1);
				else
					block = new DataBlock();
				new_list[(i*image_width)+j]=block;
			}
		}
	}

};

#endif
