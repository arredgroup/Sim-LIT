#include <string>
#include <iostream>

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

void readArguments(int n, char const *args[]){
	int i=1;
	string argument;
	while(i<n){
		argument=args[i];
		if(!argument.compare(IMAGE_NAME)){
			if((i+1)<n && args[i+1][0]!='-'){
				src = args[i+1];
				i+=2;
			}
			else
				i=i+1;
		}
/*		if(!argument.compare(WORK_DIR)){
			if((i+1)<n && args[i+1][0]!='-'){
				folder = args[i+1];
				i+=2;
			}
			else
				i=i+1;
		}*/
		if(!argument.compare(FORWARD_METHOD)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				i=i+1;
				int amount_forward_processing=atoi(args[i]);
				int x=1;
				i=i+1;
				while(x<=amount_forward_processing && i<n && args[i][0]!='-'){
					int nArguments = -1;
					if(isNumber(args[i+1][0]))
						nArguments = atoi(args[i+1]);
					vector<int> argv;
					for(int c=1;c<=nArguments;c++){
						if(isNumber(args[i+1+c][0]))
							argv.push_back(atoi(args[i+1+c]));
					}
					string fpname = args[i];
					ForwardProcessing* fpr = ForwardProcessingCreator::create(fpname);
					if(fpr!=NULL && nArguments==argv.size()){
						fp_name.push_back(fpname);
						fp.push_back(fpr);
						fp_arguments.push_back(argv);
						fp_narguments.push_back(nArguments);
					}
					x=x+1;
					i=i+argv.size()+2;
				}
			}
			else
				i=i+1;
		}
		if(!argument.compare(WIDTH_BLOCK)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				wblock = atoi(args[i+1]);
				i+=2;
			}
			else
				i=i+1;
		}
		if(!argument.compare(HEIGHT_BLOCK)){
			if((i+1)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				hblock = atoi(args[i+1]);
				i+=2;
			}
			else
				i=i+1;
		}
		if(!argument.compare(PACKETIZER_METHOD)){
			if((i+2)<n && args[i+1][0]!='-' && isNumber(args[i+1][0])){
				package_size = atoi(args[i+1]);
				i=i+1;
				string pname = args[i+1];
				pktz = PacketizerCreator::create(pname);
				if(pktz==NULL)
					pktz = new NormalPacketizer();
				i+=2;
			}
			else{
				pktz = new NormalPacketizer();
				i=i+1;
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
					if(argu.size()==x){
						Channel* chn = ChannelCreator::create(fpname, argu);
						if(chn!=NULL){
							chnl=chn;
						}
						else{
							vector<string> str;
							str.push_back("loss_random.loss");
							chnl = new ChannelFile(str);
						}
					}
				}
				else{
					vector<string> str;
					str.push_back("loss_random.loss");
					chnl = new ChannelFile(str);
					i=i+1;
				}
			}
			else{
				vector<string> str;
				str.push_back("loss_random.loss");
				chnl = new ChannelFile(str);
				i=i+1;
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
				rbd = new RebuilderAverage();
				i=i+1;
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
				metrics.push_back(new MetricPsnr());
				i=i+1;
			}
		}
		if(!argument.compare(FOLDER_RESULT)){
			if((i+1)<n && args[i+1][0]!='-'){
				folder_result = args[i+1];
				i=i+2;
			}
			else
				i=i+1;
		}
		if(!argument.compare(SHOW_DATA_SCREEN)){
			show_data=true;
			i=i+1;
		}
		if(!argument.compare(EXPORT_IMAGES)){
			export_images=true;
			i=i+1;
		}
		if(!argument.compare(EXPORT_FILES)){
			if((i+1)<n && args[i+1][0]!='-'){
				export_files=true;
				result_name = args[i+1];
				i=i+2;
			}
			else
				i=i+1;
		} 
	}
	if(pktz==NULL)
		pktz = new NormalPacketizer();
	if(rbd==NULL)
		rbd = new RebuilderAverage();
	if(chnl==NULL){
		vector<string> str;
		str.push_back("loss_random.loss");
		chnl = new ChannelFile(str);
	}
}
