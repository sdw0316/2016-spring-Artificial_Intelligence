#pragma once

#include <vector>

#define TRUE 1
#define FALSE 0
#define P1 1
#define P2 2
#define SIZE 15
#define WINLEN	5
#define SWAP(x,y,t) ((t)=(x),(x)=(y),(y)=(t)) 

using namespace std;

typedef struct _pos {
	int x;
	int y;
} Pos;

/*�� ���� ���� �����ϴ� state�Լ�*/
typedef struct _state {
	int board[SIZE][SIZE];	//���� ���� ����
	Pos pos;	//�� �ǿ� ���� �� ��ǥ
	Pos action[SIZE*SIZE];	//������ �� ������ ��ǥ
	int len;	//action�� lenth;
} State;

/**********state ���� �Լ���**********************/
void init(State* state);	//���� ���۽� state �ʱ�ȭ
void showState(State* state);
void changeState(State* state, Pos pos, int player);

/*********************************������ �˻�**************************/
void makeBoard(int(*board)[SIZE], int n); //���� �ٵ����� ���¸� ǥ��
int checkBoard(State* state); //�ٵ����� ���� á���� �˻�
int inputBoard(State* state , Pos pos, int player); //�ٵ��ǿ� �� ����
int inputChecker(int(*board)[SIZE], Pos pos); //�ٵ��ǿ� ���� �� ��ġ ����
int isEmpty(int(*board)[SIZE]);	//�ٵ��� ����ֳ� �˻�
int checkWin(int(*board)[SIZE], Pos pos, int player); //���� �� ���� �� �������� 3�������� ���� �����ִ��� �˻�
int checkDir(int(*board)[SIZE], Pos pos, int row, int set); //checkWin �ȿ��� �����ϴ� �Լ�, �� �������� �� �������� �����ִ��� �˻�

/**************3*3üũ �Լ�******************/
int renju(int(*board)[SIZE], Pos pos, int player); //player1 ���ʿ� pos�� �� ���� �ݼ����� �˻�.
int checkThree(int(*board)[SIZE], Pos pos, int row, int start, int player); //���� 3���� �Ǵ�.
int checkFour(int(*board)[SIZE], Pos pos, int row, int set, int player); //���� 4�Ǵ�

