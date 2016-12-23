#pragma once

#define TRUE 1
#define FALSE 0
#define P1 1
#define P2 2
#define SIZE 5
#define WINLEN 5

typedef struct _pos {
	int x;
	int y;
} Pos;

void makeBoard(int(*board)[SIZE], int n); //���� �ٵ����� ���¸� ǥ��
void printBoard(int(*board)[SIZE]);	//�ٵ��� ���
int checkBoard(int(*board)[SIZE], int n); //�ٵ����� ���� á���� �˻�
int inputBoard(int(*board)[SIZE], Pos pos, int player); //�ٵ��ǿ� �� ����
int inputChecker(int(*board)[SIZE], Pos pos); //�ٵ��ǿ� ���� �� ��ġ ����
int isEmpty(int(*board)[SIZE]);	//�ٵ��� ����ֳ� �˻�
/*
�Ʒ� �� �Լ��� tree������ ����� ���̱⿡ ����ü�� �ƴϰ� x,y�� ��ǥ ����
*/
int checkWin(int(*board)[SIZE], Pos pos, int player); //���� �� ���� �� �������� 3�������� ���� �����ִ��� �˻�
int checkDir(int(*board)[SIZE], Pos pos, int row, int set); //checkWin �ȿ��� �����ϴ� �Լ�, �� �������� �� �������� �����ִ��� �˻�

Pos Ai(int(*board)[SIZE]);	//���� board �Է¹ް� ���ƾ� �� ��ǥ return

/*
2���� �߰� �Լ�
*/
int renju(int(*board)[SIZE], Pos pos); //player1 ���ʿ� pos�� �� ���� �ݼ����� �˻�.
int checkThree(int(*board)[SIZE], Pos pos, int row, int start); //���� 3���� �Ǵ�.
int checkFour(int(*board)[SIZE], Pos pos, int row, int set); //���� 4�Ǵ�

int checkLine(int(*board)[SIZE], Pos pos, int num);//�� ���ο��� �� �� �˻�(checkwin�� ���׷��̵� ����) �޸���ƽ �Լ����� ���