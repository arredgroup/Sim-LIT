/**
  @file JpgCompressor.h
  @class JpgCompressor
  @brief Clase que implementa un tipo de compresión de JPG 
  @author Christopher Arredondo Flores
  @date 01/01/2016

**/

#ifndef CLASS_JPGCOMPRESSOR
#define CLASS_JPGCOMPRESSOR

#include "../Images.h"
#include "../Libs/huffman.c"
#include "../DataByteStream.h"
#include <cmath>

#define JPGE_MAX(a,b) (((a)>(b))?(a):(b))
#define JPGE_MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

class JpgCompressor: public ForwardProcessing{

private:	

	bool proceed;

	int jpg_width;
	int jpg_height;

	int image_width;
	int image_height;

	vector< vector< vector <double> > > image;
	vector< vector< vector <double> > > dct_r;
	vector< vector< vector <double> > > dct_g;
	vector< vector< vector <double> > > dct_b;

	vector< vector< double> > y_data;
	vector< vector< double> > cb_data;
	vector< vector< double> > cr_data;

	int zigzag[64] = { 0,1,8,16,9,2,3,10,17,24,32,25,18,11,4,5,12,19,26,33,40,48,41,34,27,20,13,6,7,14,21,28,35,42,49,56,57,50,43,36,29,22,15,23,30,37,44,51,58,59,52,45,38,31,39,46,53,60,61,54,47,55,62,63 };
	int s_std_lum_quant[64] = { 16,11,12,14,12,10,16,14,13,14,18,17,16,19,24,40,26,24,22,22,24,49,35,37,29,40,58,51,61,60,57,51,56,55,64,72,92,78,64,68,87,69,55,56,80,109,81,87,95,98,103,104,103,62,77,113,121,112,100,120,92,101,103,99 };
	int quantization_table[64];

	//enum subsampling { Y_ONLY = 0, H1V1 = 1, H2V1 = 2, H2V2 = 3 };

	//int type_subsampling;
	int quality;

	int extra_w;
	int extra_h;

	vector< vector<unsigned char*> > compressed_blocks;

	vector< vector<int > > bytes_before_huffman;
	vector< vector<int > > bytes_huffman;
	vector< vector< vector<int > > > negative_position;

	vector<DataType *> new_list;

	vector<int> losses;

public:

	JpgCompressor(){}

	~JpgCompressor(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_image){
		if(nParameters!=1){
            cout << "(JPEG): Parameters number are incorrect! STOP!\n";
            return 1;
		}
		quality = atoi((char*)parameters[0]);
		if ((quality < 1) || (quality > 100)) {
            cout << "(JPEG): Compression Quality must be between 1 and 100\n";
            return 1;
        }

		image_width = (header->w*header->wb);
		image_height = (header->h*header->hb);

		jpg_width = image_width/8;
		jpg_height = image_height/8;
		if(image_width%8!=0)
			jpg_width+=1;
		if(image_height%8!=0)
			jpg_height+=1;
		extra_w = image_width%8;
		extra_h = image_height%8;

		readImage(list, header);
		generateExtraPixels(); //Crea los bloques de 8x8
		convertColorSpace(); //realiza el cambio de RGB a YCC
		calculateAllDCT(); // Calcula la DCT en cada bloque
		computeQuantizationTable(); //Genera la tabla de cuantización
		quantizateAllDCT(); //cuantiza todos los bloques
		reduceZerosToAll(); // Elimina los ceros y genera un vector dado zigzag
		applyHuffmanToAll(); //Aplica Huffman
		generateDatabytestream(); //Exporta los elementos
		*list = new_list;
        if(show_data)
            cout << "JPEG Compression Applied to Image\n";
        if(export_image)
        	cout << "The Preview can't be exported\n";
        proceed = true;

		return 0;
	}

	int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_image){
		if(proceed){
			clearAll(); //Limpia la memoria del compresor
			generateLocalUnsignedChar(list); // Importa el vector de datos comprimidos
			resetHuffmanToAll(); //deshace Huffman
			amplifyZerosToAll();//Agrega los ceros faltantes, genera un bloque de ocho x ocho usando zigzag
			redoQuantizateAllDCT();//Invierte la Cuantización
			calculateAllIDCT();//Calcula la IDCT
			invertColorSpace();
			setInvalid();
			clearExtraPixels();
			generateImage();
			*list = new_list;
			proceed = false;
		    if(show_data)
				cout << "JPEG Compression undo to Image\n";
			return 0;
		}
		return 1;
	}

	void clearAll(){
		image.clear();
		y_data.clear();
		cr_data.clear();
		cb_data.clear();
		compressed_blocks.clear();
		bytes_huffman.clear();
		bytes_before_huffman.clear();
		dct_r.clear();
		dct_g.clear();
		dct_b.clear();
	}

	double min(double a, double b){
		if(a<b)
			return a;
		return b;
	}

	double max(double a, double b){
		if(a>b)
			return a;
		return b;
	}

	bool isEmpty(int pos, vector <double> v){
		for (int i = pos; (unsigned)i < v.size(); i+=1){
			if(v[i]!=0)
				return false;
		}
		return true;
	}

private:


	/***************************************************************
	*
	* FUNCTIONS FOR DECOMPRESSION
	*
	****************************************************************/

	void generateLocalUnsignedChar(vector<DataType *> *list){
		int size = list->size();
		bytes_huffman.resize(size);
		bytes_before_huffman.resize(size);
		compressed_blocks.resize(size);
		for(int i = 0;i < size;i+=1){
			vector < unsigned char*> compressed_block;
			vector <int> bytes_huffman_block;
			vector <int> bytes_original_block;
			DataByteStream * compressed = (DataByteStream *)list->at(i);
			if(compressed!=NULL && compressed->isValid()){
				compressed_block = compressed->getHuffman();
				bytes_huffman_block = compressed->getHuffmanBytes();
				bytes_original_block = compressed->getOriginalBytes();
			}
			else{
				losses.push_back(i);
				for(int c = 0;c<3;c+=1){
					unsigned char* huffman_stream = (unsigned char*)malloc(sizeof(unsigned char));
					huffman_stream[0] = (unsigned char)0;
					bytes_huffman_block.push_back(0);
					bytes_original_block.push_back(0);
					compressed_block.push_back(huffman_stream);
				}
			}
			bytes_huffman[i] = bytes_huffman_block;
			bytes_before_huffman[i] = bytes_original_block;
			compressed_blocks[i] = compressed_block;

		}
	}

	void resetHuffman(int fb, int cb){
 		int block = (fb*jpg_width)+cb;
 		int size = compressed_blocks[block].size();
		for(int i = 0;i < size && (unsigned)i < bytes_huffman[block].size() && (unsigned)i < bytes_before_huffman[block].size();i+=1){
			if(bytes_huffman[block][i]>0){
				int bytes_b = bytes_before_huffman[block][i];
				int bytes_h = bytes_huffman[block][i];
				unsigned char* uncompressed = (unsigned char*)malloc(sizeof(unsigned char)*bytes_b);
				Huffman_Uncompress(compressed_blocks[block][i], uncompressed,  bytes_h, bytes_b);
				free(compressed_blocks[block][i]);
				compressed_blocks[block][i] = uncompressed;
			}
			else{
				unsigned char* vacio = (unsigned char*)malloc(sizeof(unsigned char)*64);
				for (int i = 0; i < 64; i+=1){
					vacio[i]=(unsigned char)0;
				}
				compressed_blocks[block][i] = vacio;
			}
		}
	}

	void resetHuffmanToAll(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				resetHuffman(f,c);
			}
		}
	}

	void amplifyZeros(int fb, int cb){
		int block = (fb*jpg_width)+cb;
		int size = 0;
		vector<double> with_zero_r;
		with_zero_r.resize(64,0);
		vector<double> with_zero_g;
		with_zero_g.resize(64,0);
		vector<double> with_zero_b;
		with_zero_b.resize(64,0);
		int i=0;
		//int pos = 0;
		unsigned char* uc_r = compressed_blocks[block][0];
		size = bytes_before_huffman[block][0];
		for(i=0;i<size;i+=1){
			with_zero_r[i]=static_cast<double>(uc_r[i]);
			if(negative_position[block][0][i]!=0)
				with_zero_r[i] += (256*negative_position[block][0][i]);
		}
		while(i<64){
			with_zero_r[i]=0;
			i+=1;
		}
		unsigned char* uc_g = compressed_blocks[block][1];
		size = bytes_before_huffman[block][1];
		for(i=0;i<size;i+=1){
			with_zero_g[i]=static_cast<double>(uc_g[i]);
			if(negative_position[block][1][i]!=0)
				with_zero_g[i] += (256*negative_position[block][1][i]);
		}
		while(i<64){
			with_zero_g[i]=0;
			i+=1;
		}
		unsigned char* uc_b = compressed_blocks[block][2];
		size = bytes_before_huffman[block][2];
		for(i=0;i<size;i+=1){
			with_zero_b[i]=static_cast<double>(uc_b[i]);
			if(negative_position[block][2][i]!=0)
				with_zero_b[i] += (256*negative_position[block][2][i]);
		}
		while(i<64){
			with_zero_b[i]=0;
			i+=1;
		}

		for (int i = 0; i < 64; i+=1){
			int pos = zigzag[i];
			int col = pos%8;
			int row = pos/8;
			dct_r[block][row][col] = with_zero_r[i];
			dct_g[block][row][col] = with_zero_g[i];
			dct_b[block][row][col] = with_zero_b[i];
		}
	}

	void amplifyZerosToAll(){
		dct_r.resize(jpg_height*jpg_width);
		dct_g.resize(jpg_height*jpg_width);
		dct_b.resize(jpg_height*jpg_width);
		for (int b = 0; b < (jpg_height*jpg_width); b+=1){
			dct_r[b].resize(8);
			dct_g[b].resize(8);
			dct_b[b].resize(8);
			for (int i = 0; i < 8; i+=1){
				dct_r[b][i].resize(8,0);
				dct_g[b][i].resize(8,0);
				dct_b[b][i].resize(8,0);
			}
		}
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				amplifyZeros(f,c);
			}
		}
	}

	void redoQuantizateDCT(int fb, int cb){
		int pos = (fb*jpg_width)+cb;
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				int x = (f*8)+c;
				dct_r[pos][f][c] = dct_r[pos][f][c] * quantization_table[x];
				dct_g[pos][f][c] = dct_g[pos][f][c] * quantization_table[x];
				dct_b[pos][f][c] = dct_b[pos][f][c] * quantization_table[x];
			}
		}
	}

	void redoQuantizateAllDCT(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				redoQuantizateDCT(f,c);
			}
		}
	}

	void calculateIDCT(int fb, int cb){
		int pos = (fb*jpg_width)+cb;
		vector< vector<double>> dr;
		vector< vector<double>> dg;
		vector< vector<double>> db;
		dr.resize(8);
		dg.resize(8);
		db.resize(8);
		double Cu, Cv;
		for (int i = 0; i < 8; i+=1){
			dr[i].resize(8,0);
			dg[i].resize(8,0);
			db[i].resize(8,0);
		}

		for (int x = 0; x < 8; x+=1){
			for (int y = 0; y < 8; y+=1){
				for (int u = 0; u < 8; u+=1) {
		            for (int v = 0; v < 8; v+=1) {
						if (u == 0) {
		                	Cu = 1/sqrt(2);
			            } else {
			                Cu = 1;
			            }

			            if (v == 0) {
			                Cv = 1/sqrt(2);
			            } else {
			                Cv = 1;
			            }  
		                dr[x][y] += Cu * Cv * dct_r[pos][u][v] * cos( (((2*x)+1)*u*M_PI)/16 ) * cos( (((2*y)+1)*v*M_PI)/16 );
		                dg[x][y] += Cu * Cv * dct_g[pos][u][v] * cos( (((2*x)+1)*u*M_PI)/16 ) * cos( (((2*y)+1)*v*M_PI)/16 );
		                db[x][y] += Cu * Cv * dct_b[pos][u][v] * cos( (((2*x)+1)*u*M_PI)/16 ) * cos( (((2*y)+1)*v*M_PI)/16 );
		            }             
		        }
		        dr[x][y] = round(0.25 * dr[x][y]);
		        dg[x][y] = round(0.25 * dg[x][y]);
		        db[x][y] = round(0.25 * db[x][y]);
				image[(fb*8)+x][(cb*8)+y].push_back(dr[x][y]);
				image[(fb*8)+x][(cb*8)+y].push_back(dg[x][y]);
				image[(fb*8)+x][(cb*8)+y].push_back(db[x][y]);
			}
		}
	}

	void calculateAllIDCT(){
		image.resize(jpg_height*8);
		for (int i = 0; (unsigned)i < image.size(); i+=1){
			image[i].resize(jpg_width*8);
		}
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				calculateIDCT(f,c);
			}
		}
	}
	
	vector<double> YCC_to_RGB(vector<double> ycc){
		ycc[0] += 128;
		ycc[1] += 128;
		ycc[2] += 128;
		/*
	 	double R = (ycc[0] + 1.40200 * (ycc[2] - 128));
 		double G = (ycc[0] - 0.34414 * (ycc[1] - 128) - 0.71414 * (ycc[2] - 128));
 		double B = (ycc[0] + 1.77200 * (ycc[1] - 128));
		*/
		double B = (1.164 * (ycc[0] - 16))                     	      + (2.018 * (ycc[1] - 128));
		double G = (1.164 * (ycc[0] - 16)) - (0.813 * (ycc[2] - 128)) - (0.391 * (ycc[1] - 128));
		double R = (1.164 * (ycc[0] - 16)) + (1.596 * (ycc[2] - 128));

		R = round(max(min(R,255),0));
 		G = round(max(min(G,255),0));
 		B = round(max(min(B,255),0));

 		vector<double> rgb;
		rgb.push_back(R);
		rgb.push_back(G);
		rgb.push_back(B);

		return rgb;
	}

	void invertColorSpace(){
		for(int i=0;(unsigned)i<image.size();i+=1){
			for(int j=0;(unsigned)j<image[i].size();j+=1){
				image[i][j] = YCC_to_RGB(image[i][j]);
			}
		}
	}

	void setInvalid(){
		for(int c = 0; (unsigned)c < losses.size(); c+=1){
			int pos = losses[c];
			int row = floor(pos/jpg_width);
			int col = pos%jpg_width;
			for (int i = 0; i < 8; i+=1){
				for (int j = 0; j < 8; j+=1){
					for(int x = 0; (unsigned)x < image[(row*8)+i][(col*8)+j].size(); x+=1)
						image[(row*8)+i][(col*8)+j][x]=-1;
				}
			}
		}
	}

	void clearExtraPixels(){
		if(extra_w>0 || extra_h>0){
			image.resize((jpg_height*8)-extra_h);
			for (int i = 0; (unsigned)i < image.size(); i+=1){
				image[i].resize((jpg_width*8)-extra_w);
			}
		}
	}

	void generateImage(){
		new_list.clear();
		new_list.resize(image_width*image_height);
		for (int i = 0; (unsigned)i < image.size(); i+=1){
			for (int j = 0; (unsigned)j < image[i].size(); j+=1){
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

	/***************************************************************
	*
	* FUNCTIONS FOR COMPRESSION
	*
	****************************************************************/

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
		if(extra_w>0){
			for (int i = 0; (unsigned)i < image.size(); i+=1){
				int j=image[i].size();
				image[i].resize(j+extra_w);
				for(int pos = 0; pos<extra_w; pos+=1){
					vector <double> rgb;
					for(int x =0; (unsigned)x < image[i][j-1].size();x+=1){
						rgb.push_back(image[i][j-1][x]);
					}
					image[i][j+pos]=rgb;
				}
			}
		}
		if(extra_h>0){
			int i=image.size();
			image.resize(i+extra_h);
			for(int pos=0; pos < extra_h; pos+=1){
				vector< vector <double>> aux;
				for (int j = 0; (unsigned)j < image[i-1].size(); j+=1){
					vector <double> rgb;
					for(int x =0; (unsigned)x < image[i-1][j].size();x+=1){
						rgb.push_back(image[i-1][j][x]);
					}
					aux.push_back(rgb);
				}
				image[i+pos]=aux;
			}
		}
	}

	vector<double> RGB_to_YCC(vector<double> rgb){ //[0]=R [1]=G [2]=B

		double Y  = round((0.257    *rgb[0]) + (0.504  *rgb[1]) + (0.098  *rgb[2]) + 16 );
		double Cb = round((- 0.148  *rgb[0]) - (0.291  *rgb[1]) + (0.439  *rgb[2]) + 128);
		double Cr = round((0.439    *rgb[0]) - (0.368  *rgb[1]) - (0.071  *rgb[2]) + 128);
		
		Y -= 128;
		Cb -= 128;
		Cr -= 128;

		vector<double> ycc;
		ycc.push_back(Y);
		ycc.push_back(Cb);
		ycc.push_back(Cr);
		return ycc;
	}

	double RGB_to_Y(vector<double> rgb){
	    //double Y = rgb[0]*0.299 + rgb[0]*0.587 + rgb[0]*0.114 - 128.0;
	    double Y  = 0.257    *rgb[0] + 0.504  *rgb[0] + 0.098  *rgb[0] + 16;
		return Y-128;
	}

	vector<double> Y_to_YCC(double element){
	    double Y  = element;
		double Cb = 0;
		double Cr = 0;
		vector<double> YCC(3,0);
		YCC[0]=Y;
		YCC[1]=Cb;
		YCC[2]=Cr;
		return YCC;
	}

	void convertColorSpace(){
		for(int i=0;(unsigned)i<image.size();i+=1){
			for(int j=0;(unsigned)j<image[i].size();j+=1){
				if(image[i][j].size()==3){
					image[i][j] = RGB_to_YCC(image[i][j]);
				}
				if(image[i][j].size()==1){
					double y = RGB_to_Y(image[i][j]);
					image[i][j] = Y_to_YCC(y);
				}
			}
		}
	}

	void calculateDCT(int fb, int cb){
		int pos = (fb*jpg_width)+cb;
		vector< vector<double>> dr(8);
		vector< vector<double>> dg(8);
		vector< vector<double>> db(8);

		dct_r.resize(jpg_width*jpg_height);
		dct_g.resize(jpg_width*jpg_height);
		dct_b.resize(jpg_width*jpg_height);

		for (int i = 0; i < 8; i+=1){
			dr[i].resize(8,0);
			dg[i].resize(8,0);
			db[i].resize(8,0);
		}

		double Cu, Cv;
		for (int u = 0; u < 8; u+=1){
			for (int v = 0; v < 8; v+=1){
				if (u == 0) {
                	Cu = 1.0 / sqrt(2.0);
	            } else {
	                Cu = 1.0;
	            }

	            if (v == 0) {
	                Cv = 1.0 / sqrt(2.0);
	            } else {
	                Cv = (1.0);
	            }   
				for (int x = 0; x < 8; x+=1) {
		            for (int y = 0; y < 8; y+=1) {
		            	int row = (fb*8)+x;
		            	int col = (cb*8)+y;
		                dr[u][v] += image[row][col][0] * cos( (((2*x)+1)*u*M_PI)/16 ) * cos( (((2*y)+1)*v*M_PI)/16 );
		                dg[u][v] += image[row][col][1] * cos( (((2*x)+1)*u*M_PI)/16 ) * cos( (((2*y)+1)*v*M_PI)/16 );
		                db[u][v] += image[row][col][2] * cos( (((2*x)+1)*u*M_PI)/16 ) * cos( (((2*y)+1)*v*M_PI)/16 );
		            }              
		        }
		        dr[u][v] = round(0.25 * Cu * Cv * dr[u][v]);
		        dg[u][v] = round(0.25 * Cu * Cv * dg[u][v]);
		        db[u][v] = round(0.25 * Cu * Cv * db[u][v]);
			}
		}
		dct_r[pos]=dr;
		dct_g[pos]=dg;
		dct_b[pos]=db;
	}

	void calculateAllDCT(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				calculateDCT(f,c);
			}
		}
	}

	void computeQuantizationTable(){
		double q;
	    if (quality < 50) {
	        q = 5000.0 / quality;
	    } else {
	        q = 200.0 - quality * 2.0;
	    }
	    for (int i = 0; i < 64; i+=1) {
	        int j = s_std_lum_quant[i];
	        j = (j * q + 50L) / 100L;
	        quantization_table[i] = JPGE_MIN(JPGE_MAX(j, 1), 1024/3);
	    }
	    // DC quantized worse than 8 makes overall quality fall off the cliff
	    if (quantization_table[0] > 8) quantization_table[0] = (quantization_table[0]+8*3)/4;
	    if (quantization_table[1] > 24) quantization_table[1] = (quantization_table[1]+24)/2;
	    if (quantization_table[2] > 24) quantization_table[2] = (quantization_table[2]+24)/2;    
	}

	void quantizateDCT(int fb, int cb){
		int pos = (fb*jpg_width)+cb;
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				int x = (f*8)+c;
				dct_r[pos][f][c] = roundToZero(dct_r[pos][f][c],quantization_table[x]);
				dct_g[pos][f][c] = roundToZero(dct_g[pos][f][c],quantization_table[x]);
				dct_b[pos][f][c] = roundToZero(dct_b[pos][f][c],quantization_table[x]);
			}
		}
	}

	void quantizateAllDCT(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				quantizateDCT(f,c);
			}
		}
	}

	vector<double> clearZero(vector<double> v){
		for (int i = 0; (unsigned)i < v.size(); i+=1){
			if(isEmpty(i,v))
				v.resize(i);
		}
		return v;
	}

	void reduceZeros(int fb, int cb){
		int block = (fb*jpg_width)+cb;
		vector<double> without_zero_r;
		without_zero_r.resize(64,0);
		vector<double> without_zero_g;
		without_zero_g.resize(64,0);
		vector<double> without_zero_b;
		without_zero_b.resize(64,0);
		//USE ZIGZAG!!!
		for (int i = 0; i < 64; i+=1){
			int pos = zigzag[i];
			int col = pos%8;
			int row = pos/8;
			without_zero_r[i] = dct_r[block][row][col];
			without_zero_g[i] = dct_g[block][row][col];
			without_zero_b[i] = dct_b[block][row][col];
		}
		without_zero_r = clearZero(without_zero_r);
		y_data[block]  = without_zero_r;
		without_zero_g = clearZero(without_zero_g);
		cb_data[block] = without_zero_g;
		without_zero_b = clearZero(without_zero_b);
		cr_data[block] = without_zero_b;
	}

	void reduceZerosToAll(){
		y_data.resize(jpg_width*jpg_height);
		cb_data.resize(jpg_width*jpg_height);
		cr_data.resize(jpg_width*jpg_height);
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				reduceZeros(f,c);
			}
		}
	}

	void applyHuffman(int fb, int cb){
		int block = (fb*jpg_width)+cb;
		vector<unsigned char*> huffman(3);
		vector<int> bytes_before(3);
		vector<int> bytes(3);
		vector< vector<int> > negative_block(3);
		vector<int> negative_y(64,0);
		vector<int> negative_cb(64,0);
		vector<int> negative_cr(64,0);

		int ybyte;
		int cbbyte;
		int crbyte;
		unsigned char* o_y =  (unsigned char*)malloc(sizeof(unsigned char)* (y_data[block].size()));
		unsigned char* o_cb = (unsigned char*)malloc(sizeof(unsigned char)*(cb_data[block].size()));
		unsigned char* o_cr = (unsigned char*)malloc(sizeof(unsigned char)*(cr_data[block].size()));
		int extra_byte_y = 0;
		for (int i = 0; (unsigned)i < y_data[block].size(); i+=1){
			if(y_data[block][i]<0){
				negative_y[i]= floor((double)(y_data[block][i])/256);
				extra_byte_y += floor((double)(y_data[block][i])/256);
			}
			if(y_data[block][i]>255){
				negative_y[i]=floor((double)(y_data[block][i])/256);
				extra_byte_y += floor((double)(y_data[block][i])/256);
			}
			o_y[i] = static_cast<unsigned char>(y_data[block][i]);
		}
		int extra_byte_cb = 0;
		for (int i = 0; (unsigned)i < cb_data[block].size(); i+=1){
			if(cb_data[block][i]<0){
				negative_cb[i]= floor((double)(cb_data[block][i])/256);
				extra_byte_cb += floor((double)(cb_data[block][i])/256);
			}
			if(cb_data[block][i]>255){
				negative_cb[i]=floor((double)(cb_data[block][i])/256);
				extra_byte_cb+= floor((double)(cb_data[block][i])/256);
			}
			o_cb[i] = static_cast<unsigned char>(cb_data[block][i]);
		}
		int extra_byte_cr = 0;
		for (int i = 0; (unsigned)i < cr_data[block].size(); i+=1){
			if(cr_data[block][i]<0){
				negative_cr[i]= floor((double)(cr_data[block][i])/256);
				extra_byte_cr+= floor((double)(cr_data[block][i])/256);
			}
			if(cr_data[block][i]>255){
				negative_cr[i]= floor((double)(cr_data[block][i])/256);
				extra_byte_cr+= floor((double)(cr_data[block][i])/256);
			}
			o_cr[i] = static_cast<unsigned char>(cr_data[block][i]);
		}

		unsigned char* h_y =  (unsigned char*)malloc(sizeof(unsigned char)* (y_data[block].size()+390));
		unsigned char* h_cb = (unsigned char*)malloc(sizeof(unsigned char)*(cb_data[block].size()+390));
		unsigned char* h_cr = (unsigned char*)malloc(sizeof(unsigned char)*(cr_data[block].size()+390));
 
		negative_block[0] = negative_y;
		negative_block[1] = negative_cb;
		negative_block[2] = negative_cr;

		ybyte = Huffman_Compress(o_y,h_y,y_data[block].size());
		cbbyte = Huffman_Compress(o_cb,h_cb,cb_data[block].size());
		crbyte = Huffman_Compress(o_cr,h_cr,cr_data[block].size());

		bytes_before[0] = y_data[block].size();
		huffman[0] = h_y;
		bytes[0] = ybyte;

		bytes_before[1] = cb_data[block].size();
		huffman[1] = h_cb;
		bytes[1] = cbbyte;

		bytes_before[2] = cr_data[block].size();
		huffman[2] = h_cr;
		bytes[2] = crbyte;

		compressed_blocks[block] = huffman;
		bytes_huffman[block] = bytes;
		bytes_before_huffman[block] = bytes_before;
		negative_position[block] = negative_block;
	}

	void applyHuffmanToAll(){
		compressed_blocks.resize(jpg_width*jpg_height);
		bytes_huffman.resize(jpg_width*jpg_height);
		bytes_before_huffman.resize(jpg_width*jpg_height);
		negative_position.resize(jpg_width*jpg_height);
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				applyHuffman(f,c);
			}
		}
	}

	void generateDatabytestream(){
		new_list.resize(compressed_blocks.size());
		for (int f = 0; (unsigned)f < compressed_blocks.size(); f+=1){
			DataByteStream* db = new DataByteStream(compressed_blocks[f],bytes_before_huffman[f],bytes_huffman[f]);
			new_list[f] = db;
		}
		bytes_huffman.clear();
		bytes_before_huffman.clear();
	}

	double roundToZero(double j, int quant){
		if (j < 0) {
	        double jtmp = -j + (quant >> 1);
	        return (jtmp < quant) ? 0 : static_cast<double>(-(jtmp / quant));
	    } else {
	        double jtmp = j + (quant >> 1);
	        return (jtmp < quant) ? 0 : static_cast<double>((jtmp / quant));
	    }
	}


};

#endif