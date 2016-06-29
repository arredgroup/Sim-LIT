
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <fstream> 
#include <sstream>

#include "huffman.c"

using namespace std;

int main(int argc, char const *argv[])
{
	unsigned char demo[4] = {124,20,30,20};
	unsigned char* result = (unsigned char*)malloc(sizeof(unsigned char)*34);
	int size = 4;
	int tam = Huffman_Compress(demo, result, size);
	cout << "COMPRIMIÓ!!!\n";
	Huffman_Uncompress( result, demo, tam, size );
	cout << "DESCOMPRIMIO!!!\n";
	for (int i = 0; i < size; ++i)
	{
		cout << "EL:" << (float)demo[i] << "\n";
	}
	return 0;
}