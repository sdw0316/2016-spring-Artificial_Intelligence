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
	init(current);	//������Ʈ �ʱ�ȭ
	int player = P1;			 //�÷��̾� 1���� ����
	int winner = 0;			//�ʱ� ���ڴ� ����

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
			cpyState(current, root->state);	//MCT�� ������ best��Ʈ ��ȯ

			ax = root->state->pos.x;
			ay = 15 - (root->state->pos.y);
			if (ay > 9) {
				char input[4];//���� 3¥��
				printf("%c%d\n", ax+65,ay);	
			}
			else {
				printf("%c%d\n", ax+65, ay);
			}

			player = (player % 2) + 1;
		}
		else {
			char input[4];
			/*ó���� START�̿��� �Է�*/
			if (startflag) {
				strncpy(input, start, 4);;
				startflag = FALSE;
			}
			else {
				fflush(stdout);
				scanf("%s", input);
				scanf("%c", &buffer);

			}
			/*���� 2*/
			if (input[2] == NULL) {
				x = input[0] - 65;
				y = (input[1] - 48);
			}
			/*���� 3*/
			else {
				x = input[0] - 65;
				y = (10 * (input[1] - 48)) + input[2]-48;
			}
			pos.x = x;
			pos.y = 15 - y;
			if (inputBoard(current, pos, player)) {

				//���� MCT���� ���� action�� �÷��̾ �� ���� ������, ��Ʈ�� �װɷ� �ű�
				//�ƴϸ� ���ο� ��带 ���� �׳��� root��
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