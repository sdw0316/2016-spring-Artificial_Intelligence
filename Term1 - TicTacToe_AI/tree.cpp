#include "tree.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/*현재 판을 기준으로 depth수 만큼의 수를 내다볼 수 있는 트리 형성*/
Node *makeTree(int(*board)[SIZE], int depth) {
	Node *root = makeNode(board);	//트리의 루트노드는 현재 판의 상태
	int temp = 0;	//현 node의 depth
	root = makeSubTree(root, temp, depth);

	return root;
}

Node *makeSubTree(Node *main, int temp, int depth) {
	temp++;//깊이 증가

	/*만약 목표 깊이까지 노달하면 보드 평가점수 내고, 리턴*/
	if (temp > depth) {
		main->val = evalBoard(main->board, temp-1);

		//printf("깊이 : %d 점수 : %d \n", temp - 1, main->val);
		return main;
	}
	else {
		int count = 0;	//자식노드 개수

		/*
		자식노드 만들기.
		*/
		for (int y = 0; y < SIZE; y++) {
			for (int x = 0; x < SIZE; x++) {
				/*만약 현재 판에 둘 수 있는 경우가 있다면 child node 생성*/
				if (main->board[y][x] == 0) {
					Node *sub = makeNode(main->board);
					int player = 2 - (temp) % 2; //현재 depth가 홀수면 AI 차례, 짝수면 플레이어 차례
					sub->board[y][x] = player;
					sub->pos.x = x; 
					sub->pos.y = y;
					makeChilde(main, sub, count++);	//만들어진 노드는 main노드의 자식노드로

					/*만약 만들어진 보드 상태에서 승패 결정시에는 자식노드 안만듦고 가중치 최대*/
					if (checkWin(sub->board, sub->pos, player)) {
						if (player == P1) {
							sub->val = MAX;
							//printf("깊이 : %d 점수 : %d \n", temp, sub->val);
							continue;
						}
						else if (player == P2) {
							sub->val = MIN;
							//printf("깊이 : %d 점수 : %d \n", temp, sub->val);
							continue;
						}
					}

					sub = makeSubTree(sub, temp, depth);//리프노드가 아니라면 자식노드 생성
				}
			}
		}
		/*
		자식 노드 다 만들었으면 올라오면서 순회하며 점수계산
		자식노드는 총 count-1 개 존재
		현재 깊이(위에서 temp+1했으므로 temp-1)가 짝수면 AI차례니까 큰값을, 홀수면 player 차례니까 작은 값 셀렉트.
		*/
		if ((temp - 1) % 2 == 0) {
			int max = MIN;
			for (int i = 0; i < count; i++) {
				if ((main->child[i]->val)>max)
					max = (main->child[i]->val);

				/*루트와 그 자식들 빼곤 이젠 다 필요 없으니 free*/
				if((temp -1)!=0)
					free(main->child[i]);
			}
			main->val = max;
		}
		else if ((temp - 1) % 2 == 1) {
			int min = MAX;
			for (int i = 0; i < count; i++) {
				if ((main->child[i]->val) < min)
					min = (main->child[i]->val);

				free(main->child[i]);
			}
			main->val = min;
		}
		//printf("깊이 : %d 점수 : %d \n",temp - 1,main->val);
		return main;
	}
}


Node *makeNode(int(*board)[SIZE]) {
	Node *newnode = (Node*)malloc(sizeof(Node));
	memcpy(newnode->board, board, sizeof(int)*SIZE*SIZE);
	newnode->val = 0;
	newnode->parent = NULL;
	for (int i = 0; i < SIZE*SIZE; i++)
		newnode->child[i] = NULL;

	return newnode;
}

void makeChilde(Node *main, Node *sub, int n) {
	main->child[n] = sub;
	sub->parent = main;
}

int evalBoard(int(*board)[SIZE], int depth) {
	int result = 0;
	
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			if (board[y][x] == 0) {
				/*이 부분은 차후 좀 더 수정 필요*/
				/*player1에게 유리한 수 조사*/
				Pos pos = { x,y };
				board[y][x] = P1;
				if (checkWin(board, pos, P1)) {
					/*
					printf("win %d %d\n", x, y);
					*/
					result += 2;
				}	
					/*player2에게 유리한 수 조사*/
				board[y][x] = P2;
				if (checkWin(board, pos, P1)) {
					/*
					printf("lose %d %d\n", x, y);
					*/
					result -= 2;
				}	

				board[y][x] = 0;	//다시 검사를 위해 board 원상태로
				/*
				누군가가 이기는 경우 조사.
				*/
				if (checkWin(board, pos, P1) == P1) {
					result += MAX;
				}
				if (checkWin(board, pos, P2) == P2) {
					result -= MIN;
				}
			}
		}
	}
	/*
	printBoard(board);
	printf("%d\n", result);
	*/
	return result;
}
