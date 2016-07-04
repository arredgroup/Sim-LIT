#include <string>
#include <cmath>

using namespace std;

	typedef struct tuple{
		int bits;
		string bin;
	}TUP;

	TUP intToBinC1(int num){
		int comp = 0;
		if(num<0)
			comp = 1;
		int bits = 0;
		string bin = "";
		do{
			if(num%2!=0){
				if(comp==0)
					bin = "1" + bin;
				else
					bin = "0" + bin;
			}else{
				if(comp==0)
					bin = "0" + bin;
				else
					bin = "1" + bin;
			}
			num=num/2;
			bits+=1;
		}while(num!=0);
		TUP tuple;
		tuple.bits=bits;
		tuple.bin = bin;
		return tuple;
	}

	int binC1ToInt(TUP C1){
		string num = C1.bin;
		int bits = C1.bits;
	    int result = 0, pot = 1;
	    for ( int i = num.length() - 1; i >= 0; --i, pot <<= 1 )
	        result += (num[i] - '0') * pot;
	    if(result>pow(2,bits-1))
	    	return result;
	    for (int i = 0;i < num.length();i+=1){
	    	if(num[i]=='0')
	    		num[i]='1';
	    	else
	    		num[i]='0';
	    }
	    result = 0, pot = 1;
	    for ( int i = num.length() - 1; i >= 0; --i, pot <<= 1 )
	        result += (num[i] - '0') * pot;
	    return -result;
	}

