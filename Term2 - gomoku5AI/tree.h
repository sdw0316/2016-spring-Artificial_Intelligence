#pragma once

#include "board.h"

#define DEPTH 3
#define MIN -5000
#define MAX 5000

/*�� Ʈ���� ���� �� board�� value�� ������ �ִ�.
AI�� value�� ���� ���� ���� ���� �����Ѵ�.

��������� user�� ���� ������ ��带, AI�� ���� ������ node�� �����ؾ��Ѵ�.
*/

/*����Ʈ�� ������ ����� ����ü*/
typedef struct gamenode {
	int board[SIZE][SIZE];
	Pos pos;
	int val;
	struct gamenode *parent;
	struct gamenode *child[SIZE*SIZE];
}Node;

Node * makeTree(int(*board)[SIZE], int depth); //AI�� �Ǵ��� �Ҷ����� �� ������ board�� ���ڷ� �ް� �׸� �������� depth �� ��ŭ ����Ѵ�.
Node * makeSubTree(Node *main, int temp, int depth);	//�� ����� �ڽ� Ʈ���� ����� �Լ�
Node * makeNode(int(*board)[SIZE]); //����Ʈ���� ��带 �����.
void makeChilde(Node *main, Node *sub, int n); //main����� n��° �ڽĳ�带 sub�� �Ѵ�.
int evalBoard(int(*board)[SIZE], int depth);	//�� ������ ���� ���