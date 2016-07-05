#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream> 
#include <sstream>

bool isNumber(char x){
	if(x=='0')
		return true;
	if(x=='1')
		return true;
	if(x=='2')
		return true;
	if(x=='3')
		return true;
	if(x=='4')
		return true;
	if(x=='5')
		return true;
	if(x=='6')
		return true;
	if(x=='7')
		return true;
	if(x=='8')
		return true;
	if(x=='9')
		return true;
	return false;
}

bool exists_file (const std::string& name) {
    ifstream f(name.c_str());
    if (f.good()) {
        f.close();
        return true;
    } else {
        f.close();
        return false;
    }   
}

bool readArguments(int n, char const *args[]){
	int i=1;
	string argument;
	header.hb=1;
	header.wb=1;
	while(i<n){
		argument=args[i];
		if(!argument.compare(IMAGE_NAME)){
			if((i+1)<n && args[i+1][0]!='-'){
				src = args[i+1];
				i+=2;
			}
			else{
				cout << "Image Name was not defined\n " ;
			    return false;
			}
		}
		if(!argument.compare(FORWARD_METHOD)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				i=i+1;
				int amount_forward_processing = 0;
				if(isNumber(args[i][0]))
					amount_forward_processing=atoi(args[i]);
				else{
					cout << "Number of Processing methods was not defined\n " ;
					return false;
				}
				int x=1;
				i=i+1;
				while(x<=amount_forward_processing && i<n && args[i][0]!='-'){
					int nArguments = -1;
					if(isNumber(args[i+1][0]))
						nArguments = atoi(args[i+1]);
					else{
						cout << "Number of Parameters de Processing methods was not defined\n " ;
						return false;
					}
					vector<void*> argv;
					for(int c=1;c<=nArguments;c++){
						if(args[i+1+c][0]!='-')
							argv.push_back((void*)(args[i+1+c]));
					}
					string fpname = args[i];
					ForwardProcessing* fpr = ForwardProcessingCreator::create(fpname);
					if(fpr!=NULL && (unsigned)nArguments==argv.size()){
						fp_name.push_back(fpname);
						fp.push_back(fpr);
						fp_arguments.push_back(argv);
						fp_narguments.push_back(nArguments);
					}
					x=x+1;
					i=i+argv.size()+2;
				}
			}
			
			else{
				cout << "Forward Processing Methods was not defined\n " ;
			    return false;
			}
		}
		if(!argument.compare(WIDTH_BLOCK)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				header.wb = atoi(args[i+1]);
				i+=2;
			}
			else{
				cout << "Block Width was not defined\n " ;
			    return false;
			}
		}
		if(!argument.compare(HEIGHT_BLOCK)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				header.hb = atoi(args[i+1]);
				i+=2;
			}
			else{
				cout << "Block Height was not defined\n " ;
			    return false;
			}
		}
		if(!argument.compare(PACKETIZER_METHOD)){
			if((i+2)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				package_size = atoi(args[i+1]);
				i=i+1;
				string pname = args[i+1];
				pktz = PacketizerCreator::create(pname);
				if(args[i+2][0]!='-'){
					pkgSrc=args[i+2];
					i++;
				}
				if(pktz==NULL)
					pktz = new NormalPacketizer();
				i+=2;
			}
			
			else{
				cout << "Packetizer Method was not defined\n " ;
			    return false;
			}
		}
		if(!argument.compare(CHANNEL_METHOD)){
			if((i+1)<n && args[i+1][0]!='-'){
				i=i+1;
				string fpname = args[i];
				if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
					i=i+1;
					int x = atoi(args[i]);
					int c=0;
					vector<string> argu;
					i=i+1;
					while(c<x && i<n && args[i][0]!='-'){
						argu.push_back(args[i+c]);
						c=c+1;
					}
					i=i+c;
					if(argu.size()==(unsigned)x){
						Channel* chn = ChannelCreator::create(fpname, argu);
						if(chn!=NULL){
							chnl=chn;
						}
						else{
							cout << "Channel Method was not defined\n " ;
						    return false;
						}
					}
				}
				else{
					cout << "Channel Method was not defined\n " ;
					return false;
				}
			}
			else{
				cout << "Channel Method was not defined\n " ;
				return false;
			}
		}
		if(!argument.compare(REBUILDER_METHOD)){
			if((i+1)<n && args[i+1][0]!='-'){
				string rname = args[i+1];
				rbd = RebuilderCreator::create(rname);
				if(rbd==NULL)
					rbd = new RebuilderAverage();
				i=i+2;
			}
			else{
				cout << "Rebuilder Method was not defined\n " ;
				return false;
			}
		}
		if(!argument.compare(METRIC)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				int amount_metrics = atoi(args[i+1]),c=1;
				i=i+1;
				string mname;
				for(c=1;c<=amount_metrics && i<n && args[i][0]!='-';c++){
					mname=args[i+c];
					Metric *aux = MetricCreator::create(mname);
					if(aux!=NULL)
						metrics.push_back(aux);
				}
				i=i+c;
			}
			else{
				cout << "Metric was not defined\n " ;
				return false;
			}
		}
		if(!argument.compare(FOLDER_RESULT)){
			if((i+1)<n && args[i+1][0]!='-'){
				header.folder = args[i+1];
				if(system(("mkdir -p "+header.folder).c_str()))
					;
				i=i+2;
			}
			else{
				cout << "Results folder was not defined\n " ;
				return false;
			}
		}
		if(!argument.compare(SHOW_DATA_SCREEN)){
			show_data=true;
			i=i+1;
		}
		if(!argument.compare(EXPORT_IMAGES)){
				export_images=true;
				i=i+1;
		} 
		if(!argument.compare(EXPORT_RESULT)){
			if((i+1)<n && args[i+1][0]!='-'){
				export_files=true;
				result_name = args[i+1];
				i=i+2;
			}
			else{
				cout << "Results name file was not defined\n " ;
				return false;
			}
		} 
	}
	if(header.folder.size()==0)
		header.folder="results";
	if(pktz==NULL)
		pktz = new NormalPacketizer();
	if(rbd==NULL)
		rbd = new RebuilderAverage();
	if(chnl==NULL){
		vector<string> str;
		str.push_back("./files/loss_random.loss");
		chnl = new ChannelFile(str);
	}
	return true;
}

template<typename T> void print_file(ofstream *file, T t, const int& width){
    *file << left << setw(width) << setfill(' ') << t;
}