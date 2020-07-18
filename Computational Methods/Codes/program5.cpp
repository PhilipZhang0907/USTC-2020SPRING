#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double epsilon = 1.0e-9;

double f_x(double x)
{
	double result;
	result = pow(x, 3.0) - 4*pow(x, 2.0) + 5*x - 2;
	return result;
}

double f_prime_x(double x)
{
	double result;
	result = 3*pow(x, 2.0) - 8*x + 5;
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

int main()
{
	Newton(0.0);
	return 0;
}                                                                                                                                                                                                                        
