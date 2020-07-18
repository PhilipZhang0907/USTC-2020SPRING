#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#define pi 3.1415926535897

struct Node{
	double x;
	double f_x;
}node;

Node Values[20];

// original function f of x
double function_f(double x)
{
	double f_x = 1.0/(2+2*x+pow(x, 2));
	return f_x;
}

void Fresh()
{
	int i = 0;
	while(i < 20)
	{
		Values[i].x = 0.0;
		Values[i].f_x = 0.0;
		i++;
	}
	return;
}

void Calculate_Nodes_1(int n)
{
	int i = 0;
	while(i <= n)
	{
		Values[i].x = -5.0 + 10.0/n*i;
		Values[i].f_x = function_f(Values[i].x);
		i++;
	}
	return;
}

void Calculate_Nodes_2(int n)
{
	int i = 0;
	while(i <= n)
	{
		Values[i].x = -5.0 * cos((((double)(2*i+1))/(2*n+2))*pi);
		Values[i].f_x = function_f(Values[i].x);
		i++;
	}
	return;
}

double Lagrange(int n, double x)
{
	int i = 0;
	double result = 0.0;
	while(i <= n)
	{
		double tmpUp = 1.0;
		double tmpDown = 1.0;
		int j = 0;
		while(j <= n)
		{
			if(j == i)
			{
				j++;
				continue;
			}
			else
			{
				tmpUp *= (x - Values[j].x);
				tmpDown *= (Values[i].x - Values[j].x);
			}
			j++;
		}
		result += Values[i].f_x * (tmpUp/tmpDown);
		i++;
	}
	return result;
}


double test_for_error(int n, int mode)
{
	Fresh();
	int i = 0;
	double max = 0.0;
	int flag = 0;
	if(mode == 1)
	{
		Calculate_Nodes_1(n);
		while(i <= 500)
		{
			double y = i/50.0 - 5;
			double error = (function_f(y) - Lagrange(n, y));
			if(error < 0)
			{
				error *= -1;
			}
			if(max <= error)
			{
				max = error;
				flag = i;
			}
			i++;
		}
	}
	else if(mode == 2)
	{
		Calculate_Nodes_2(n);
		while(i <= 500)
		{
			double y = i/50.0 - 5;
			double error = (function_f(y) - Lagrange(n, y));
			if(error < 0)
			{
				error *= -1;
			}
			if(max <= error)
			{
				max = error;
				flag = i;
			}
			i++;
		}
	}
	printf("Group: %d , n = %d,\terror: %.12e , at point y = %.12e\n", mode, n, max, flag/50.0-5);
}

int main()
{
	test_for_error(4, 1);
	test_for_error(8, 1);
	test_for_error(16, 1);
	test_for_error(4, 2);
	test_for_error(8, 2);
	test_for_error(16, 2);
	return 0;
} 
