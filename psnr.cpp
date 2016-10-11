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

HEAD header_org;
HEAD header_rcv;
Metric *metric;

using namespace std;

	int main(int argc, char const *argv[])
	{

		header_org.hb=1;
		header_org.wb=1;
		header_rcv.wb=1;
		header_rcv.hb=1;

		Images original("images/i.bmp",&header_org,true);
		Images rcv("images/k.bmp", &header_rcv,true);
		metric = new MetricPsnr();
		double val = metric->calculate(original,rcv,true,&header_org);
		cout << "PSNR: " << val << "\n";
		return 0;
	}
