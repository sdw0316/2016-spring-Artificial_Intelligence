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
	int player = P1;			 //�÷��̾� 1���� ����
	int winner = 0;			//�ʱ� ���ڴ� ����
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

			result = (double)(clock() - before) / CLOCKS_PER_SEC;//AI������ ��ġ�� �ɸ� �ð�
			cpyState(current, root->state);	//MCT�� ������ best��Ʈ ��ȯ
			system("cls");
			printf("�ɸ��ð��� %5.2f �� �Դϴ�.\n", result);
			printf("�÷��̾� %d �� %c%d�� ���� �ξ����ϴ�.\n", player, toupper(root->state->pos.x+65), root->state->pos.y+1);
			winner = checkWin(current->board, current->pos, P1);
			player = (player % 2) + 1;
		}
		else {
			printf("�÷��̾� %d �� �����Դϴ�. ���� �� ���� �Է��ϼ��� (��: A3) : ", player);
			scanf("%1c%d", &input_x, &y);
			scanf("%c", &buffer);

			x = toupper(input_x) - 65;
			pos.x = x;
			pos.y = y - 1;

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
					root = makeNode(current, 0);
				system("cls");
				printf("�÷��̾� %d �� %c%d�� ���� �ξ����ϴ�.\n", player, toupper(input_x), y);
				winner = checkWin(current->board, pos, player);
				player = (player % 2) + 1;
			}
			else {
				system("cls");
				printf("�� ������ �� �� �����ϴ�.(%c%d)\n", toupper(input_x), y);
			}
		}
		if (winner != 0) {
			break;
		}
	}
	if (winner != 0) {
		makeBoard(current->board, SIZE);
		printf("player %d�� �¸��Դϴ�!\n", winner);
		system("pause");
		return 0;
	}
	else {
		makeBoard(current->board, SIZE);
		printf("���º�\n");
		system("pause");
		return 0;
	}
}