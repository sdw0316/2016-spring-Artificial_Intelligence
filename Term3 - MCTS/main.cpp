#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "mct.h"

int main() {
	srand(time(NULL));
	
	/*freopen("adasd.txt.", "w", stdout);
	State* current = (State*)malloc(sizeof(State));
	init(current);
	Node* root = makeNode(current, 0);
	//root->state->board[0][0] = root->state->board[0][1] = root->state->board[0][2] = 1;
	//root->state->pos.x = 2; root->state->pos.y= 0;
	//makeBoard((root)->state->board, SIZE);
	for (int i = 0; i < 30; i++) {
		switch (i % 2) {
		case 0: printf("black\n"); break;
		case 1: printf("white\n"); break;
		}
		root = MCTS(root);
	}
	printf("\n\nresult \n\n");
	nodeInfo(root);
	
	*/
	
	State* current = (State*)malloc(sizeof(State));
	Node* root = makeNode(current, 0);
	init(current);
	int player = P1;			 //플레이어 1부터 시작
	int winner = 0;			//초기 승자는 미정
	while (checkBoard(current)) {
		Pos pos;
		char input_x;
		int x;
		int y;
		char buffer;
		makeBoard(current->board, SIZE);
		if (player == P1) {
			clock_t before;
			double result;
			before = clock();

			root = MCTS(root); 

			result = (double)(clock() - before) / CLOCKS_PER_SEC;//AI연산을 마치고 걸린 시간
			cpyState(current, root->state);	//MCT로 구해진 best루트 반환
			system("cls");
			printf("걸린시간은 %5.2f 초 입니다.\n", result);
			printf("플레이어 %d 가 %c%d에 돌을 두었습니다.\n", player, toupper(root->state->pos.x+65), root->state->pos.y+1);
			winner = checkWin(current->board, current->pos, P1);
			player = (player % 2) + 1;
		}
		else {
			printf("플레이어 %d 의 차례입니다. 돌을 둘 곳을 입력하세요 (예: A3) : ", player);
			scanf("%1c%d", &input_x, &y);
			scanf("%c", &buffer);

			x = toupper(input_x) - 65;
			pos.x = x;
			pos.y = y - 1;

			if (inputBoard(current, pos, player)) {
				
				//현재 MCT에서 다음 action중 플레이어가 둔 수가 있으면, 루트를 그걸로 옮김
				//아니면 새로운 노드를 만들어서 그놈을 root로
				int flag = FALSE;
				for (int i = 0; i<root->childnum; i++) {
					Node* child = root->child[i];
					if ((child->state->pos.x == current->pos.x) && (child->state->pos.y == current->pos.y)) {
						root = child;
						flag = TRUE;
						break;
					}
				}
				if (flag == FALSE)
					root = makeNode(current, 0);
				system("cls");
				printf("플레이어 %d 가 %c%d에 돌을 두었습니다.\n", player, toupper(input_x), y);
				winner = checkWin(current->board, pos, player);
				player = (player % 2) + 1;
			}
			else {
				system("cls");
				printf("그 곳에는 둘 수 없습니다.(%c%d)\n", toupper(input_x), y);
			}
		}
		if (winner != 0) {
			break;
		}
	}
	if (winner != 0) {
		makeBoard(current->board, SIZE);
		printf("player %d의 승리입니다!\n", winner);
		system("pause");
		return 0;
	}
	else {
		makeBoard(current->board, SIZE);
		printf("무승부\n");
		system("pause");
		return 0;
	}
}