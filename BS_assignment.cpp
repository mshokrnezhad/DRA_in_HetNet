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

double distance(double x_i, double x_j, double y_i, double y_j)
{
	return sqrt(pow((x_i - x_j), 2) + pow((y_i - y_j), 2));
}

double h(double x_i, double x_next_node_j, double y_i, double y_next_node_j)
{
	return 0.09*pow(distance(x_i, x_next_node_j, y_i, y_next_node_j), -3);
}

int random_generator(int min, int max)
{
	int random_number;
	//srand(time(NULL));
	random_number = rand() % (1000 - 0) + 0;
	for (int i = 0; i<max - min + 1; i++)
	if (random_number >= ((1000 * i) / (max - min + 1)) && random_number <= ((1000 * (i + 1)) / (max - min + 1)))
		return i + min;
}

void print_progress_bar(int percent)
{
	string bar;
	cout << "\r" << bar;
	cout << percent << "% " << std::flush;
}

int func_next_node(int i, int n, int r, double x[], double y[])
{
	int temp_distance = 1000000;
	int n_n = 0;
	if (i == 0)
		return n_n;
	else
	{
		for (int j = 0; j < i; j++)
		if (i != j)
		if (distance(x[i], x[j], y[i], y[j]) <= r)
		if (distance(x[j], x[0], y[j], y[0]) <= temp_distance)
		{
			temp_distance = distance(x[j], x[0], y[j], y[0]);
			n_n = j;
		}
		return n_n;
	}
}

double func_get_max_index(double arr[], int size)
{
	int MaxIndex;
	double temp_max = 0;
	for (int i = 0; i<size; i++)
	if (arr[i]>temp_max)
	{
		temp_max = arr[i];
		MaxIndex = i;
	}

	return MaxIndex;
}

double func_get_min_index(double arr[], int size)
{
	int MinIndex;
	double temp_min = 1000000000000;
	for (int i = 0; i<size; i++)
	if (arr[i]<temp_min)
	{
		temp_min = arr[i];
		MinIndex = i;
	}

	return MinIndex;
}

double func_get_max(double arr[], int size)
{
	int MaxIndex;
	double temp_max = 0;
	for (int i = 0; i<size; i++)
	if (arr[i]>temp_max)
	{
		temp_max = arr[i];
		MaxIndex = i;
	}

	return temp_max;
}

double func_get_min(double arr[], int size)
{
	int MinIndex;
	double temp_min = 1000000000000;
	for (int i = 0; i<size; i++)
	if (arr[i]<temp_min)
	{
		temp_min = arr[i];
		MinIndex = i;
	}

	return temp_min;
}

double diff(double a, double b)
{
	if (a >= b)
		return a - b;
	else
		return b - a;
}

int main(int argc, char* argv[])
{
	int cm_NoN = atoi(argv[1]);
	int cm_NoS = atoi(argv[2]);
	double cm_femto_distance = atoi(argv[3]);

	const int NoN = cm_NoN;
	const int NoS = cm_NoS;
	double fd = cm_femto_distance;
	
	int next_node[NoN];
	double x_n[NoN];
	double y_n[NoN];
	double x_s[NoS];
	double y_s[NoS];

	int temp_a = 0;
	int temp_c = 0;
	double temp_b = 0;
	ifstream rxfile;
	rxfile.open("R01_x_n.txt");
	while (!rxfile.eof())
	{
		rxfile >> temp_a >> temp_b;
		x_n[temp_a] = temp_b;
	}
	rxfile.close();

	temp_a = 0;
	temp_b = 0;
	ifstream ryfile;
	ryfile.open("R01_y_n.txt");
	while (!ryfile.eof())
	{
		ryfile >> temp_a >> temp_b;
		y_n[temp_a] = temp_b;
	}
	ryfile.close();

	x_s[0] = 5000;
	y_s[0] = 5000;

	/*x_s[1] = 5003;
	y_s[1] = 5000;

	x_s[2] = 4997;
	y_s[2] = 5000;

	x_s[3] = 5000;
	y_s[3] = 5003;

	x_s[4] = 5000;
	y_s[4] = 4997;*/

	int start_s = clock();

	for (int i = 0; i < NoN; i++)
		next_node[i] = 0;

	for (int i = 0; i < NoN; i++)
		for (int s = 0; s < NoS; s++)
			if (abs(x_n[i]-x_s[s])<= fd && abs(y_n[i]-y_s[s])<= fd)
				next_node[i] = s;

	ofstream file_NN;
	file_NN.open("R02_NN.txt", std::ios::app);
	for (int i = 0; i < NoN; i++)
		file_NN << i << " " << next_node[i] << "\n";
	file_NN.close();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}
