/**
  @file main.cpp
  @brief Implementación Programa Principal de Sim-LIT 2.0
  @author Christopher Arredondo Flores
  @date 28/9/2014

**/
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include "header.h"
#include "structs.h"
#include <strings.h>



/*-----------------------------------------------*/
/*-----------VARIABLES DEL SISTEMA---------------*/
/* Estas variables son las que hacen que el software funcione por defecto*/
string src="src/1.bmp";
string result_name="result.txt";
string pkgSrc="";
bool show_data=false;
bool export_files=false;
bool export_images=false;
int wblock=1, hblock=1, package_size=27;
vector<ForwardProcessing *> fp;
vector<int> fp_narguments;
vector<string> fp_name;
vector<vector<void *> > fp_arguments;
Packetizer* pktz=NULL;
Rebuilder* rbd=NULL;
Channel* chnl=NULL;
vector<Metric *> metrics;
ofstream file;
HEAD header;
/*------------------------------------------------*/
double npkgo, npkgf, nepkgo, nepkgf;

#include "functions.h"

using namespace std;

	int main(int argc, char const *argv[])
	{

		if(!readArguments(argc,argv)){
			cout << "ERROR: Arguments are not valid!!\n";
			return 0;
		}
		
		if(export_files){
			if(exists_file((header.folder + "/" +result_name))){
				file.open((header.folder + "/" +result_name).c_str(),std::fstream::in | std::fstream::out | std::fstream::app);
				print_file(&file, src, 35);
			}
			else{
				file.open((header.folder + "/" +result_name).c_str(),std::fstream::in | std::fstream::out | std::fstream::app);
				print_file(&file, "Image Name", 35);
				print_file(&file, "\% Pkg-Lost", 15);
				print_file(&file, "\% Block-Lost", 15);
				print_file(&file, "Metric", 15); 
				print_file(&file, "\n", 3); 
				print_file(&file, src, 35);
			}
		}

		if(show_data)
			cout << "Arguments readed\n";
		Images img(src,&header,show_data);
		vector<DataType *> list = img.toList();
		for (int i = 0; (unsigned)i < fp.size(); i=i+1)
		{
			int narg = fp_narguments[i];
			vector<void *> args = fp_arguments[i];
			if(fp[i]->make(&list,&header,args,narg,show_data,export_images)!=0){
				cout << "Sim-LIT was ended with errors!\n";
				return 0;
			}
		}
		vector<Package> pkgs;
		if(pkgSrc.size()>0){
			pkgs = pktz->imgToPackageList(list,package_size,show_data, (void*)pkgSrc.c_str());
		}
		else{
			pkgs = pktz->imgToPackageList(list,package_size,show_data);
		}
		npkgo = pkgs.size(); //Número de Paquetes generados
		nepkgo = pktz->getSizeElements(); //Número de elementos paquetizados
		vector<Package> rcved = chnl->simulate(pkgs);
		npkgf = rcved.size(); //Número de Paquetes recibidos
		if(rcved.size()==0){
			cout << "Not exists packages received, end \n";
			return 0;
		}
		vector<DataType *> pkgs_rcved = pktz->packageListToImg(rcved,show_data,&header);
		nepkgf = pktz->getSizeElements(); //Número de elementos recibidos
/*
		if(export_images){
            Images aux(data,header,show_data);
            string path = header->folder+"/image_received_unforwared.bmp";
            aux.save(path.c_str());
        	aux.destroy();
        }
*/
		if(show_data){
			cout << "NPG: " << npkgo << " NPR: " << npkgf << "\nNEP: " << nepkgo << " NER: " << nepkgf << "\n";
		}
		if(export_files){
			print_file(&file, 100-((npkgf*100)/npkgo),15);
			print_file(&file, 100-((nepkgf*100)/nepkgo),15);
		}

		for (int i = fp.size()-1; i>=0; i=i-1)
		{ 
			if(fp[i]->unmake(&pkgs_rcved,&header,show_data,export_images)!=0){
				cout << "Sim-LIT was ended with errors!\n";
				return 0;
			}
		}
		Images received(pkgs_rcved,&header,show_data);
		if(export_images)
			received.save((header.folder + "/image_received_before_restored.bmp").c_str());
		rbd->hidden(&received,show_data,export_images,&header);
		for (int i = 0; (unsigned)i < metrics.size(); i=i+1)
		{
			double met = metrics.at(i)->calculate(img,received,show_data,&header);
			if(export_files){
				string str = metrics.at(i)->getName() + ": " + to_string(met);
				print_file(&file, str ,15);
			}
		}
		if(export_files){
			print_file(&file, "\n",3);
		} 
		return 0;
	}
	
