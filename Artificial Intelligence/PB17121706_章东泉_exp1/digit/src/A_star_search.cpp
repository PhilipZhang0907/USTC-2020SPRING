#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string>
#include<iostream>
#define inputfile "../input/2.txt"

struct State{
	int status[5][5];
	int blank1[2];
	int blank2[2];
	int forward_cost;
	int manhattan;
	int disorder;
	int heuristics;
	int award7;
	int value;
	std::string path;
};

struct Node{
	State stateNode;
	Node* nextState;
};

struct CloseNode{
	State stateNode;
	CloseNode* leftchild;
	CloseNode* rightchild;
};

Node* QUEUE = NULL;
CloseNode* ROOT = NULL;
int DONE = 0;
int nodeNum = 0;
char inputBuffer[256];
char hashbuffer[256];
int initialInput[5][5] = {0};
int goalState[5][5] = {{1, 2, 3, 4, 5},
					   {7, 7, 8, 9, 10},
					   {6, 7, 11,12,13},
					   {14,15,16,17,18},
					   {19,20,21,0, 0}};
int goalPosition[21][2] = {{0,0}, {0,1}, {0,2}, {0,3}, {0,4},
						   {2,0}, {1,0}, {1,2}, {1,3}, {1,4},
						   {2,2}, {2,3}, {2,4}, {3,0}, {3,1},
						   {3,2}, {3,3}, {3,4}, {4,0}, {4,1}, {4,2}};

// check for state detail when meet the same heuristics	
int checkState(CloseNode* q, Node* p)
{
	int flag = 1;
	int i = 0;
	while(i < 5){
		int j = 0;
		while(j < 5){
			if(q->stateNode.status[i][j] != p->stateNode.status[i][j]){
				flag = 0;
				return flag;
			}
			j++;
		}
		i++;
	}
	return flag;
}

// Tree is ordered by heuristics			   
int searchNode(CloseNode* q, Node* p)
{
	if(q == NULL){
		return 0;
	}
	if(q->stateNode.heuristics == p->stateNode.heuristics){
		if(checkState(q, p) == 1)
		{
			return 1;
		}
		else{
			return searchNode(q->rightchild, p);
		}
	}
	else{
		if(q->stateNode.heuristics < p->stateNode.heuristics){
			return searchNode(q->rightchild, p);
		}
		else if(q->stateNode.heuristics > p->stateNode.heuristics){
			return searchNode(q->leftchild, p);
		}
	}
	return 0;
}

void insertTree(CloseNode* q, Node* p)
{
	if(ROOT == NULL){
		ROOT = new CloseNode;
		ROOT->rightchild = NULL;
		ROOT->leftchild = NULL;
		ROOT->stateNode = p->stateNode;
		return;
	}
	if(q->stateNode.heuristics <= p->stateNode.heuristics){
		if(q->rightchild == NULL){
			q->rightchild = new CloseNode;
			q->rightchild->leftchild = NULL;
			q->rightchild->rightchild = NULL;
			q->rightchild->stateNode = p->stateNode;
			return;
		}
		else{
			insertTree(q->rightchild, p);
			return;
		}
	}
	else{
		if(q->leftchild == NULL){
			q->leftchild = new CloseNode;
			q->leftchild->leftchild = NULL;
			q->leftchild->rightchild = NULL;
			q->leftchild->stateNode = p->stateNode;
			return;
		}
		else{
			insertTree(q->leftchild, p);
			return;
		}
	}	
}

void insertNode(Node* p)
{
	Node* tmp = QUEUE;
	while(tmp->nextState != NULL && tmp->nextState->stateNode.value <= p->stateNode.value){
		tmp = tmp->nextState;
	}
	if(tmp->nextState == NULL){
		tmp->nextState = p;
		return;
	}
	else{
		Node* q = tmp->nextState;
		tmp->nextState = p;
		p->nextState = q;
		return;
	}
}

int Manhattan(int num, int i, int j)
{
	int result = 0;
	if(num == 7 || num == 0){
		return 0;
	}
	else{
		int x = goalPosition[num-1][0];
		int y = goalPosition[num-1][1];
		result =  abs(i - x) + abs(j - y);
	}
	return result;
}

int seven_award(int i, int j)
{
	return abs(i - 1) + abs(j - 0);
}

int Disorder(int num, Node* p)
{
	int result = 0;
	int i = 0;
	while(i < 5){
		int j = i;
		while(j < 5){
			if(p->stateNode.status[num][i] > p->stateNode.status[num][j] && p->stateNode.status[num][j] != 0){
				result++;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while(i < 5){
		int j = i;
		while(j < 5){
			if(p->stateNode.status[i][num] > p->stateNode.status[j][num]){
				result++;
			}
			j++;
		}
		i++;
	}
	return result;
}

void Heuristics_Do(Node* p)
{
	int i = 0;
	int flag = 0;
	p->stateNode.manhattan = 0;
	p->stateNode.disorder = 0;
	while(i < 5){
		int j = 0;
		while(j < 5){
			if(p->stateNode.status[i][j] == 7 && flag == 0)
			{
				p->stateNode.award7 = seven_award(i, j);
				flag = 1;
			}
			p->stateNode.manhattan += Manhattan(p->stateNode.status[i][j], i, j);
			j++;
		}
		p->stateNode.disorder += Disorder(i, p);
		i++;
	}
	return;
}

void A_star_Print(Node* p)
{
	/*
	int i = 0;
	while(i < 5){
		int j = 0;
		while(j < 5){
			printf("%d\t",QUEUE->stateNode.status[i][j]);
			j++;
		}
		printf("\n");
		i++;
	}
	*/
	nodeNum++;
	printf("No. %d, Heuristics: %d", nodeNum, p->stateNode.heuristics);
	printf("\n");
}

void A_star_Search()
{
	if(QUEUE != NULL){
		insertTree(ROOT, QUEUE);
		A_star_Print(QUEUE);
	}
	/*
	if(QUEUE->stateNode.heuristics < 50){
		int i = 0;
		while(i < 5){
			int j = 0;
			while(j < 5){
				printf("%d ",QUEUE->stateNode.status[i][j]);
				j++;
			}
			printf("\n");
			i++;
		}
		system("pause");
	}
	*/
	if(QUEUE->stateNode.manhattan == 0){
		DONE = 1;
		printf("DONE!\n");
		int i = 0;
		while(i < 5){
			int j = 0;
			while(j < 5){
				printf("%d ",QUEUE->stateNode.status[i][j]);
				j++;
			}
			printf("\n");
			i++;
		}
		std::cout << "Path: " << QUEUE->stateNode.path;
		return;
	}
	else{
		int x1 = QUEUE->stateNode.blank1[0];
		int y1 = QUEUE->stateNode.blank1[1];
		int x2 = QUEUE->stateNode.blank2[0];
		int y2 = QUEUE->stateNode.blank2[1];
		if(x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > 4 || x2 > 4 || y1 > 4 || y2 > 4){
			int tmp_i = 0;
			while(tmp_i < 5){
				int tmp_j = 0;
				while(tmp_j < 5){
					printf("%d ", QUEUE->stateNode.status[tmp_i][tmp_j]);
					tmp_j++;
				}
				tmp_i++;
				printf("\n");
			}
			system("pause");
		}
		// location of two blank tiles
		if(x1 > 0 && QUEUE->stateNode.status[x1-1][y1] != 0 && QUEUE->stateNode.status[x1-1][y1] != 7){
			// blank tile 1 can move upwards
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = p->stateNode.status[x1-1][y1];
			p->stateNode.status[x1-1][y1] = 0;
			p->stateNode.blank1[0]--;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,d)";
			operation[1] = (p->stateNode.status[x1][y1] / 10) + '0';
			operation[2] = (p->stateNode.status[x1][y1] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(y1 > 0 && QUEUE->stateNode.status[x1][y1-1] != 0 && QUEUE->stateNode.status[x1][y1-1] != 7){
			// blank tile 1 can move to left
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = p->stateNode.status[x1][y1-1];
			p->stateNode.status[x1][y1-1] = 0;
			p->stateNode.blank1[1]--;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,r)";
			operation[1] = (p->stateNode.status[x1][y1] / 10) + '0';
			operation[2] = (p->stateNode.status[x1][y1] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(x1 < 4 && QUEUE->stateNode.status[x1+1][y1] != 0 && QUEUE->stateNode.status[x1+1][y1] != 7){
			// blank tile 1 can move downwards
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = p->stateNode.status[x1+1][y1];
			p->stateNode.status[x1+1][y1] = 0;
			p->stateNode.blank1[0]++;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,u)";
			operation[1] = (p->stateNode.status[x1][y1] / 10) + '0';
			operation[2] = (p->stateNode.status[x1][y1] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(y1 < 4 && QUEUE->stateNode.status[x1][y1+1] != 0 && QUEUE->stateNode.status[x1][y1+1] != 7){
			// blank tile 1 can move to right
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = p->stateNode.status[x1][y1+1];
			p->stateNode.status[x1][y1+1] = 0;
			p->stateNode.blank1[1]++;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,l)";
			operation[1] = (p->stateNode.status[x1][y1] / 10) + '0';
			operation[2] = (p->stateNode.status[x1][y1] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(x2 > 0 && QUEUE->stateNode.status[x2-1][y2] != 0 && QUEUE->stateNode.status[x2-1][y2] != 7){
			// blank tile 2 can move upwards
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x2][y2] = p->stateNode.status[x2-1][y2];
			p->stateNode.status[x2-1][y2] = 0;
			p->stateNode.blank2[0]--;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,d)";
			operation[1] = (p->stateNode.status[x2][y2] / 10) + '0';
			operation[2] = (p->stateNode.status[x2][y2] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(y2 > 0 && QUEUE->stateNode.status[x2][y2-1] != 0 && QUEUE->stateNode.status[x2][y2-1] != 7){
			// blank tile 2 can move to left
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x2][y2] = p->stateNode.status[x2][y2-1];
			p->stateNode.status[x2][y2-1] = 0;
			p->stateNode.blank2[1]--;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,r)";
			operation[1] = (p->stateNode.status[x2][y2] / 10) + '0';
			operation[2] = (p->stateNode.status[x2][y2] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(x2 < 4 && QUEUE->stateNode.status[x2+1][y2] != 0 && QUEUE->stateNode.status[x2+1][y2] != 7){
			// blank tile 2 can move downwards
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x2][y2] = p->stateNode.status[x2+1][y2];
			p->stateNode.status[x2+1][y2] = 0;
			p->stateNode.blank2[0]++;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,u)";
			operation[1] = (p->stateNode.status[x2][y2] / 10) + '0';
			operation[2] = (p->stateNode.status[x2][y2] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(y2 < 4 && QUEUE->stateNode.status[x2][y2+1] != 0 && QUEUE->stateNode.status[x2][y2+1] != 7){
			// blank tile 2 can move to right
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x2][y2] = p->stateNode.status[x2][y2+1];
			p->stateNode.status[x2][y2+1] = 0;
			p->stateNode.blank2[1]++;
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 18;
			std::string operation = "(00,l)";
			operation[1] = (p->stateNode.status[x2][y2] / 10) + '0';
			operation[2] = (p->stateNode.status[x2][y2] % 10) + '0';
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(x1 < 4 && x2 < 4 && QUEUE->stateNode.status[x1+1][y1] == 7 && QUEUE->stateNode.status[x2+1][y2] == 7){
			// tile 7 move upwards
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = 7;
			p->stateNode.status[x2][y2] = 7;
			if(y1 < y2){
				p->stateNode.status[x1+1][y1] = 0;
				p->stateNode.status[x2+2][y2] = 0;
				p->stateNode.blank1[0] += 1;
				p->stateNode.blank2[0] += 2;
			}
			else{
				p->stateNode.status[x1+2][y1] = 0;
				p->stateNode.status[x2+1][y2] = 0;
				p->stateNode.blank1[0] += 2;
				p->stateNode.blank2[0] += 1;
			}
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 6;
			std::string operation = "(07,u)";
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(x1 > 0 && x2 > 0 && QUEUE->stateNode.status[x1-1][y1] == 7 && QUEUE->stateNode.status[x2-1][y2] == 7){
			// tile 7 move downwards
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = 7;
			p->stateNode.status[x2][y2] = 7;
			if(y1 < y2){
				p->stateNode.status[x1-1][y1] = 0;
				p->stateNode.status[x2-2][y2] = 0;
				p->stateNode.blank1[0] -= 1;
				p->stateNode.blank2[0] -= 2;
			}
			else{
				p->stateNode.status[x1-2][y1] = 0;
				p->stateNode.status[x2-1][y2] = 0;
				p->stateNode.blank1[0] -= 2;
				p->stateNode.blank2[0] -= 1;
			}
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 6;
			std::string operation = "(07,d)";
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(y1 < 4 && y2 < 4 && QUEUE->stateNode.status[x1][y1+1] == 7 && QUEUE->stateNode.status[x2][y2+1] == 7){
			// tile 7 move left
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = 7;
			p->stateNode.status[x2][y2] = 7;
			if(x1 < x2){
				p->stateNode.status[x1][y1+2] = 0;
				p->stateNode.status[x2][y2+1] = 0;
				p->stateNode.blank1[1] += 2;
				p->stateNode.blank2[1] += 1;
			}
			else{
				p->stateNode.status[x1][y1+1] = 0;
				p->stateNode.status[x2][y2+2] = 0;
				p->stateNode.blank1[1] += 1;
				p->stateNode.blank2[1] += 2;
			}
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 6;
			std::string operation = "(07,l)";
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
		if(y1 > 0 && y2 > 0 && QUEUE->stateNode.status[x1][y1-1] == 7 && QUEUE->stateNode.status[x2][y2-1] == 7){
			// tile 7 move right
			Node* p = new Node;
			p->nextState = NULL;
			p->stateNode = QUEUE->stateNode;
			p->stateNode.status[x1][y1] = 7;
			p->stateNode.status[x2][y2] = 7;
			if(x1 < x2){
				p->stateNode.status[x1][y1-2] = 0;
				p->stateNode.status[x2][y2-1] = 0;
				p->stateNode.blank1[1] -= 2;
				p->stateNode.blank2[1] -= 1;
			}
			else{
				p->stateNode.status[x1][y1-1] = 0;
				p->stateNode.status[x2][y2-2] = 0;
				p->stateNode.blank1[1] -= 1;
				p->stateNode.blank2[1] -= 2;
			}
			p->stateNode.forward_cost++;
			Heuristics_Do(p);
			p->stateNode.heuristics = p->stateNode.manhattan + 4 * p->stateNode.disorder;
			p->stateNode.value = p->stateNode.heuristics + p->stateNode.forward_cost + p->stateNode.award7 * 6;
			std::string operation = "(07,r)";
			p->stateNode.path += operation;
			if(searchNode(ROOT, p) == 0){
				insertNode(p);
			}
			else{
				free(p);
			}
		}
	}
	Node* tmp = QUEUE;
	QUEUE = tmp->nextState;
	free(tmp);
	return;
}

int main()
{
	FILE* file = NULL;
	if((file = fopen(inputfile, "r")) == NULL){
		printf("Input file does not exists!\n");
		return 0;
	}
	fread(inputBuffer, sizeof(char), 256, file);
	fclose(file);
	int bufferPointer = 0;
	int i = 0;
	int j = 0;
	int temp_char;
	QUEUE = new Node;
	int flag = 0;
	while((temp_char = inputBuffer[bufferPointer]) != '\0'){
		if(temp_char == ','){
			QUEUE->stateNode.status[i][j] = initialInput[i][j];
			j++;
			bufferPointer++;
			continue;
		}
		else if(temp_char == '\n'){
			QUEUE->stateNode.status[i][j] = initialInput[i][j];
			i++;
			j = 0;
			bufferPointer++;
			continue;
		}
		else{
			if(temp_char == '0' && initialInput[i][j] == 0){
				if(flag == 0){
					QUEUE->stateNode.blank1[0] = i;
					QUEUE->stateNode.blank1[1] = j;
					flag = 1;
				}
				else{
					QUEUE->stateNode.blank2[0] = i;
					QUEUE->stateNode.blank2[1] = j;
				}
			}
			initialInput[i][j] = initialInput[i][j]*10 + (temp_char - '0');
			bufferPointer++;
			continue;
		}
	}
	QUEUE->stateNode.status[i][j] = initialInput[i][j];
	// done initial state input
	QUEUE->stateNode.forward_cost = 0;
	Heuristics_Do(QUEUE);
	QUEUE->stateNode.heuristics = QUEUE->stateNode.manhattan + 4 * QUEUE->stateNode.disorder;
	QUEUE->stateNode.value = QUEUE->stateNode.forward_cost + QUEUE->stateNode.heuristics + QUEUE->stateNode.award7 * 18;
	QUEUE->nextState = NULL;
	QUEUE->stateNode.path = "";
	i = 0;
	while(i < 5){
		j = 0;
		while(j < 5){
			printf("%d ",QUEUE->stateNode.status[i][j]);
			j++;
		}
		i++;
		printf("\n");
	}
	//A_star_Search();
	/*
	printf("\n");
	Node* GOAL = new Node;
	int tmp_i = 0;
	while(tmp_i < 5){
		int tmp_j = 0;
		while(tmp_j < 5){
			GOAL->stateNode.status[tmp_i][tmp_j] = goalState[tmp_i][tmp_j];
			tmp_j++;
		}
		tmp_i++;
	}
	Heuristics_Do(GOAL);
	printf("%d\n", GOAL->stateNode.disorder);
	*/
	while(DONE == 0){
		A_star_Search();
	}
	return 0;
}
