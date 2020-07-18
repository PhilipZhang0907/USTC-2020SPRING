#include<stdio.h>
#include<stdlib.h>
#include<math.h>

struct Domin{
	int domin[10];
	int remain;
};

Domin forward_check_trace[9][9];
char sudoku_buffer[256];
char sudoku_store[9][9];
char sudoku_check[10];				// set sudoku_check[i] = 1 if i appears
char standard_region[9][9] = {{0,0,0,1,1,1,2,2,2},
							  {0,0,0,1,1,1,2,2,2},
							  {0,0,0,1,1,1,2,2,2},
							  {3,3,3,4,4,4,5,5,5},
							  {3,3,3,4,4,4,5,5,5},
							  {3,3,3,4,4,4,5,5,5},
							  {6,6,6,7,7,7,8,8,8},
							  {6,6,6,7,7,7,8,8,8},
							  {6,6,6,7,7,7,8,8,8}
							  };
int value_set[9] = {1,2,3,4,5,6,7,8,9};
int NEXT_ROW = 0;
int NEXT_COLUMN = 0;
int done = 0;						// when done = 1, stop recall and return
int nodeNum = 0;
FILE *debugfile;
#define FILE_PATH "../input/sudoku01.txt"

int forward_check(int row, int column)
{
	int check_buffer_domin[10];
	int i = 1;
	while(i < 10){
		check_buffer_domin[i] = forward_check_trace[row][column].domin[i];
		i++;
	}
	int check_buffer_remain = forward_check_trace[row][column].remain;
	// store original value for forward_check failure
	int value = sudoku_store[row][column];
	forward_check_trace[row][column].remain = 1;
	forward_check_trace[row][column].domin[value] = 1;
	i = 1;
	while(i <= 9){
		if(i != value){
			forward_check_trace[row][column].domin[i] = 0;
		}
		i++;
	}
	// set value
	i = 0;
	while(i < 9){
		if(i != row){
			if(forward_check_trace[i][column].domin[value] == 1){
				forward_check_trace[i][column].domin[value] = 0;
				forward_check_trace[i][column].remain--;
				if(forward_check_trace[i][column].remain == 0){
					return 0;
				}
			}
		}
		i++;
	}
	i = 0;
	while(i < 9){
		if(i != column){
			if(forward_check_trace[row][i].domin[value] == 1){
				forward_check_trace[row][i].domin[value] = 0;
				forward_check_trace[row][i].remain--;
				if(forward_check_trace[row][i].remain == 0){
					return 0;
				}
			}
		}
		i++;
	}
	int tem_row = (standard_region[row][column] / 3) * 3;
	int tem_column = (standard_region[row][column] % 3) * 3;
	i = 0;
	while(i < 3){
		int j = 0;
		while(j < 3){
			if(tem_row+i != row && tem_column+j != column){
				if(forward_check_trace[tem_row+i][tem_column+j].domin[value] == 1){
					forward_check_trace[tem_row+i][tem_column+j].domin[value] = 0;
					forward_check_trace[tem_row+i][tem_column+j].remain--;
					if(forward_check_trace[tem_row+i][tem_column+j].remain == 0){
						return 0;
					}
				}
			}
			j++;
		}
		i++;
	}
	if(row == column){
		i = 0;
		while(i < 9){
			if(i != row){
				if(forward_check_trace[i][i].domin[value] == 1){
					forward_check_trace[i][i].domin[value] = 0;
					forward_check_trace[i][i].remain--;
					if(forward_check_trace[i][i].remain == 0){
						return 0;
					}
				}
			}
			i++;
		}
	}
	if(row + column == 8){
		i = 0;
		while(i < 9){
			if(i != row){
				if(forward_check_trace[i][8-i].domin[value] == 1){
					forward_check_trace[i][8-i].domin[value] = 0;
					forward_check_trace[i][8-i].remain--;
					if(forward_check_trace[i][8-i].remain == 0){
						return 0;
					}
				}
			}
			i++;
		}
	}
	return 1;
}

void get_next_var(int row, int column)
{
	NEXT_ROW = row;
	NEXT_COLUMN = column;
	while(sudoku_store[NEXT_ROW][NEXT_COLUMN] != 0){
		if(NEXT_ROW == 8 && NEXT_COLUMN == 8){
			NEXT_ROW = 0;
			NEXT_COLUMN = 0;
			return;
		}
		NEXT_ROW = NEXT_COLUMN == 8 ? (NEXT_ROW + 1) : NEXT_ROW;
		NEXT_COLUMN = NEXT_COLUMN == 8 ? 0 : (NEXT_COLUMN + 1);
	}
	return;
}

void MRV_get_next()
{
	NEXT_ROW = 0;
	NEXT_COLUMN = 0;
	int i = 0;
	while(i < 9){
		int j = 0;
		while(j < 9){
			if(sudoku_store[i][j] == 0 &&
			(forward_check_trace[i][j].remain < forward_check_trace[NEXT_ROW][NEXT_COLUMN].remain || sudoku_store[NEXT_ROW][NEXT_COLUMN] != 0))
			{
					NEXT_ROW = i;
					NEXT_COLUMN = j;
			}
			j++;
		}
		i++;
	}
}

// dir = 1 means column, dir = 2 means row, dir = 3 means diagonal, dir = 4 means region
// return 1 means legal, 0 means illegal
int check(int dir, int num)
{
	int i = 0;
	while(i < 10){
		sudoku_check[i] = 0;
		i++;
	}
	if(dir == 1){
		int tem_column = num;
		int tem = 0;
		while(sudoku_check[sudoku_store[tem][tem_column]] == 0){
			if(sudoku_store[tem][tem_column] != 0){
				sudoku_check[sudoku_store[tem][tem_column]] = 1;
			}
			tem++;
			if(tem == 9){
				return 1;
			}
		}
		return 0;
	}
	else if(dir == 2){
		int tem_row = num;
		int tem = 0;
		while(sudoku_check[sudoku_store[tem_row][tem]] == 0){
			if(sudoku_store[tem_row][tem] != 0){
				sudoku_check[sudoku_store[tem_row][tem]] = 1;				
			}
			tem++;
			if(tem == 9){
				return 1;
			}
		}
		return 0;
	}
	else if(dir == 3 && num == 1){
		int tem = 0;
		while(sudoku_check[sudoku_store[tem][tem]] == 0){
			if(sudoku_store[tem][tem] != 0){
				sudoku_check[sudoku_store[tem][tem]] = 1;
			}
			tem++;
			if(tem == 9){
				return 1;
			}
		}
		return 0;
	}
	else if(dir == 3 && num == 2){
		int tem = 0;
		while(sudoku_check[sudoku_store[tem][8-tem]] == 0){
			if(sudoku_store[tem][8-tem] != 0){
				sudoku_check[sudoku_store[tem][8-tem]] = 1;
			}
			tem++;
			if(tem == 9){
				return 1;
			}
		}
		return 0;
	}
	else if(dir == 4){
		int tem_row = (num / 3) * 3;
		int tem_column = (num % 3) * 3;
		int i = 0;
		while(i < 3){
			int j = 0;
			while(j < 3){
				if(sudoku_check[sudoku_store[tem_row+i][tem_column+j]] == 1){
					return 0;
				}
				else{
					if(sudoku_store[tem_row+i][tem_column+j] != 0){
						sudoku_check[sudoku_store[tem_row+i][tem_column+j]] = 1;						
					}
				}
				j++;
			}
			i++;
		}
		return 1;
	}
	return 0;
}

void CSP_print()
{
	int i = 0;
	int j = 0;
	while(i < 9){
		while(j < 9){
			printf("%d ", sudoku_store[i][j]);
			j++;
		}
		printf("\n");
		i++;
		j = 0; 
	}
	return;
}

void sudoku_recall(int row, int column, int value)
{
	//fprintf(debugfile, "set (%d, %d) with %d\n", row, column, value);
	nodeNum++;
	sudoku_store[row][column] = value;
	Domin forward_check_image[9][9];
	int i = 0;
	while(i < 9){
		int j = 0;
		while(j < 9){
			forward_check_image[i][j] = forward_check_trace[i][j];
			j++;
		}
		i++;
	}
	if(check(1, column) == 0){
		sudoku_store[row][column] = 0;
		//fprintf(debugfile, "column %d check failure\n", column);
		return;
	}
	else{
		if(check(2, row) == 0){
			sudoku_store[row][column] = 0;
			//fprintf(debugfile, "row %d check failure\n", row);
			return;
		}
		else{
			if(check(4, standard_region[row][column]) == 0){
				sudoku_store[row][column] = 0;
				//fprintf(debugfile, "region %d check failure\n", standard_region[row][column]);
				return;
			}
			else{
				if(row == column){
					if(check(3, 1) == 0){
						sudoku_store[row][column] = 0;
						//fprintf(debugfile, "+diagonal check failure\n");
						return;
					}
				}
				if(row == 8-column){
					if(check(3, 2) == 0){
						sudoku_store[row][column] = 0;
						//fprintf(debugfile, "-diagonal check failure\n");
						return;
					}
				}
				// this value is OK, start forward checking
				/*
				if(forward_check(row, column) == 0){
					sudoku_store[row][column] = 0;
					fprintf(debugfile, "forward check failure\n");
					i = 0;
					while(i < 9){
						int j = 0;
						while(j < 9){
							forward_check_trace[i][j] = forward_check_image[i][j];
							j++;
						}
					i++;
					}
					return;
				}
				*/
				int value_pointer = 0;
				//MRV_get_next();
				//printf("(%d,%d)\n", NEXT_ROW, NEXT_COLUMN);
				get_next_var(row, column);
				if(NEXT_ROW == 0 && NEXT_COLUMN == 0){
					CSP_print();
					printf("%d\n", nodeNum);
					done = 1;
					return;
				}
				int temp_row = NEXT_ROW;
				int temp_column = NEXT_COLUMN;
				while(value_pointer < 9){
					sudoku_recall(temp_row, temp_column, value_set[value_pointer]);
					if(done == 1)
					{
						return;
					}
					value_pointer++;
				}
			}
		}
	}
	sudoku_store[row][column] = 0;
	i = 0;
	while(i < 9){
		int j = 0;
		while(j < 9){
			forward_check_trace[i][j] = forward_check_image[i][j];
			j++;
		}
		i++;
	}
	return;
}

void forward_check_initial()
{
	int i = 0;
	while(i < 9){
		int j = 0;
		while(j < 9){
			int m = 0;
			while(m < 10){
				forward_check_trace[i][j].domin[m] = 1;
				m++;
			}
			forward_check_trace[i][j].remain = 9;
			j++;
		}
		i++;
	}
	i = 0;
	while(i < 9){
		int j = 0;
		while(j < 9){
			forward_check(i, j);
			j++;
		}
		i++;
	}
	return;
}

int main()
{
	// sudoku file input
	FILE *file = NULL;
	if((file = fopen(FILE_PATH, "r")) == NULL){
		printf("File doesn't exists!\n");
		return 0;
	}
	fread(sudoku_buffer, sizeof(char), 256, file);
	fclose(file);
	// done
	// store into 2d array
	int sudoku_row = 0;
	int sudoku_column = 0;
	int buffer_pointer = 0;
	while(sudoku_buffer[buffer_pointer] != '\0'){
		if(sudoku_buffer[buffer_pointer] == ' '){
			sudoku_column++;
		}
		else if(sudoku_buffer[buffer_pointer] == '\n'){
			sudoku_column = 0;
			sudoku_row++;
		}
		else{
			sudoku_store[sudoku_row][sudoku_column] = sudoku_buffer[buffer_pointer] - '0';
		}
		buffer_pointer++;
	}
	// done
	//forward_check_initial();
	int value_p = 0;
	if(sudoku_store[0][0] != 0){
		//MRV_get_next();
		get_next_var(0,0);
	}
	int temp_row = NEXT_ROW;
	int temp_column = NEXT_COLUMN;
	while(value_p < 9){
		sudoku_recall(temp_row, temp_column, value_set[value_p]);
		if(done == 1)
		{
			break;
		}
		value_p++;
	}
	return 0;
}
