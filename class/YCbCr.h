#ifndef CLASS_YCBCR
#define CLASS_YCBCR

#include "class/Pixel.h";

using namespace std;

class YCbCr{

private:
	float Y;
	float Cr;
	float Cb;

public:

	YCbCr(Pixel px){
      int* pxCnt = px.getContent();
      if(pxCnt[0]==1){
        Y = Cb = Cr = 0;
      }
      else{
        if(pxCnt[1]==3){
          Y  = (0.299 * pxCnt[2]) + (0.587 * pxCnt[3]) + (0.114 * pxCnt[4]) - 128.0
          Cb = (- 0.1687 * pxCnt[2]) - (0.3313 * pxCnt[3]) + (0.5 * pxCnt[4]) 
          Cr = (0.5 * pxCnt[2]) - (0.4187 * pxCnt[3]) - (0.0813 * pxCnt[4])
        } 
      }
    }

  YCbCr(int nY, int nCb, int nCr){
    Y = nY;
    Cb = nCb;
    Cr = nCr;

  }

	~YCbCr(){
	}

	Pixel getRGB(){

       int r = (int) (Y + 1.40200 * (Cr - 0x80));
       int g = (int) (Y - 0.34414 * (Cb - 0x80) - 0.71414 * (Cr - 0x80));
       int b = (int) (Y + 1.77200 * (Cb - 0x80));

       vector<int> nChannels = new vector<int>();

       nChannels.put(Max(0, Min(255, r)));
       nChannels.put(Max(0, Min(255, g)));
       nChannels.put(Max(0, Min(255, b)));

       return new Pixel(nChannels, 3);

	}

}