/**
  @file LeicaCompressor.h
  @class LeicaCompressor
  @brief Clase que implementa un tipo de compresión de Tibs 
  @author Christopher Arredondo Flores
  @date 01/01/2016

**/

#ifndef CLASS_AHCRCOMPRESSOR
#define CLASS_AHCRCOMPRESSOR

#include "../Images.h"
#include <cmath>
#include "../DataAhcrStream.h"

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))

using namespace std;

class AhcrCompressor: public ForwardProcessing{

	bool proceed;

	vector< vector< vector <double> > > image;
	int image_width;
	int image_height;

	int extra_w;
	int extra_h;

	int ahcr_width;
	int ahcr_height;
	//int image_compressed_w;
	//int image_compressed_h;
	int chnls;

	vector< vector<double>> r;
	vector< vector<double>> g;
	vector< vector<double>> b;

	vector< vector<int>> pos_r;
	vector< vector<int>> pos_g;
	vector< vector<int>> pos_b;

	vector< DataType *> new_list;


public:

	AhcrCompressor(){}

	~AhcrCompressor(){}

	int make(vector<DataType *> *list, HEAD *header, vector<void*> parameters, int nParameters, bool show_data, bool export_image){
		image_width = (header->w);
		image_height = (header->h);

		ahcr_width = image_width/3;
		ahcr_height = image_height/3;
		
		if(image_width%3!=0)
			ahcr_width+=1;
		if(image_height%3!=0)
			ahcr_height+=1;
		
		extra_w = 3-(image_width%3);
		if(extra_w==3)
			extra_w=0;
		extra_h = 3-(image_height%3);
		if(extra_h==3)
			extra_h=0;

		chnls = header->chnls;
		readImage(list,header);
		generateExtraPixels();
		readBlocks();
		generateAllCompressedBlocks();
		proceed = false;
		if(show_data)
			cout << "AHCR Compression Applied to Image";
		return 0;
	}

	int unmake(vector<DataType *> *list, HEAD *header, bool show_data, bool export_image){
		if(proceed){
			//generateLocalBlocks();

			return 0;
		}
		return 1;
	}

private:
/*
	void generateLocalBlocks(vector<DataType *> *list){
		int size = list->size();
		r.clear();
		pos_r.clear();
		g.clear();
		pos_g.clear();
		b.clear();
		pos_b.clear();
		if(chnls==1){			
			for (int i = 0; i < size; i+=1){
				if(list->at(i)->isValid()){
					DataAhcrStream * blk_g = (DataAhcrStream *)list->at(i);
					g.push_back(blk_g->getBlock());
					pos_g.push_back(blk_g->getPositions());
				}
				else{	
					double blk_empty[] = {0.0,0.0,0.0,0.0,0.0,0.0};
  					vector<double> blk (blk_empty, blk_empty + sizeof(blk_empty) / sizeof(double));
  					g.push_back(blk);

  					int pos_empty[] = {0,1,2};
  					vector<int> pos (pos_empty, pos_empty + sizeof(pos_empty) / sizeof(int));
  					pos_g.push_vack(blk);
				}
			}
		}
		if(chnls==3){
			for (int i = 0; i < size; i+=3){
				if(list->at(i)->isValid()){
					DataAhcrStream * blk_r = (DataAhcrStream *)list->at(i);
					r.push_back(blk_r->getBlock());
					pos_r.push_back(blk_r->getPositions());
				}
				else{	
					double blk_empty[] = {0.0,0.0,0.0,0.0,0.0,0.0};
  					vector<double> blk (blk_empty, blk_empty + sizeof(blk_empty) / sizeof(double));
  					r.push_back(blk);

  					int pos_empty[] = {0,1,2};
  					vector<int> pos (pos_empty, pos_empty + sizeof(pos_empty) / sizeof(int));
  					pos_r.push_vack(blk);
				}

				if(list->at(i+1)->isValid()){
					DataAhcrStream * blk_g = (DataAhcrStream *)list->at(i+1);
					g.push_back(blk_g->getBlock());
					pos_g.push_back(blk_g->getPositions());
				}
				else{	
					double blk_empty[] = {0.0,0.0,0.0,0.0,0.0,0.0};
  					vector<double> blk (blk_empty, blk_empty + sizeof(blk_empty) / sizeof(double));
  					g.push_back(blk);

  					int pos_empty[] = {0,1,2};
  					vector<int> pos (pos_empty, pos_empty + sizeof(pos_empty) / sizeof(int));
  					pos_g.push_vack(blk);
				}

				if(list->at(i+2)->isValid()){
					DataAhcrStream * blk_b = (DataAhcrStream *)list->at(i+2);
					b.push_back(blk_b->getBlock());
					pos_b.push_back(blk_b->getPositions());
				}
				else{	
					double blk_empty[] = {0.0,0.0,0.0,0.0,0.0,0.0};
  					vector<double> blk (blk_empty, blk_empty + sizeof(blk_empty) / sizeof(double));
  					b.push_back(blk);

  					int pos_empty[] = {0,1,2};
  					vector<int> pos (pos_empty, pos_empty + sizeof(pos_empty) / sizeof(int));
  					pos_b.push_vack(blk);
				}
			}
		}
	}

	void generateAllUncompressedBlocks(){
		for (int i = 0; i < (ahcr_width*ahcr_height); i+=1){
			if(chnls==1){
				g.at(i) = generateUncompressBlock(g.at(i), pos_g.at(i), 1);
			}
			if(chnls==3){
				r.at(i) = generateUncompressBlock(r.at(i), pos_r.at(i), 0);
				g.at(i) = generateUncompressBlock(g.at(i), pos_g.at(i), 1);
				b.at(i) = generateUncompressBlock(b.at(i), pos_b.at(i), 2);
			}
		}
	}

	vector<double> generateUncompressBlock(vector<double> block, vector<int> pos ,int chnl){
		vector<double> uncompressed;
		int p_min = pos[0];
		int p_max = pos[1];
		int p_med = pos[2];

		return uncompressed;

	}
*/
/*
*
*	COMPRESSION METHODs
*
*/

	void readImage(vector<DataType *> *list, HEAD* header){
		image.resize(image_height);
		for (int i = 0; (unsigned)i < image.size(); i+=1){
			image[i].resize(image_width);
		}
		int w_block = 0;
		int h_block = 0;
		int chnl_block = 0;
		int pos = 0;
		for (int i = 0; i < header->h/header->hb; i+=1){
			for (int j = 0; j < header->w/header->wb; j+=1){
				pos = (i*(header->h/header->hb))+j;
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

	void clearExtraPixels(){
		if(extra_w>0 || extra_h>0){
			image.resize((ahcr_height*8)-extra_h);
			for (int i = 0; (unsigned)i < image.size(); i+=1){
				image[i].resize((ahcr_width*8)-extra_w);
			}
		}
	}

	void readBlocks(){
		//SE RECORRE LA IMAGEN EN BLOQUES DE 3X3
		cout << "IMG-SIZE:" << image.size() << "\n";
		for (int i = 0; i < image.size(); i+=3){
			for(int j = 0; j < image.at(i).size(); j+=3){
				//SE LEE EL BLOQUE DE 3x3
				if(chnls==1){
					g.push_back(readMiniBlock(i,j,0));

				}
				if(chnls==3){
					r.push_back(readMiniBlock(i,j,0));
					g.push_back(readMiniBlock(i,j,1));
					b.push_back(readMiniBlock(i,j,2));
				}
			}
		}
	}

	vector <double> readMiniBlock(int i, int j, int c){
		vector<double> block;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				double px = image[i+x][j+y][c];
				block.push_back(px);
			}
		}
		return block;
	}

	void generateAllCompressedBlocks(){
		for (int i = 0; i < g.size(); i+=1){
			if(chnls==1){
				//g[i][j] = 
				generateCompressedBlock(g[i],1);
			}
			if(chnls==3){
				//r[i][j] = 
				generateCompressedBlock(r[i],0);
				//g[i][j] = 
				generateCompressedBlock(g[i],1);
				//b[i][j] = 
				generateCompressedBlock(b[i],2);
			}
		}
	}

	//vector<double> 
	void generateCompressedBlock(vector<double> block, int chnl){
		double min, max, med;
		int pos_min, pos_max, pos_med;
		min = getMin(block);
		pos_min = getPositionMin(block);
		max = getMax(block);
		pos_max = getPositionMax(block);
		med = getMed(block);
		pos_med = getPositionMed(block);
		vector<double> comp_block;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				int pos = (x*3+y);
				if(pos!=pos_min && pos!=pos_max && pos!=pos_med){
					comp_block.push_back(block[pos]);
				}
			}
		}
		comp_block[0] = change_binary((int)comp_block[0], pos_min);
		comp_block[1] = change_binary((int)comp_block[1], pos_max);
		comp_block[2] = change_binary((int)comp_block[2], pos_med);
		vector<int> positions;
		positions.push_back(pos_min);
		positions.push_back(pos_max);
		positions.push_back(pos_med);/*
		switch(chnl){
			case 0:{
				pos_r.push_back(positions);
			}break;
			case 1:{
				pos_g.push_back(positions);
			}break;
			case 2:{
				pos_b.push_back(positions);
			}break;
		}*/
		DataAhcrStream * blk = new DataAhcrStream(comp_block,positions);
		new_list.push_back(blk);
		//return comp_block;
	}

	double change_binary(int element, int position){
		vector<int> binary_element;
		vector<int> binary_position;
		do{
			binary_position.push_back(position%2);
			position /= 2;	
		}
		while(position>0);

		while(binary_position.size()<4)
			binary_position.push_back(0);
		do{
			binary_element.push_back(element%2);
			element /= 2;	
		}
		while(element>0);
		for (int i = 0; i < binary_position.size() && i < binary_element.size(); i+=1){
			binary_element[i]=binary_position[i];
		}
		double changed = 0;
		for (int i = binary_element.size()-1; i >= 0; i-=1){
			if(binary_element[i]!=0)
				changed += pow(2,i);
		}
		return changed;
	}

	double getMin(vector<double> block){
		double min;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				if(x==0 && y==0){
					min = block[x*3+y];
				}
				if(block[x*3+y]<min){
					min = block[x*3+y];
				}
			}
		}
		return min;
	}

	int getPositionMin(vector<double> block){
		double min = getMin(block);
		int pos = 0;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				if(block[x*3+y]==min){
					pos = x*3+y;
					return pos;
				}
			}
		}
		return pos;
	}

	int getMax(vector<double> block){
		double max;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				if(x==0 && y==0){
					max = block[x*3+y];
				}
				if(block[x*3+y]>max){
					max = block[x*3+y];
				}
			}
		}
		return max;
	}

	int getPositionMax(vector<double> block){
		double max = getMax(block);
		int pos = 0;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				if(block[x*3+y]==max){
					pos = x*3+y;
					return pos;
				}
			}
		}
		return pos;
	}

	int getMed(vector<double> block){
		double med = 0.0;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				med+= block[x*3+y];
			}
		}
		med /= 9;
		return med;
	}

	int getPositionMed(vector<double> block){
		double med = getMed(block);
		int dif, pos;
		for(int x = 0; x < 3; x+=1){
			for(int y = 0; y < 3; y+=1){
				if(x==0 && y==0){
					pos = 0;
					dif = abs(med-block[0]);
				}
				if(med==block[x*3+y]){
					pos = x*3+y;
					return pos;
				}
				if(abs(med-block[x*3+y])<dif){
					pos = x*3+y;
				}
			}
		}
		return pos;
	}


	
};

#endif