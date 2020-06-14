// iot_pc_cha_d2a.cpp : Defines the entry point for the console application.
//
#include "iostream"
#include "fstream"
#include "iomanip"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
#include "math.h"
#include "vector"
#include "ctime"


using namespace std;

int main(int argc, char* argv[])
{
	int cm_n = atoi(argv[1]);
	const int n =cm_n; //number of nodes
	int seed = 1;

	double x[n];
	double y[n];
	double a;
	double b;

	int flag;
	for (int i = 0; i < n; i++)
	{
		flag=0;
		while(flag==0)
		{
			flag=1;
			srand(time(NULL) + seed * 15);
			a = rand() % 10;
			if(a>=5)
				x[i]=5000+a-5;
			if(a<5)
				x[i]=5000-a;
			srand(time(NULL) + seed * 27);
			b = rand() % 10;
			if(b>=5)
				y[i]=5000+b-5;
			if(b<5)
				y[i]=5000-b;
			for (int j = 0; j <i; j++)
				if(x[i]==x[j] && y[i]==y[j])
					flag=0;
		}
		seed++;
	}

	// preparing results for colormap picture /////////////////////////////////////////////////////////////////////////////////////////////

	ofstream file_LoN;
	file_LoN.open("R01_LoN.txt", std::ios::app);
	for (int i = 0; i < n; i++)
	{
		file_LoN<<"x["<<i<<"] = "<<x[i]<<";"<<"\n";
		file_LoN<<"y["<<i<<"] = "<<y[i]<<";"<<"\n";
	}
	file_LoN.close();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}
