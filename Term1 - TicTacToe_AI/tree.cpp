#include "tree.h"
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

/*���� ���� �������� depth�� ��ŭ�� ���� ���ٺ� �� �ִ� Ʈ�� ����*/
Node *makeTree(int(*board)[SIZE], int depth) {
	Node *root = makeNode(board);	//Ʈ���� ��Ʈ���� ���� ���� ����
	int temp = 0;	//�� node�� depth
	root = makeSubTree(root, temp, depth);

	return root;
}

Node *makeSubTree(Node *main, int temp, int depth) {
	temp++;//���� ����

	/*���� ��ǥ ���̱��� ����ϸ� ���� ������ ����, ����*/
	if (temp > depth) {
		main->val = evalBoard(main->board, temp-1);

		//printf("���� : %d ���� : %d \n", temp - 1, main->val);
		return main;
	}
	else {
		int count = 0;	//�ڽĳ�� ����

		/*
		�ڽĳ�� �����.
		*/
		for (int y = 0; y < SIZE; y++) {
			for (int x = 0; x < SIZE; x++) {
				/*���� ���� �ǿ� �� �� �ִ� ��찡 �ִٸ� child node ����*/
				if (main->board[y][x] == 0) {
					Node *sub = makeNode(main->board);
					int player = 2 - (temp) % 2; //���� depth�� Ȧ���� AI ����, ¦���� �÷��̾� ����
					sub->board[y][x] = player;
					sub->pos.x = x; 
					sub->pos.y = y;
					makeChilde(main, sub, count++);	//������� ���� main����� �ڽĳ���

					/*���� ������� ���� ���¿��� ���� �����ÿ��� �ڽĳ�� �ȸ���� ����ġ �ִ�*/
					if (checkWin(sub->board, sub->pos, player)) {
						if (player == P1) {
							sub->val = MAX;
							//printf("���� : %d ���� : %d \n", temp, sub->val);
							continue;
						}
						else if (player == P2) {
							sub->val = MIN;
							//printf("���� : %d ���� : %d \n", temp, sub->val);
							continue;
						}
					}

					sub = makeSubTree(sub, temp, depth);//������尡 �ƴ϶�� �ڽĳ�� ����
				}
			}
		}
		/*
		�ڽ� ��� �� ��������� �ö���鼭 ��ȸ�ϸ� �������
		�ڽĳ��� �� count-1 �� ����
		���� ����(������ temp+1�����Ƿ� temp-1)�� ¦���� AI���ʴϱ� ū����, Ȧ���� player ���ʴϱ� ���� �� ����Ʈ.
		*/
		if ((temp - 1) % 2 == 0) {
			int max = MIN;
			for (int i = 0; i < count; i++) {
				if ((main->child[i]->val)>max)
					max = (main->child[i]->val);

				/*��Ʈ�� �� �ڽĵ� ���� ���� �� �ʿ� ������ free*/
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
		//printf("���� : %d ���� : %d \n",temp - 1,main->val);
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
				/*�� �κ��� ���� �� �� ���� �ʿ�*/
				/*player1���� ������ �� ����*/
				Pos pos = { x,y };
				board[y][x] = P1;
				if (checkWin(board, pos, P1)) {
					/*
					printf("win %d %d\n", x, y);
					*/
					result += 2;
				}	
					/*player2���� ������ �� ����*/
				board[y][x] = P2;
				if (checkWin(board, pos, P1)) {
					/*
					printf("lose %d %d\n", x, y);
					*/
					result -= 2;
				}	

				board[y][x] = 0;	//�ٽ� �˻縦 ���� board �����·�
				/*
				�������� �̱�� ��� ����.
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
