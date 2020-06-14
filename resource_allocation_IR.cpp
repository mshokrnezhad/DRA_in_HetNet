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
	int cm_no_specs = atoi(argv[3]);
	//double cm_target_sinr = atoi(argv[4]);
	//double cm_noise = atoi(argv[5]);
	//double cm_femto_distance = atoi(argv[6]);

	const int NoN = cm_NoN;
	const int NoS = cm_NoS;
	const int no_specs = cm_no_specs;
	double noise = 0.0000000001;
	double target_sinr = 0.3;
	double max_power = 2;
	double fd = 2;

	const int round_bound = NoN; //minimun amount is 1 where we have just one round
	const int iteration_bound = 200; //minimun amount is 1 where we have just one iteration

	double sinr[round_bound][NoN][no_specs];
	double t_sinr_n[NoN][no_specs];
	double t_sinr_l[NoN][no_specs];
	double sum_sinr[round_bound];
	int sinr_received_counter = 0;
	double ssir = 0;

	double p[round_bound][NoN][no_specs];
	double t_p_n[NoN][no_specs];
	double t_p_l[NoN][no_specs];
	double sum_power[round_bound];
	double spower = 0;

	double I[round_bound][NoN][no_specs];
	double t_I_n[NoN][no_specs];
	double t_I_l[NoN][no_specs];

	double outage_ratio[round_bound];
	double tc = 0;

	int max_powered_node;
	int min_interfered_channel;

	int round_loop_index_counter = 0;
	int round_loop_flag = 0;

	int next_node[NoN];
	int chflag[NoN];
	int round;
	int channel[round_bound][NoN];
	double temp[NoN + no_specs];
	int iteration;

	double max_sum_sinr;
	double min_sum_power;
	int round_max_sum_sinr;
	int round_min_sum_power;
	double min_outage_ratio;

	double x_n[NoN];
	double y_n[NoN];

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

	//////////////////////////////////////////////////////////////
	for (int i = 0; i < NoN; i++)
	if(y_n[i]==5000 && x_n[i]==5000)
		y_n[i]=5001;
	for (int i = 0; i < NoN; i++)
	if(y_n[i]==5000 && x_n[i]==5003)
		y_n[i]=5001;
	for (int i = 0; i < NoN; i++)
	if(y_n[i]==5000 && x_n[i]==4997)
		y_n[i]=5001;
	for (int i = 0; i < NoN; i++)
	if(y_n[i]==5003 && x_n[i]==5000)
		x_n[i]=5001;
	for (int i = 0; i < NoN; i++)
	if(y_n[i]==4997 && x_n[i]==5000)
		x_n[i]=5001;/**/
	//////////////////////////////////////////////////////////////


	double x_s[NoS];
	double y_s[NoS];

	x_s[0] = 5000;
	y_s[0] = 5000;

	x_s[1] = 5003;
	y_s[1] = 5000;

	x_s[2] = 4997;
	y_s[2] = 5000;

	x_s[3] = 5000;
	y_s[3] = 5003;

	x_s[4] = 5000;
	y_s[4] = 4997;/**/

	temp_a = 0;
	temp_c = 0;
	temp_b = 0;

	ifstream nnfile;
	nnfile.open("R02_NN.txt");
	while (!nnfile.eof())
	{
		nnfile >> temp_a >> temp_c;
		next_node[temp_a] = temp_c;
	}
	nnfile.close();

	int start_s = clock();

	for (int SN = 0; SN < NoS; SN++)
	{
		round = 0;
		if (SN == 0)
		for (int i = 0; i < NoN; i++)
		{
			channel[round][i] = 0;// temp_nu%no_specs;
			chflag[i] = 0;
		}

		//while (sinr_received_counter != no_trans && round<round_bound && round_loop_flag != 1)
		while (round < round_bound)
		{
			if (round > 0)
			{
				// finding max_interfered_node

				for (int i = 0; i < NoN + no_specs; i++)
					temp[i] = 0;

				for (int i = 0; i < NoN; i++)
				if (chflag[i] == 0 && next_node[i] == SN)
					temp[i] = p[round - 1][i][channel[round - 1][i]];
				else
					temp[i] = 0;
				max_powered_node = func_get_max_index(temp, NoN);

				// finding min_interfered_channel
				for (int k = 0; k < no_specs; k++)
					temp[k] = I[round - 1][max_powered_node][k];
				min_interfered_channel = func_get_min_index(temp, no_specs);

				for (int i = 0; i < NoN; i++)
				{
					// assigning channel and power to max_interfered_node
					if (i == max_powered_node)
						channel[round][i] = min_interfered_channel;

					// assigning channel and power to other nodes
					else
						channel[round][i] = channel[round - 1][i];
				}
			}

			iteration = 0;
			// power control procedure
			for (int i = 0; i < NoN; i++)
			if (next_node[i] == SN)
			{
				for (int k = 0; k < no_specs; k++)
					t_p_n[i][k] = 0;
				t_p_n[i][channel[round][i]] = max_power;
			}

			// power control itterations
			while (iteration < iteration_bound)
			{
				if (iteration != 0)
				for (int i = 0; i < NoN; i++)
				if (next_node[i] == SN)
				{
					for (int k = 0; k < no_specs; k++)
						t_p_n[i][k] = 0;
					t_p_n[i][channel[round][i]] = min(max_power, target_sinr*(t_p_l[i][channel[round][i]] / t_sinr_l[i][channel[round][i]]));
				}

				for (int i = 0; i < NoN; i++)
				if (next_node[i] == SN)
				for (int k = 0; k < no_specs; k++)
				{
					t_I_n[i][k] = 0;
					for (int j = 0; j < NoN; j++)
					if (j != i && next_node[j] == SN)
						t_I_n[i][k] = t_I_n[i][k] + t_p_n[j][k] * h(x_n[j], x_s[next_node[i]], y_n[j], y_s[next_node[i]]);
					t_I_n[i][k] = t_I_n[i][k] + noise;
					t_sinr_n[i][k] = (t_p_n[i][k] * h(x_n[i], x_s[next_node[i]], y_n[i], y_s[next_node[i]])) / t_I_n[i][k];
				}

				for (int i = 0; i < NoN; i++)
				if (next_node[i] == SN)
				for (int k = 0; k < no_specs; k++)
				{
					t_p_l[i][k] = t_p_n[i][k];
					t_I_l[i][k] = t_I_n[i][k];
					t_sinr_l[i][k] = t_sinr_n[i][k];
				}

				iteration++;
			}

			for (int i = 0; i < NoN; i++)
			{
				if (next_node[i] == SN)
				for (int k = 0; k < no_specs; k++)
				{
					p[round][i][k] = t_p_l[i][k];
					I[round][i][k] = t_I_l[i][k];
					sinr[round][i][k] = t_sinr_l[i][k];
				}
				else
				for (int k = 0; k < no_specs; k++)
				{
					p[round][i][k] = 0;
					I[round][i][k] = 0;
					sinr[round][i][k] = 0;
				}
			}

			// final values of each power control procedure
			sinr_received_counter = 0;
			sum_sinr[round] = 0;
			sum_power[round] = 0;
			for (int i = 0; i < NoN; i++)
			if (next_node[i] == SN)
			{
				if (sinr[round][i][channel[round][i]] > target_sinr - 0.001)
					sinr_received_counter++;

				for (int k = 0; k < no_specs; k++)
				{
					sum_sinr[round] = sum_sinr[round] + sinr[round][i][k];
					sum_power[round] = sum_power[round] + p[round][i][k];
				}
			}

			if (round > 0)
			{
				if (sum_power[round] > sum_power[round - 1])
				for (int i = 0; i < NoN; i++)
				if (next_node[i] == SN)
				{
					channel[round][i] = channel[round - 1][i];
					for (int k = 0; k < no_specs; k++)
					{
						I[round][i][k] = I[round - 1][i][k];
						p[round][i][k] = p[round - 1][i][k];
						sum_power[round] = sum_power[round - 1];
						sinr[round][i][k] = sinr[round - 1][i][k];
					}
				}
				else
					chflag[max_powered_node] = 1;
			}
			round++;
		}

		spower = spower + sum_power[round - 1];
		ssir = ssir + sum_sinr[round - 1];
		tc = tc + sinr_received_counter;
	}

	int stop_s = clock();

	// writing Sum of Power to file
	ofstream file_SoP;
	file_SoP.open("R03_SoP.txt", std::ios::app);
	file_SoP << spower << "\n";
	file_SoP.close();

	// writing Sum of SINR to file
	ofstream file_SoS;
	file_SoS.open("R03_SoS.txt", std::ios::app);
	file_SoS << ssir << "\n";
	file_SoS.close();

	// writingNumber of Satisfied Users to file
	ofstream file_et;
	file_et.open("R03_OR.txt", std::ios::app);
	file_et << tc << endl;
	file_et.close();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return 0;
}
