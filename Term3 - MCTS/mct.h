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

/*각 트리의 노드는 한 board의 value를 가지고 있다.
AI는 value가 가장 높은 다음 수를 선택한다.

결론적으로 user는 낮은 점수의 노드를, AI는 높은 점수의 node를 선택해야한다.
*/

/*게임트리 각각의 노드의 구조체*/
typedef struct gamenode {
	State *state;	//각 node의 state를 지칭하는 state pointer

	int depth;	//turn을 계산하기 위한 tree의 depth
	double val;	//이 state의 통계적 점수 (score/visit)
	double score;	//노드의 절대적 점수
	double visit;	//이 state의 방문 횟수
	int unable;	//이 node가 룰 위반이면 select 하면 안댐

	struct gamenode *parent;	//부모노드
	struct gamenode *child[SIZE*SIZE];	//모든 자식노드들
	int childnum;	//자식노드 개수
}Node;

Node * makeNode(State* state, int depth); //게임트리의 노드를 만든다.
void makeChilde(Node *main, Node *sub); //main노드의 자식노드들에 sub를 추가한다.
void cpyState(State* s1, State* s2);	//노드 형성시 state복사할껀데 그 때 사용
Node * MCTS(Node* root); //root노드를 기준으로 MCTS
Node * MCTS2(Node* root); //root노드를 기준으로 MCTS

Node * selection(Node* node);	//not expanded한 놈들 중 하나 셀렉트

Node* expand(Node* node);	//fully expanded 되지 않은 노드라면 랜덤하게 expand
int notExpand(Node* node);	//expand 되지 않았으면 TRUE;

int playout(Node* node);

void backPro(Node* node, int result);

/*
for debuging
*/

void nodeInfo(Node* node);