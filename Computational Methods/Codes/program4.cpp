#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double epsilon = 1.0e-9;

double f_x(double x)
{
	double result;
	result = 2*pow(x, 4.0) + 24*pow(x, 3.0) + 61*pow(x, 2.0) - 16*x + 1.0;
	return result;
}

double f_prime_x(double x)
{
	double result;
	result = 8*pow(x, 3.0) + 72*pow(x, 2.0) + 122*x - 16;
	return result;
}

double Newton(double x)
{
	printf("NEWTON: %.10e\t%.10e\n", x, f_x(x));
	double y = f_x(x) >= 0 ? f_x(x) : -1*f_x(x);
	int k = 0;
	while(y >= epsilon)
	{
		printf("%d: %.10e\t%.10e\n",k, x, f_x(x));
		x = x - f_x(x)/f_prime_x(x);
		y = f_x(x) >= 0 ? f_x(x) : -1*f_x(x);
		k++;
	}
	printf("%d: %.10e\t%.10e\n",k, x, f_x(x));
}

double Secant(double x0, double x1)
{
	printf("SECANT: %.10e\t%.10e\n", x0, f_x(x0));
	double y = f_x(x1) >= 0 ? f_x(x1) : -1*f_x(x1);
	int k = 0;
	while(y >= epsilon)
	{
		printf("%d: %.10e\t%.10e\n", k, x0, f_x(x0));
		double x2 = x1 - f_x(x1)*((x1 - x0)/(f_x(x1) - f_x(x0)));
		x0 = x1;
		x1 = x2;
		y = f_x(x1) >= 0 ? f_x(x1) : -1*f_x(x1);
		k++;
	}
	printf("%d: %.10e\t%.10e\n", k, x0, f_x(x0));
	printf("%d: %.10e\t%.10e\n", k+1, x1, f_x(x1));
}

int main()
{
	Newton(0.0);
	Newton(3.0);
	Secant(0.0, 0.5);
	Secant(0.1, 1.5);
	return 0;
}                                                                                                                                                                                                                        
