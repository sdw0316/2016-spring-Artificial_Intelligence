#include "board.h"
#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define TRUE 1
#define FALSE 0

int dir[4][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 } }; //�¸� üũ�� ���� �˻� ����
int sample3[5] = { 0,1,1,1,0 };	//���������� 3�� ���ϱ� ���� ��
int sample4[6] = { 0,1,1,1,1,0 }; //�ϳ� �� ������ 4�� �Ǵ� 3�� ���ϱ� ���� ��

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
						printf("��");
					else if (board[i - 1][j - 1] == P2)
						printf("��");
					else {
						if (i == 1) {
							if (j == 1)
								printf("��");
							else if (j == n)
								printf("��");
							else
								printf("��");
						}
						else if (i == n) {
							if (j == 1)
								printf("��");
							else if (j == n)
								printf("��");
							else
								printf("��");
						}
						else {
							if (j == 1)
								printf("��");
							else if (j == n)
								printf("��");
							else
								printf("��");
						}
					}
				}
			}
		}
		printf("\n");
	}
}

/*����� (0)�� ������ TRUE �ƴϸ� FALSE*/
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

/*�ٵ��ǿ� ���� �ΰ� ���� ����ġ ����*/
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

/*������ �ش� ��ġ�� ���� �� �� �ִ°� �˻�*/
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
/*�� �ڸ��� ������ �¸� �˻�*/
int checkWin(int(*board)[SIZE], Pos pos, int player) {
	for (int row = 0; row < 4; row++) {
		int len = 1;
		len += checkDir(board, pos, row, 1); //������ �˻�
		len += checkDir(board, pos, row, -1);//������ �˻�
		if (len == WINLEN) {
			return player;
		}
	}
	return FALSE;
}

/*�������� line �˻�*/
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
tree�� ��Ʈ���� ���� ���� ū child����� value ���� ������ �ִ�.
�� ���� ���� value�� ���� child��带 ������ ��
�� ��忡�� ���� ���� ��ǥ���� return �Ѵ�.
*/
Pos Ai(int(*board)[SIZE]) {
	Pos pos;
	if (isEmpty(board)) {
		pos.x = SIZE / 2;
		pos.y = SIZE / 2;
		return pos;
	}
	Node *root = makeTree(board, DEPTH);	//���Լ� ������ tree ����
	for (int i = 0; i < SIZE*SIZE; i++) {
		/*
		root ����� �ڽ� �� root�� val�� ���� ���� ��带 ã��
		�� ��忡 ���� �߰��� x, y ��ǥ�� ã�´�.
		*/
		if (root->val == root->child[i]->val) {
			pos = root->child[i]->pos;
			free(root->child[i]);
			free(root);
			return pos;
		}
		free(root->child[i]);//��� ���Ŀ� ������� Ʈ�� free
	}
}

/*
���� ��
*/
int renju(int(*board)[SIZE], Pos pos) {
	int num3 = 0; //3�� ���� �̳��� 2�Ǹ� ���� True;
	int row;
	for (row = 0; row < 4; row++) {
		int temp = 0;//�� ���⸶���� 3�� ���� -> �̳��� �� 1���� �־�� 3���� ����
					 /*���θ��� 3�� ����  �ľ�*/
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
	/*�� �ڸ��� ���� �� 4�� ����� �ִ� 3 �ľ�*/
	for (int i = start; i <= 4 + start; i++) {
		int ny = pos.y + i*dir[row][0];
		int nx = pos.x + i*dir[row][1];
		if (i == 0) {
			continue;
		}
		if (sample3[i - start] != board[ny][nx]) {//���� �̰� �ٸ��� �߰��� ���� ���Ե��ְų� �����ְų� 4��
			return 0;
		}
	}
	/*���� 3����� ���� �� 3�� �� ���� ���� ������, ���� 4 ����� ���� �˻�*/
	Pos spos = { pos.x + start*dir[row][1], pos.y + (start)*dir[row][0] };
	Pos lpos = { pos.x + (4 + start)*dir[row][1], pos.y + (4 + start)*dir[row][0] };
	return (checkFour(board, spos, row, 1) || checkFour(board, lpos, row, -1));
}

int checkFour(int(*board)[SIZE], Pos pos, int row, int set) {
	int temp = 0;//�Ѱ� 0�� ��� ok
	for (int i = -1; i <= 4; i++) {
		int ny = pos.y + dir[row][0] * i*set;
		int nx = pos.x + dir[row][1] * i*set;
		/*���� ������*/
		if (i == 0) {
			continue;
		}
		if (sample4[((i + 1)*set)] != board[ny][nx]) {//���� �̰� �ٸ��� �߰��� ���� ���Ե��ְų� �����ְų� 4��
			if (board[ny][nx] == P2 || i == -1 || i == 4)//���� ���� ���������� ���� 4 �ȳ����ϱ� �׳� 0 ���� �� ���� �ٸ��� ���� 4 �ƴϹǷ� ����
				return 0;
			temp++;
			if (temp < 2)//�ƴϸ�(0) �Ѱ������� ������ �ְ� �� �ܿ� 0 ����
				continue;
			else
				return 0;
		}
	}
	if (temp == 1)//4��°� ������ �߰��� 0�ϳ��� �־�� �˴ϴ�.
		return 1;
	else//�װ� �ƴϸ� �׳� 4���� (3,4)üũ�� �ȵǿ�.
		return 0;
}

int checkLine(int(*board)[SIZE], Pos pos, int num) {
	for (int row = 0; row < 4; row++) {
		int len = 1;
		len += checkDir(board, pos, row, 1); //������ �˻�
		len += checkDir(board, pos, row, -1);//������ �˻�
		if (len == num) {
			return true;
		}
	}
	return FALSE;
}