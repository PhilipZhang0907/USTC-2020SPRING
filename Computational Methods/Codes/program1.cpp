#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

float inputNum[10];

void Initial(){
	float tmp = 1.0/8.0;
	int i = 0;
	while(i < 10){
		inputNum[i] = tmp;
		tmp *= 1.0/8.0;
		i++;
	}
	return;
}

float function_f(float x)
{
	float result = sqrt(pow(x, 2) + 9) - 3;
	return result;
}

float function_g(float x)
{
	float result = pow(x, 2)/(sqrt(pow(x, 2) + 9) + 3);
	return result;
}

void outputFloat(float f)
{
	if(f != 0)
	{
		char str[21];
		str[0] = '0';
		sprintf(str+1, "%.11e", f);
		str[2] = str[1];
		str[1] = '.';
		int len = strlen(str) - 1;
		if(str[len] != '-')
		{
			str[len]--;
			while(str[len] < '0')
			{
				str[len--] = '9';
				if(str[len] == '-')
				{
					break;
				}
				str[len]--;
			}
		}
		printf("%s\t", str);
	}
	else
	{
		printf("0.000000000000e-000\t");
	}
}

int main(){
	int i = 0;
	Initial();
	while(i < 10){
		float x = inputNum[i];
		printf("x:");
		outputFloat(x);
		double fResult = function_f(x);
		printf("f(x):");
		outputFloat(fResult);
		double gResult = function_g(x);
		printf("g(x):");
		outputFloat(gResult);
		printf("\n");
		i++;
	}
	return 0;
}
