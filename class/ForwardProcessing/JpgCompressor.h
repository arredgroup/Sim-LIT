/**
  @file Jpg.h
  @class Jpg
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

#define PI 3.14159265
#define JPGE_MAX(a,b) (((a)>(b))?(a):(b))
#define JPGE_MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

class JpgCompressor: public ForwardProcessing{

private:	

	bool proceed;

	int jpg_width;
	int jpg_height;

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

	vector<DataType *> new_list;

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
		extra_w = (header->w*header->wb)%8;
		extra_h = (header->h*header->hb)%8;
		readImage(list,header);
		generateBlocks();
		convertColorSpace();
		generateAllDCT();
		computeQuantizationTable();
		quantizateAllDCT();
		reduceZerosToAll();
		applyHuffmanToAll();
		generateDatabytestream();
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
			clearAll();
			generateLocalUnsignedChar(list);
			getchar();
			resetHuffmanToAll();
			getchar();/*
			amplifyZerosToAll();
			redoQuantizateAllDCT();
			regenerateAllDCT();
			invertColorSpace();
			clearExtraPixels();
			generateImage();
			*/
			proceed = false;
		    if(show_data)
				cout << "JPEG Compression undo to Image\n";
			return 1;
		}
		return 1;
	}

	void clearAll(){
		image.clear();
		for(int i=0;i<dct_r.size();i+=1){
			for(int j=0;j<dct_r[i].size();j+=1){
				dct_r[i][j].clear();
			}
		}
		for(int i=0;i<dct_g.size();i+=1){
			for(int j=0;j<dct_g[i].size();j+=1){
				dct_g[i][j].clear();
			}
		}
		for(int i=0;i<dct_b.size();i+=1){
			for(int j=0;j<dct_b[i].size();j+=1){
				dct_b[i][j].clear();
			}
		}
		y_data.clear();
		cr_data.clear();
		cb_data.clear();
		compressed_blocks.clear();
		bytes_huffman.clear();
	}

private:

	unsigned char* generateEmpty(){
		unsigned char* vacio = (unsigned char*)malloc(sizeof(unsigned char)*64);
		for (int i = 0; i < 64; i+=1){
			vacio[i]=(unsigned char)0.0;
		}
		return vacio;
	}

	/***************************************************************
	*
	* FUNCTIONS FOR DECOMPRESSION
	*
	****************************************************************/

	void clearExtraPixels(){
		if(extra_w>0 || extra_h>0){
			vector< vector< vector <double> > > img_original;
			img_original.resize((jpg_height*8)-extra_h);
			for (int i = 0; i < img_original.size(); i+=1){
				img_original[i].resize((jpg_width*8)-extra_w);
			}
			for (int i = 0; i < (jpg_height*8)-extra_h; i+=1){
				for (int j = 0; j < (jpg_width*8)-extra_w; j+=1){
					img_original[i][j]=image[i][j];
				}
			}
			image.clear();
			image = img_original;
		}
	}

	void invertColorSpace(){
		for(int i=0;i<image.size();i+=1){
			for(int j=0;j<image[i].size();j+=1){
				if(image[i][j].size()==3)
					image[i][j] = RGB_to_YCC(image[i][j]);
				if(image[i][j].size()==1)
					image[i][j] = Y_to_YCC(RGB_to_Y(image[i][j]));
			}
		}
	}

	vector<double> YCC_to_RGB(vector<double> ycc){
		vector<double> rgb;
		double R = round(ycc[0] + 1.402  * (ycc[2]-128));
		double G = round(ycc[1] - 0.34414* (ycc[1]-128) - 0.71414 * (ycc[2]-128));
		double B = round(ycc[2] + 1.772  * (ycc[1]-128));
		rgb.push_back(R);
		if(G==0 && B==0)
			return rgb;
		rgb.push_back(G);
		rgb.push_back(B);
		return rgb;
	}

	void calculateIDCT(int fb, int cb){
		int pos = (fb*jpg_width)+cb;
		vector< vector<double>> dr;
		vector< vector<double>> dg;
		vector< vector<double>> db;
		image.clear();
		dr.resize(8);
		dg.resize(8);
		db.resize(8);
		image.resize(jpg_height*8);
		for (int i = 0; i < image.size(); i+=1){
			image[i].resize(jpg_width*8);
		}
		double Cu, Cv;
		for (int i = 0; i < 8; i+=1){
			dr[i].resize(8);
			dg[i].resize(8);
			db[i].resize(8);
		}
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				dr[f][c] = 0.0;
				dg[f][c] = 0.0;
				db[f][c] = 0.0; 
				for (int i = 0; i < 8; i+=1) {
		            for (int j = 0; j < 8; j+=1) {
						if (f == 0) {
		                	Cu = 1.0 / sqrt(2.0);
			            } else {
			                Cu = 1.0;
			            }

			            if (c == 0) {
			                Cv = 1.0 / sqrt(2.0);
			            } else {
			                Cv = (1.0);
			            }  
		                //dr[f][c] += image[pos][0] * cos(M_PI/((float)8)*(f+1./2.)*i) * cos(M_PI/((float)8)*(c+1./2.)*j);
		                dr[f][c] += dct_r[pos][f][c] * cos( (((2*i)+1)*f*PI)/16 ) * cos( (((2*j)+1)*c*PI)/16 );
		                dg[f][c] += dct_g[pos][f][c] * cos( (((2*i)+1)*f*PI)/16 ) * cos( (((2*j)+1)*c*PI)/16 );
		                db[f][c] += dct_b[pos][f][c] * cos( (((2*i)+1)*f*PI)/16 ) * cos( (((2*j)+1)*c*PI)/16 );
		            }              
		        }
		        dr[f][c] = round(0.25 * dr[f][c]);
		        dg[f][c] = round(0.25 * dg[f][c]);
		        db[f][c] = round(0.25 * db[f][c]);
			}
		}
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				//pos = ((fb*jpg_width*8)+(cb*64))+(f*8)+c;
				vector<double> ycc;
				ycc.push_back(dr[f][c]);
				ycc.push_back(dg[f][c]);
				ycc.push_back(db[f][c]);
				image[(fb*8)+f][(cb*8)+c] = ycc;
			}
		}
	}

	void regenerateAllDCT(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				calculateIDCT(f,c);
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

	void redoQuantizateDCT(int fb, int cb){
		int pos = fb*jpg_width+cb;
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				int x = (f*8)+c;
				dct_r[pos][f][c] = round(dct_r[pos][f][c] * quantization_table[x]);
				dct_g[pos][f][c] = round(dct_g[pos][f][c] * quantization_table[x]);
				dct_b[pos][f][c] = round(dct_b[pos][f][c] * quantization_table[x]);
			}
		}
	}

	void amplifyZerosToAll(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				amplifyZeros(f,c);
			}
		}
	}

	void amplifyZeros(int fb, int cb){
		int block = fb*jpg_width+cb;
		int size = 0;
		vector<double> with_zero_r;
		with_zero_r.resize(64,0.0);
		vector<double> with_zero_g;
		with_zero_g.resize(64,0.0);
		vector<double> with_zero_b;
		with_zero_b.resize(64,0.0);
		int i=0;
		unsigned char* uc_r = compressed_blocks[block][0];
		size = sizeof(uc_r) / sizeof(unsigned char);
		for(i=0;i<size;i+=1)
			with_zero_r[i]=(float)uc_r[i];
		while(i<64){
			with_zero_r[i]=0.0;
		}
		unsigned char* uc_g = compressed_blocks[block][1];
		size = sizeof(uc_g) / sizeof(unsigned char);
		for(i=0;i<size;i+=1)
			with_zero_r[i]=(float)uc_g[i];
		while(i<64){
			with_zero_r[i]=0.0;
		}
		unsigned char* uc_b = compressed_blocks[block][2];
		size = sizeof(uc_b) / sizeof(unsigned char);
		for(i=0;i<size;i+=1)
			with_zero_r[i]=(float)uc_b[i];
		while(i<64){
			with_zero_r[i]=0.0;
		}
		int x=0;
		while(x<64){
			for (int f = 0; f < 8; f+=1){
				for (int c = 0; c < 8; c+=1){
					if(zigzag[x]==(f*8)+c){
						dct_r[block][f][c] = with_zero_r[(f*8)+c];
						dct_g[block][f][c] = with_zero_g[(f*8)+c];
						dct_b[block][f][c] = with_zero_b[(f*8)+c];
					}
				}
			}
			x+=1;
		}
	}

	void generateLocalUnsignedChar(vector<DataType *> *list){
		int size = list->size();
		int x=0;
		vector < unsigned char*> compressed_block;
		vector <int> bytes_block;
		DataType * compressed = NULL;
		for(int i = 0;i < size;i+=1){
			compressed = list->at(i);
			if(compressed!=NULL && compressed->isValid()){
				unsigned char* huffman_stream = (unsigned char*)compressed->getExtras();
				int size_bytes = (int)compressed->getSize();
				bytes_block.push_back(size_bytes);
				compressed_block.push_back(huffman_stream);
			}
			else{
				unsigned char* huffman_stream = (unsigned char*)malloc(sizeof(unsigned char));
				huffman_stream[0] = '\0';
				bytes_block.push_back(0);
				compressed_block.push_back(huffman_stream);
			}
			x+=1;
			if(x==3){
				bytes_huffman.push_back(bytes_block);
				compressed_blocks.push_back(compressed_block);
				vector < unsigned char*> compressed_block;
				vector <int> bytes_block;
				x=0;
			}
		}
		cout << "IMPORTADA... SIZE:" << compressed_blocks.size() << "\n";
		getchar();
	}
/*
	void substring(unsigned char* sbtr, unsigned char* str, int pinit, int pend){
		int size = pend - pinit;
		for (int i = 0; i < size; i+=1){
			sbtr[i] = str[pinit+i];
		}
	}
*/
	void resetHuffmanToAll(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				resetHuffman(f,c);
			}
		}
	}

	void resetHuffman(int fb, int cb){
 		int block = fb*jpg_width+cb;
 		int size = compressed_blocks[block].size();
		for(int i = 0;i< size && i < bytes_huffman[block].size();i+=1){
			if(compressed_blocks[block][i][0]!='\0' && bytes_huffman[block][i]>0){
				int bytes_b = bytes_before_huffman[block][i];
				int bytes_h = bytes_huffman[block][i];
				unsigned char* c = (unsigned char*)malloc(sizeof(unsigned char)*bytes_b);
				cout << "DESCOMPRIMIR... ";
				getchar();
				Huffman_Uncompress(compressed_blocks[block][i], c,  bytes_h, bytes_b);
				cout << "DESCOMPRIMIÓ!!!";
				getchar();
				compressed_blocks[block][i] = c;
			}
			else{
				compressed_blocks[block][i] = generateEmpty();
			}
		}
	}

	/***************************************************************
	*
	* FUNCTIONS FOR COMPRESSION
	*
	****************************************************************/

	void generateDatabytestream(){
		for (int f = 0; f < compressed_blocks.size(); f+=1){
			for (int c = 0; c < compressed_blocks[f].size(); c+=1){
				int byte = bytes_huffman[f][c];
				DataByteStream* db = new DataByteStream(compressed_blocks[f][c],byte);
				new_list.push_back(db);
			}
		}
	}

	void reduceZerosToAll(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				reduceZeros(f,c);
			}
		}
	}

	void reduceZeros(int fb, int cb){
		int block = fb*jpg_width+cb;
		vector<double> without_zero_r;
		without_zero_r.resize(64,0.0);
		vector<double> without_zero_g;
		without_zero_g.resize(64,0.0);
		vector<double> without_zero_b;
		without_zero_b.resize(64,0.0);
		//USAR ZIGZAG!!!
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				int pos = zigzag[(f*8)+c];
				without_zero_r[pos] = (dct_r[block][f][c]);
				without_zero_g[pos] = (dct_g[block][f][c]);
				without_zero_b[pos] = (dct_b[block][f][c]);
			}
		}
		 y_data.push_back(clearZero(without_zero_r));
		cr_data.push_back(clearZero(without_zero_g));
		cb_data.push_back(clearZero(without_zero_b));
	}

	vector<double> clearZero(vector<double> v){
		for (int i = 0; i < v.size(); i+=1){
			if(isEmpty(i,v))
				v.resize(i+1);
		}
		return v;
	}

	bool isEmpty(int pos, vector <double> v){
		for (int i = pos; i < v.size(); i+=1){
			if(v[i]!=0.0)
				return false;
		}
		return true;
	}

	void applyHuffmanToAll(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				applyHuffman(f,c);
			}
		}
	}

	void applyHuffman(int fb, int cb){
		int block = fb*jpg_width+cb;
		vector<unsigned char*> huffman;
		vector<int> bytes_before;
		vector<int> bytes;
		int rbyte;
		int gbyte;
		int bbyte;
		unsigned char* r = (unsigned char*)malloc(sizeof(unsigned char)*y_data[block].size());
		unsigned char* g = (unsigned char*)malloc(sizeof(unsigned char)*cr_data[block].size());
		unsigned char* b = (unsigned char*)malloc(sizeof(unsigned char)*cb_data[block].size());

		for (int i = 0; i < y_data[block].size(); i+=1){
			r[i] = static_cast<unsigned>(y_data[block][i]);
		}
		for (int i = 0; i < cr_data[block].size(); i+=1){
			g[i] = static_cast<unsigned>(cr_data[block][i]);
		}
		for (int i = 0; i < cb_data[block].size(); i+=1){
			b[i] = static_cast<unsigned>(cb_data[block][i]);
		}
		unsigned char* cr = (unsigned char*)malloc(sizeof(unsigned char)*64);
		unsigned char* cg = (unsigned char*)malloc(sizeof(unsigned char)*64);
		unsigned char* c_b = (unsigned char*)malloc(sizeof(unsigned char)*64);
		//unsigned char* cr,cg,c_b;

		rbyte = Huffman_Compress(r,cr,y_data[block].size());
		gbyte = Huffman_Compress(g,cg,cr_data[block].size());
		bbyte = Huffman_Compress(b,c_b,cb_data[block].size());

		bytes_before.push_back(y_data[block].size());
		huffman.push_back(cr);
		bytes.push_back(rbyte);

		bytes_before.push_back(cr_data[block].size());
		huffman.push_back(cg);
		bytes.push_back(gbyte);

		bytes_before.push_back(cb_data[block].size());
		huffman.push_back(c_b);
		bytes.push_back(bbyte);

		compressed_blocks.push_back(huffman);
		bytes_huffman.push_back(bytes);
		bytes_before_huffman.push_back(bytes_before);
	}

	void readImage(vector<DataType *> *list, HEAD *header){
		jpg_width = (header->w*header->wb)/8;
		jpg_height = (header->h*header->hb)/8;
		if((header->w*header->wb)%8>0)
			jpg_width+=1;
		if((header->h*header->hb)%8>0)
			jpg_height+=1;
		/* EN ESTA PARTE SIMULO ALGUNOS VALORES */
			quality = 80;
			//type_subsampling = 1;
		/*  FIN SIMULADOS  */
		image.resize(header->h*header->hb);
		for (int i = 0; i < image.size(); i+=1){
			image[i].resize(header->w*header->wb);
		}
		int w_block = 0;
		int h_block = 0;
		int chnl_block = 0;
		int* head_block = NULL;
		vector<Pixel> *block = NULL;
		vector<int> aux;
		vector<double> last_w;
		int pos = 0;
		int flag = 0;
		for (int i = 0; i < header->h; i+=1){
			for (int j = 0; j < header->w; j+=1){
				pos = (i*(header->h))+j;
				if(pos < list->size()){
					head_block = list->at(pos)->getContent();
					w_block = head_block[2];
					h_block = head_block[1];
					chnl_block = head_block[3];
					//Here, we read each block inside in the list
					block = (vector<Pixel>*)list->at(pos)->getExtras();
					for(int f = 0; f < h_block; f+=1){
						for(int c = 0;c < w_block; c+=1){
							vector<double> rgb;
							flag+=1;
							aux = block->at((f*h_block)*c).getChannels();
							for (int x = 0; x < chnl_block && x < aux.size(); x+=1){
								rgb.push_back(round(aux[x]+0.0));
							}
							image[(i*h_block)+f][(j*w_block)+c]=rgb;
							//int pos_img = ((i*header->h)+(j*h_block*w_block))+(f*h_block)+c;
							//image[pos_img]=rgb;
						}
					}
				}
			}
		}
	}

	void generateBlocks(){
		for (int i = 0; i < image.size(); i+=1){
			if(extra_w>0){
				int pos = 0;
				int j=image[i].size();
				while(pos<extra_h){
					vector <double> rgb;
					for(int x =0; x < image[i][j-1].size();x+=1){
						rgb.push_back(image[i][j-1][x]);
					}
					image[i].push_back(rgb);
					pos+=1;
				}
			}
		}
		if(extra_h>0){
			int pos = 0;
			int j=image.size();
			while(pos<extra_h){
				vector< vector <double>> aux;
				for (int i = 0; i < image[j-1].size(); i+=1){
					vector <double> rgb;
					for(int x =0; x < image[j-1][i].size();x+=1){
						rgb.push_back(image[j-1][i][x]);
					}
					aux.push_back(rgb);
				}
				image.push_back(aux);
				pos+=1;
			}
		}
	}

	void generateAllDCT(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				calculateDCT(f,c);
			}
		}
	}

	void convertColorSpace(){
		for(int i=0;i<image.size();i+=1){
			for(int j=0;j<image[i].size();j+=1){
				if(image[i][j].size()==3)
					image[i][j] = RGB_to_YCC(image[i][j]);
				if(image[i][j].size()==1)
					image[i][j] = Y_to_YCC(RGB_to_Y(image[i][j]));
			}
		}
	}

	void computeQuantizationTable(){
		float q;
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

	void quantizateAllDCT(){
		for (int f = 0; f < jpg_height; f+=1){
			for (int c = 0; c < jpg_width; c+=1){
				quantizateDCT(f,c);
			}
		}
	}

	void quantizateDCT(int fb, int cb){
		int pos = fb*jpg_width+cb;
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				int x = (f*8)+c;
				dct_r[pos][f][c] = round(dct_r[pos][f][c] / quantization_table[x]);
				dct_g[pos][f][c] = round(dct_g[pos][f][c] / quantization_table[x]);
				dct_b[pos][f][c] = round(dct_b[pos][f][c] / quantization_table[x]);
			}
		}
	}

/* AQUIIII */
	void calculateDCT(int fb, int cb){
		vector< vector<double>> dr;
		vector< vector<double>> dg;
		vector< vector<double>> db;
		dr.resize(8);
		dg.resize(8);
		db.resize(8);
		double Cu, Cv;
		for (int i = 0; i < 8; i+=1){
			dr[i].resize(8);
			dg[i].resize(8);
			db[i].resize(8);
		}
		for (int f = 0; f < 8; f+=1){
			for (int c = 0; c < 8; c+=1){
				dr[f][c] = 0.0;
				dg[f][c] = 0.0;
				db[f][c] = 0.0;
				if (f == 0) {
                	Cu = 1.0 / sqrt(2.0);
	            } else {
	                Cu = 1.0;
	            }

	            if (c == 0) {
	                Cv = 1.0 / sqrt(2.0);
	            } else {
	                Cv = (1.0);
	            }   
				for (int i = 0; i < 8; i+=1) {
		            for (int j = 0; j < 8; j+=1) {
		            	int row = (fb*8)+i;
		            	int col = (cb*8)+j;
		            	//int pos = ((fb*sqrt(image.size()))+(cb*64))+(i*8)+j;
		                //dr[f][c] += image[pos][0] * cos(M_PI/((float)8)*(f+1./2.)*i) * cos(M_PI/((float)8)*(c+1./2.)*j);
		                dr[f][c] += image[row][col][0] * cos( (((2*i)+1)*f*PI)/16 ) * cos( (((2*j)+1)*c*PI)/16 );
		                dg[f][c] += image[row][col][1] * cos( (((2*i)+1)*f*PI)/16 ) * cos( (((2*j)+1)*c*PI)/16 );
		                db[f][c] += image[row][col][2] * cos( (((2*i)+1)*f*PI)/16 ) * cos( (((2*j)+1)*c*PI)/16 );
		            }              
		        }
		        dr[f][c] = round((0.25) * Cu * Cv * dr[f][c]);
		        dg[f][c] = round((0.25) * Cu * Cv * dg[f][c]);
		        db[f][c] = round((0.25) * Cu * Cv * db[f][c]);
			}
		}
		dct_r.push_back(dr);
		dct_g.push_back(dg);
		dct_b.push_back(db);
	}

	vector<double> RGB_to_YCC(vector<double> rgb){ //[0]=R [1]=G [2]=B
		double Y  = 0.299    *rgb[0] + 0.587  *rgb[1] + 0.114  *rgb[2]- 128.0;
		double Cb = - 0.1687 *rgb[0] - 0.3313 *rgb[1] + 0.5    *rgb[2];
		double Cr = 0.5      *rgb[0] - 0.4187 *rgb[1] - 0.0813 *rgb[2];
		vector<double> YCC;
		YCC.push_back(Y);
		YCC.push_back(Cb);
		YCC.push_back(Cr);
		return YCC;
	}

	double RGB_to_Y(vector<double> rgb){
	    double Y = rgb[0]*0.299 + rgb[0]*0.587 + rgb[0]*0.114 - 128.0;
		return Y;
	}

	vector<double> Y_to_YCC(double element){
	    double Y  = element - 128.0;
		double Cb = 0;
		double Cr = 0;
		vector<double> YCC;
		YCC.push_back(Y);
		YCC.push_back(Cb);
		YCC.push_back(Cr);
		return YCC;
	}

};

#endif