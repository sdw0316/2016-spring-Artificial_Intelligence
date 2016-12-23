#include "board.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

int dir[4][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 } }; //승리 체크를 위한 검사 방향
int sample3[5] = { 0,1,1,1,0 };	//놓았을때의 3을 비교하기 위한 셋
int sample4[6] = { 0,1,1,1,1,0 }; //하나 더 놓으면 4가 되는 3을 비교하기 위한 셋

void makeBoard(int(*board)[SIZE], int n) {
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++) {
			if (i == 0) {
				if (j == 0)
					printf("   ");
				else
					printf("%c ", 64 + j);
			}
			else {
				if (j == 0)
					printf("%2d", i);
				else {
					if (board[i - 1][j - 1] == P1)
						printf("●");
					else if (board[i - 1][j - 1] == P2)
						printf("○");
					else {
						if (i == 1) {
							if (j == 1)
								printf("┌");
							else if (j == n)
								printf("┐");
							else
								printf("┬");
						}
						else if (i == n) {
							if (j == 1)
								printf("└");
							else if (j == n)
								printf("┘");
							else
								printf("┴");
						}
						else {
							if (j == 1)
								printf("├");
							else if (j == n)
								printf("┤");
							else
								printf("┼");
						}
					}
				}
			}
		}
		printf("\n");
	}
}

/*빈공간 (0)이 있으면 TRUE 아니면 FALSE*/
int checkBoard(int(*board)[SIZE], int n) {
	int check = FALSE;
	for (int i = 0; i < n; i++) {
		if (check == TRUE)
			break;
		for (int j = 0; j < n; j++) {
			if (board[i][j] != P1 && board[i][j] != P2) {
				check = TRUE;
				break;
			}
		}
	}
	return check;
}

/*바둑판에 돌을 두고 판의 가중치 변경*/
int inputBoard(int(*board)[SIZE], Pos pos, int player) {
	int x = pos.x;
	int y = pos.y;
	if (inputChecker(board, pos)) {
		board[y][x] = player;
		return TRUE;
	}
	else {
		return FALSE;
	}
}

/*보드의 해당 위치에 돌을 둘 수 있는가 검사*/
int inputChecker(int(*board)[SIZE], Pos pos) {
	int x = pos.x;
	int y = pos.y;
	if (x<0 || x >= SIZE || y < 0 || y >= SIZE)
		return FALSE;
	if (board[y][x] != 0)
		return FALSE;
	return TRUE;
}

int isEmpty(int(*board)[SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] != 0)
				return FALSE;
		}
	}
	return TRUE;
}
/*그 자리에 뒀을때 승리 검사*/
int checkWin(int(*board)[SIZE], Pos pos, int player) {
	for (int row = 0; row < 4; row++) {
		int len = 1;
		len += checkDir(board, pos, row, 1); //정방향 검사
		len += checkDir(board, pos, row, -1);//역방향 검사
		if (len == WINLEN) {
			return player;
		}
	}
	return FALSE;
}

/*방향으로 line 검사*/
int checkDir(int(*board)[SIZE], Pos pos, int row, int set) {
	int result = 0;
	Pos npos = { pos.x + dir[row][1] * set, pos.y + dir[row][0] * set };
	int x = pos.x;
	int y = pos.y;
	int nx = npos.x;
	int ny = npos.y;

	if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
		return 0;
	else {
		if (board[ny][nx] == board[y][x] && board[y][x] != 0) {
			result++;
			result += checkDir(board, npos, row, set);
		}
	}
	return result;
}

void printBoard(int(*board)[SIZE]) {
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			printf("%d", board[y][x]);
		}
		printf("\n");
	}
}

/*
tree의 루트노드는 값이 가장 큰 child노드의 value 값을 가지고 있다.
이 값과 같은 value를 가진 child노드를 선택한 후
그 노드에서 새로 놓은 좌표값을 return 한다.
*/
Pos Ai(int(*board)[SIZE]) {
	Pos pos;
	if (isEmpty(board)) {
		pos.x = SIZE / 2;
		pos.y = SIZE / 2;
		return pos;
	}
	Node *root = makeTree(board, DEPTH);	//평가함수 적용한 tree 형성
	for (int i = 0; i < SIZE*SIZE; i++) {
		/*
		root 노드의 자식 중 root와 val과 값이 같은 노드를 찾아
		그 노드에 새로 추가된 x, y 좌표를 찾는다.
		*/
		if (root->val == root->child[i]->val) {
			pos = root->child[i]->pos;
			free(root->child[i]);
			free(root);
			return pos;
		}
		free(root->child[i]);//계산 이후엔 만들어진 트리 free
	}
}

/*
렌주 룰
*/
int renju(int(*board)[SIZE], Pos pos) {
	int num3 = 0; //3의 개수 이놈이 2되면 리턴 True;
	int row;
	for (row = 0; row < 4; row++) {
		int temp = 0;//각 방향마다의 3의 개수 -> 이놈이 딱 1개만 있어야 3으로 인정
					 /*라인마다 3의 개수  파악*/
		temp += checkThree(board, pos, row, -2);
		temp += checkThree(board, pos, row, -3);
		temp += checkThree(board, pos, row, -1);
		if (temp == 0) {
			temp += checkFour(board, pos, row, -1);
			temp += checkFour(board, pos, row, 1);
		}
		if (temp == 1)
			num3++;
		if (num3 == 2)
			return TRUE;
	}
	return FALSE;
}

int checkThree(int(*board)[SIZE], Pos pos, int row, int start) {
	int temp = 0;
	/*현 자리에 뒀을 시 4를 만들수 있는 3 파악*/
	for (int i = start; i <= 4 + start; i++) {
		int ny = pos.y + i*dir[row][0];
		int nx = pos.x + i*dir[row][1];
		if (i == 0) {
			continue;
		}
		if (sample3[i - start] != board[ny][nx]) {//만약 이게 다르면 중간에 흰돌이 포함되있거나 막혀있거나 4임
			return 0;
		}
	}
	/*만약 3만들어 지면 그 3의 양 끝에 돌을 뒀을시, 열린 4 만들어 지나 검사*/
	Pos spos = { pos.x + start*dir[row][1], pos.y + (start)*dir[row][0] };
	Pos lpos = { pos.x + (4 + start)*dir[row][1], pos.y + (4 + start)*dir[row][0] };
	return (checkFour(board, spos, row, 1) || checkFour(board, lpos, row, -1));
}

int checkFour(int(*board)[SIZE], Pos pos, int row, int set) {
	int temp = 0;//한개 0은 취급 ok
	for (int i = -1; i <= 4; i++) {
		int ny = pos.y + dir[row][0] * i*set;
		int nx = pos.x + dir[row][1] * i*set;
		/*현재 포지션*/
		if (i == 0) {
			continue;
		}
		if (sample4[((i + 1)*set)] != board[ny][nx]) {//만약 이게 다르면 중간에 흰돌이 포함되있거나 막혀있거나 4임
			if (board[ny][nx] == P2 || i == -1 || i == 4)//적군 돌로 막혀있으면 열린 4 안나오니까 그냥 0 리턴 양 끝이 다르면 열린 4 아니므로 리턴
				return 0;
			temp++;
			if (temp < 2)//아니면(0) 한개까지는 인정해 주고 그 외엔 0 리턴
				continue;
			else
				return 0;
		}
	}
	if (temp == 1)//4라는건 무조건 중간에 0하나가 있어야 됩니다.
		return 1;
	else//그게 아니면 그냥 4에요 (3,4)체크가 안되요.
		return 0;
}

int checkLine(int(*board)[SIZE], Pos pos, int num) {
	for (int row = 0; row < 4; row++) {
		int len = 1;
		len += checkDir(board, pos, row, 1); //정방향 검사
		len += checkDir(board, pos, row, -1);//역방향 검사
		if (len == num) {
			return true;
		}
	}
	return FALSE;
}