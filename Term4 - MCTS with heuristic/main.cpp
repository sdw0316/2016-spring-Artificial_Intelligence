#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "mct.h"

int main() {
	srand(time(NULL));
	State* current = (State*)malloc(sizeof(State));
	Node* root = makeNode(current, 0);
	init(current);	//스테이트 초기화
	int player = P1;			 //플레이어 1부터 시작
	int winner = 0;			//초기 승자는 미정

	Pos pos;
	char input_x;
	int x;
	int y;

	char start[7];
	start[2] = NULL;
	char buffer;
	int startturn = P2;
	int startflag = TRUE;

	fflush(stdout);
	scanf("%s", start);
	scanf("%c", &buffer);

	if (strncmp(start,"START",5) == 0){
		startturn = P1;
		startflag = FALSE;
	}

	while (1) {
		/*AI turn*/
		if (player == startturn){
			int ax, ay;
			root = MCTS(root);
			cpyState(current, root->state);	//MCT로 구해진 best루트 반환

			ax = root->state->pos.x;
			ay = 15 - (root->state->pos.y);
			if (ay > 9) {
				char input[4];//길이 3짜리
				printf("%c%d\n", ax+65,ay);	
			}
			else {
				printf("%c%d\n", ax+65, ay);
			}

			player = (player % 2) + 1;
		}
		else {
			char input[4];
			/*처음에 START이외의 입력*/
			if (startflag) {
				strncpy(input, start, 4);;
				startflag = FALSE;
			}
			else {
				fflush(stdout);
				scanf("%s", input);
				scanf("%c", &buffer);

			}
			/*길이 2*/
			if (input[2] == NULL) {
				x = input[0] - 65;
				y = (input[1] - 48);
			}
			/*길이 3*/
			else {
				x = input[0] - 65;
				y = (10 * (input[1] - 48)) + input[2]-48;
			}
			pos.x = x;
			pos.y = 15 - y;
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
					root = makeNode(current, (root->depth) + 1);
				player = (player % 2) + 1;
			}
		}
	}
}