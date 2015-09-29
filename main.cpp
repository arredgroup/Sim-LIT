/**
  @file main.cpp
  @brief Implementación Programa Principal de Sim-LIT 2.0
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include "header.h"

/*-----------------------------------------------*/
/*-----------VARIABLES DEL SISTEMA---------------*/
/* Estas variables son las que hacen que el software funcione por defecto*/
string src="lena.jpg";
string folder_result="";
string folder_images="";
string result_name="result.txt";
bool show_data=false;
bool export_files=false;
bool export_images=false;
int wblock=1, hblock=1, package_size=27;
vector<ForwardProcessing *> fp;
vector<int> fp_narguments;
vector<string> fp_name;
vector<vector<int> > fp_arguments;
Packetizer* pktz=NULL;
Rebuilder* rbd=NULL;
Channel* chnl=NULL;
vector<Metric *> metrics;
ofstream file;
/*------------------------------------------------*/
double npkgo, npkgf, nepkgo, nepkgf;

#include "functions.h"

using namespace std;

	int main(int argc, char const *argv[])
	{
		readArguments(argc,argv);
		if(export_files){
			file.open(result_name.c_str(),std::fstream::in | std::fstream::out | std::fstream::app);
			file << src << "\t";
		}
		if(show_data)
			cout << "Arguments readed\n";
		Images img(src,wblock,hblock,show_data);
		vector<DataType *> list = img.toList();
		int nwblock=(int)(img.getWidth()/img.getWidthBlock());
		int nhblock=(int)(img.getHeight()/img.getHeightBlock());
		for (int i = 0; i < fp.size(); i=i+1)
		{
			int narg = fp_narguments[i];
			vector<int> args = fp_arguments[i];
			fp[i]->make(&list,nwblock,nhblock,args,narg,show_data);
		}	
		//if((wblock*hblock)*img.getType()>package_size){
		//	cout << "The Package Size is minor than the block sizes\n";
		//	return 0;
		//}

		if(export_images){
			Images aux(pkgs_rcved,img.getType(),img.getWidth(),img.getHeight(),img.getWidthBlock(),img.getHeightBlock(),show_data);
			string path = folder_images+"image_forwared.bmp";
			aux.save(path.c_str());
		}

		vector<Package> pkgs = pktz->imgToPackageList(list,package_size,show_data);
		npkgo = pkgs.size();
		nepkgo = pktz->getSizeElements();
		vector<Package> rcved = chnl->simulate(pkgs);
		npkgf = rcved.size();

		if(rcved.size()==0){
			cout << "Not exists packages received, end \n";
			return 0;
		}
		vector<DataType *> pkgs_rcved = pktz->packageListToImg(rcved,show_data);
		nepkgf = pktz->getSizeElements();

		if(export_files){
			file << "\% Pkg-Lost: " << 100-((npkgf*100)/npkgo) << "\t";
			file << "\% Block-Lost: " << 100-((nepkgf*100)/nepkgo) << "\t";
		}

		if(export_images){
			Images aux(pkgs_rcved,img.getType(),img.getWidth(),img.getHeight(),img.getWidthBlock(),img.getHeightBlock(),show_data);
			string path = folder_images+"image_received_forwared.bmp";
			aux.save(path.c_str());
		}
		for (int i = fp.size()-1; i>=0; i=i-1)
		{
			fp[i]->unmake(&pkgs_rcved,show_data);
		}
		Images received(pkgs_rcved,img.getType(),img.getWidth(),img.getHeight(),img.getWidthBlock(),img.getHeightBlock(),show_data);
		if(export_images){
			string path = folder_images+"image_received_unforwared.bmp";
			received.save(path.c_str());
		}
		rbd->hidden(&received,show_data);
		if(export_images){
			string path = folder_images+"image_received_rebuilded.bmp";
			received.save(path.c_str());
		}
		for (int i = 0; i < metrics.size(); i=i+1)
		{
			double met = metrics.at(i)->calculate(img,received,show_data);
			if(export_files){
				file << metrics.at(i)->getName() << ": " << met << "\t";
			}
		}
		if(export_files){
			file << "\n";
		} 
		return 0;
	}
	
