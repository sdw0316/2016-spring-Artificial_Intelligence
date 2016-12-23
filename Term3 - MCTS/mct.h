#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <math.h>
#include <time.h>
#include "board.h"

using namespace std;

#define DEPTH 1
#define LOOP 20000
#define MIN -2147483647
#define MAX 2147483647
#define TRUE 1
#define FALSE 0

/*�� Ʈ���� ���� �� board�� value�� ������ �ִ�.
AI�� value�� ���� ���� ���� ���� �����Ѵ�.

��������� user�� ���� ������ ��带, AI�� ���� ������ node�� �����ؾ��Ѵ�.
*/

/*����Ʈ�� ������ ����� ����ü*/
typedef struct gamenode {
	State *state;	//�� node�� state�� ��Ī�ϴ� state pointer

	int depth;	//turn�� ����ϱ� ���� tree�� depth
	double val;	//�� state�� ����� ���� (score/visit)
	double score;	//����� ������ ����
	double visit;	//�� state�� �湮 Ƚ��
	int unable;	//�� node�� �� �����̸� select �ϸ� �ȴ�

	struct gamenode *parent;	//�θ���
	struct gamenode *child[SIZE*SIZE];	//��� �ڽĳ���
	int childnum;	//�ڽĳ�� ����
}Node;

Node * makeNode(State* state, int depth); //����Ʈ���� ��带 �����.
void makeChilde(Node *main, Node *sub); //main����� �ڽĳ��鿡 sub�� �߰��Ѵ�.
void cpyState(State* s1, State* s2);	//��� ������ state�����Ҳ��� �� �� ���
Node * MCTS(Node* root); //root��带 �������� MCTS
Node * MCTS2(Node* root); //root��带 �������� MCTS

Node * selection(Node* node);	//not expanded�� ��� �� �ϳ� ����Ʈ

Node* expand(Node* node);	//fully expanded ���� ���� ����� �����ϰ� expand
int notExpand(Node* node);	//expand ���� �ʾ����� TRUE;

int playout(Node* node);

void backPro(Node* node, int result);

/*
for debuging
*/

void nodeInfo(Node* node);