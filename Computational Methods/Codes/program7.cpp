#include<stdio.h>
#include<stdlib.h>
#include<math.h>

double a = 0.0;
double b = 8.0;
double accurate = 1.145500033808614;

double f_x(double x)
{
	return sin(x);
}

double Composite_trapezoid(int n)
{
	double h = (b-a)/n;
	double result = 0.0;
	result = (f_x(a) + f_x(b))/2;
	int i = 1;
	while(i < n){
		result += f_x(a + i*h);
		i++;
	}
	result *= h;
	return result;
}

double Composite_simpson(int n)
{
	double h = (b-a)/n;
	double result = 0.0;
	result = (f_x(a) + f_x(b));
	int i = 1;
	while(i < n){
		if(i % 2 == 0){
			result += 2*f_x(a + i*h);
		}
		else{
			result += 4*f_x(a + i*h);
		}
		i++;
	}
	result *= h;
	result /= 3;
	return result;
}

int main()
{
	int k = 0;
	double old_error = 0.0;
	while(k <= 10){
		double result = Composite_trapezoid(int(pow(2,k)));
		double error = (result - accurate) > 0 ? (result - accurate) : -(result - accurate);
		//printf("%.10e, %.10e, %.10e\n", result, accurate, error);
		double order = 0.0;
		if(k == 0){
			old_error = error;
		}
		order = -(log(error/old_error))/log(2.0);
		printf("%d: %.10e error: %.10e order: %lf\n",k, result, error, order);
		old_error = error;
		k++;
	}
	printf("\n");
	k = 0;
	old_error = 0.0;
	while(k <= 10){
		double result = Composite_simpson(int(pow(2,k)));
		double error = (result - accurate) > 0 ? (result - accurate) : -(result - accurate);
		//printf("%.10e, %.10e, %.10e\n", result, accurate, error);
		double order = 0.0;
		if(k == 0){
			old_error = error;
		}
		order = -(log(error/old_error))/log(2.0);
		printf("%d: %.10e error: %.12e order: %lf\n",k, result, error, order);
		old_error = error;
		k++;
	}
	return 0;
}
