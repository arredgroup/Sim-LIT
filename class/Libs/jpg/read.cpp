
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdio>
#include <algorithm>
#include <fstream> 
#include <sstream>
#include <math.h>

#include "stb_image.c"
#include "jpge.cpp"
#include "jpgd.cpp"

char image_size(uint8* buffer, int *width, int *height)
{
  unsigned long i;

  i = 3;
  while ((buffer[i] != 0xFF || buffer[i + 1] != 0xC0) && i < 5000)
  {
    if (buffer[i] == 0xFF)
      i += (buffer[i + 2] << 8) + buffer[i + 3] + 1;
    ++i;
  }
  if (buffer[i] != 0xFF)
    return 0;
  *height = (buffer[i + 5] << 8) + buffer[i + 6];
  *width = (buffer[i + 7] << 8) + buffer[i + 8];
  return 1;
}

int main(int argc, char const *argv[])
{
	jpge::jpeg_encoder enc;
	int x=0, y=0, i=0, j=0, comp=0, req_comp=3;
	unsigned char *img = stbi_load("lena.bmp", &x, &y, &comp, req_comp);
	jpge::params params;
	unsigned int subsampling = jpge::Y_ONLY;
    params.m_quality = 90;
    params.m_subsampling = (subsampling < 0) ? ((comp == 1) ? jpge::Y_ONLY : jpge::H2V2) : static_cast<jpge::subsampling_t>(subsampling);


	int buf_size = x * y * 3; // allocate a buffer that's hopefully big enough (this is way overkill for jpeg)
        if (buf_size < 1024) {
            buf_size = 1024;
        }
        void *pBuf = malloc(buf_size);

	jpge::memory_stream dst_stream(pBuf, buf_size);

	if(!enc.init(&dst_stream, x, y, params))
		return 1;
	if(!enc.read_image(img, x, y, req_comp))
		return 1;
	if (!enc.compress_image()) {
        return 1;
    }
    uint8* aux = dst_stream.get_buffer();
	image_size(aux, &i, &j);
	std::cout << "SIZE: " << i << "-" << j << "\n";

	//buf_size = *dst_stream.get_buffer();


    //for (int i = 0; i < uncomp_height*uncomp_width*3; i+=3){
    	//std::cout << static_cast<unsigned>(*(pUncomp_image_data+i)) << "-" << static_cast<unsigned>(*(pUncomp_image_data+i+1)) << "-"<< static_cast<unsigned>(*(pUncomp_image_data+i+2))<< "\n";
    //}
	int flag, dth=0;
	for (i = 0; i < dst_stream.get_size(); i+=1)
	{	
		flag=0;
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOI && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "SOI\n\n";
			i+=1;
			flag=1;
		}
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOF0 && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "SOF\n\n";
			i+=1;
			flag=1;
		}
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_DHT && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "DHT\n\n";
			i+=1;
			flag=1;
		}
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_DQT && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "DQT\n\n";
			i+=1;
			flag=1;
		}
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_SOS && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "SOS\n\n";
			dth=i+1;
			i+=1;
			flag=1;
		}
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_APP0 && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "APP\n\n";
			i+=1;
			flag=1;
		}
		if(i+1 < dst_stream.get_size() && static_cast<unsigned>(*(aux+i+1))==jpge::M_EOI && static_cast<unsigned>(*(aux+i))==0xFF){
			std::cout << "EOI\n\n";
			i+=1;
			flag=1;
		}
		if(flag==0){
			std::cout << static_cast<unsigned>(*(aux+i)) << ":";
		}
	}


    int uncomp_width = 0, uncomp_height = 0, uncomp_actual_comps = 0, uncomp_req_comps = 3;

    uint8 *pUncomp_image_data;

    pUncomp_image_data = jpgd::decompress_jpeg_image_from_memory((const stbi_uc *)pBuf, buf_size, &uncomp_width, &uncomp_height, &uncomp_actual_comps, uncomp_req_comps);
    	
    	

    std::cout << "LISTA: " << dst_stream.get_size() <<"\n";
	std::cout << "TAMAÑO: " << buf_size <<"\n";


	return 0;
}