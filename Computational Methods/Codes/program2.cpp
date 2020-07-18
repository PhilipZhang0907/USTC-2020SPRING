#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

double x1 = 4040.045551380452;
double x2 = -2759471.276702747;
double x3 = -31.64291531266504;
double x4 = 2755462.874010974;
double x5 = 0.0000557052996742893;

int main()
{
	double result = 0.0;
// calculation 1
	result += x1;
	result += x2;
	result += x3;
	result += x4;
	result += x5;
	printf("Method1:%.6e\t", result);
// calculation 2
	result = 0.0;
	result += x5;
	result += x4;
	result += x3;
	result += x2;
	result += x1;
	printf("Method2:%.6e\t", result);
//calculation 3
	double part1 = 0.0;
	part1 += x4;
	part1 += x1;
	part1 += x5;
	double part2 = 0.0;
	part2 += x2;
	part2 += x3;
	result = part1 + part2;
	printf("Method3:%.6e\n", result);
	printf("positive: %.12e\tnegative: %.12e\n",x4+x1+x5, x2+x3);
	return 0;
}
