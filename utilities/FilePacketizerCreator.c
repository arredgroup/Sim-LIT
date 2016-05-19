#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include <stdlib.h>

using namespace std;

bool exists(int x, vector<int> size, int epp){
	if(size[x]>=epp){
		return true;
	}
	return false;
}

void generateFile(int n, char const *v[]){
	int w=0,h=0,epp=0,np=0;
	ofstream save;
	save.open("../files/demo.pkg");
		string aux = v[1];
		w = atoi(aux.c_str()); //Ancho
		 aux = v[2];
		h = atoi(aux.c_str()); //Alto
		 aux = v[3];
		epp = atoi(aux.c_str()); //tamaño del paquete
		 aux = v[4];
		np = atoi(aux.c_str()); //número de paquetes
		vector<int> size(np,0);
		cout << h << w << epp << np;
	for (int i = 0; i < h; ++i)
	{
		for (int j = 0; j < w; ++j)
		{
			int auxI;
			do{
				 auxI = (rand()%np);
			}while(exists(auxI, size, epp));
			if(i!=0 || j!=0)
				save << "\n";
			save << ((i*w)+j) << "-" << auxI;
			size[auxI]++;
		}
	}
	save.close();
}


int main(int argc, char const *argv[])
{
	generateFile(argc, argv);
	return 0;
}
