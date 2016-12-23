#pragma once

#include "board.h"

#define DEPTH 3
#define MIN -5000
#define MAX 5000

/*각 트리의 노드는 한 board의 value를 가지고 있다.
AI는 value가 가장 높은 다음 수를 선택한다.

결론적으로 user는 낮은 점수의 노드를, AI는 높은 점수의 node를 선택해야한다.
*/

/*게임트리 각각의 노드의 구조체*/
typedef struct gamenode {
	int board[SIZE][SIZE];
	Pos pos;
	int val;
	struct gamenode *parent;
	struct gamenode *child[SIZE*SIZE];
}Node;

Node * makeTree(int(*board)[SIZE], int depth); //AI는 판단을 할때마다 현 상태의 board를 인자로 받고 그를 바탕으로 depth 수 만큼 계산한다.
Node * makeSubTree(Node *main, int temp, int depth);	//각 노드의 자식 트리를 만드는 함수
Node * makeNode(int(*board)[SIZE]); //게임트리의 노드를 만든다.
void makeChilde(Node *main, Node *sub, int n); //main노드의 n번째 자식노드를 sub로 한다.
int evalBoard(int(*board)[SIZE], int depth);	//그 보드의 점수 계산