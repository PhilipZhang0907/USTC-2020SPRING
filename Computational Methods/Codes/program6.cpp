#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define epsilon 1.0E-5
#define size 10

double A[10][10] = {{2.0,-1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
				 	{-1.0,2.0,-1.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
				 	{0.0,-1.0,2.0,-1.0,0.0,0.0,0.0,0.0,0.0,0.0},
				 	{0.0,0.0,-1.0,2.0,-1.0,0.0,0.0,0.0,0.0,0.0},
				 	{0.0,0.0,0.0,-1.0,2.0,-1.0,0.0,0.0,0.0,0.0},
				 	{0.0,0.0,0.0,0.0,-1.0,2.0,-1.0,0.0,0.0,0.0},
				 	{0.0,0.0,0.0,0.0,0.0,-1.0,2.0,-1.0,0.0,0.0},
				 	{0.0,0.0,0.0,0.0,0.0,0.0,-1.0,2.0,-1.0,0.0},
				 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,-1.0,2.0,-1.0},
				 	{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,-1.0,2.0}};
double b[10] = {2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0,2.0};
double old_X[10] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
double new_X[10];

double infinite_norm()
{
	int i = 0;
	double sum = 0.0;
	while(i < 10){
		sum += ((new_X[i] - old_X[i]) > 0 ? (new_X[i] - old_X[i]) : -(new_X[i] - old_X[i]));
		i++;
	}
	return sum;
}

double Jacobi_iteration(int n)
{
	int i = 0;
	double result = 0.0;
	while(i < size){
		result += A[n][i] * old_X[i];
		i++;
	}
	result = -(result - b[n])/A[n][n] + old_X[n];
	return result;
}

double Gauss_iteration(int n)
{
	int i = 0;
	double result = 0.0;
	while(i < n){
		result += A[n][i] * new_X[i];
		i++;
	}
	i++;
	while(i < size){
		result += A[n][i] * old_X[i];
		i++;
	}
	result = -(result - b[n])/A[n][n];
	return result;
}

void Jacobi()
{
	int round = 1;
	int i = 0;
	while(i < size){
		new_X[i] = Jacobi_iteration(i);
		i++;
	}
	printf("Round %d:\n",round);
	i = 0;
	while(i < size){
		printf("%.10e\n", new_X[i]);
		i++;
	}
	printf("\n");
	while(infinite_norm() >= epsilon){
		i = 0;
		while(i < size){
			old_X[i] = new_X[i];
			i++;
		}
		i = 0;
		while(i < size){
			new_X[i] = Jacobi_iteration(i);
			i++;
		}
		round++;
		printf("Round %d:\n",round);
		i = 0;
		while(i < size){
			printf("%.10e\n", new_X[i]);
			i++;
		}
		printf("\n");
	}
}

void Gauss()
{
	int round = 1;
	int i = 0;
	while(i < size){
		new_X[i] = Gauss_iteration(i);
		i++;
	}
	printf("Round %d:\n",round);
	i = 0;
	while(i < size){
		printf("%.10e\n", new_X[i]);
		i++;
	}
	printf("\n");
	while(infinite_norm() >= epsilon){
		i = 0;
		while(i < size){
			old_X[i] = new_X[i];
			i++;
		}
		i = 0;
		while(i < size){
			new_X[i] = Gauss_iteration(i);
			i++;
		}
		round++;
		printf("Round %d:\n",round);
		i = 0;
		while(i < size){
			printf("%.10e\n", new_X[i]);
			i++;
		}
		printf("\n");
	}
}

int main()
{
	Gauss();
	return 0;
}
