
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include <cmath>

#include "huffman.c"

using namespace std;

int main(int argc, char const *argv[])
{
	unsigned char demo[5] = {124,-200,-200,-127,510};
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)*34);
	int size = 5;
	int tam = Huffman_Compress(demo, result, size);
	cout << "COMPRIMIÓ!!!\n";
	cout << tam << "\n";
	int cel = floor((double) 510 / 256);
	cout << "CEIL " << cel << "\n";
	Huffman_Uncompress( result, demo, tam, size );
	cout << "DESCOMPRIMIO!!!\n";
	for (int i = 0; i < size; ++i)
	{
		//if(i==4)
		//	cout << "EL:" << ((float)demo[i])+(cel*256) << "\n";
		//else
			cout << "EL:" << (float)demo[i] << "\n";
	}
	return 0;
}