#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "tree.h"


int main() {
	int board[SIZE][SIZE] = { 0, }; //board�� player1 �� 1, player2 �� 2�� ����
	int player = P1;			 //�÷��̾� 1���� ����
	int winner = 0;			//�ʱ� ���ڴ� ����

	while (checkBoard(board, SIZE)) {
		Pos pos;
		char input[3];
		char buffer;
		makeBoard(board, SIZE);

		/*
		player 1�� AI, player 2�� ����
		*/
		if (player == P1) {
			pos = Ai(board);
			/*
			AI�� �δ� ��ǥ Ȯ���� ���� �κ�
			printf("%d %d\n", pos.x, pos.y);
			system("pause");
			*/
			input[0] = pos.x + 65;
			input[1] = pos.y + 49;
			input[2] = NULL;
		}

		else if (player == P2) {
			printf("�÷��̾� %d �� �����Դϴ�. ���� �� ���� �Է��ϼ��� (��: A3) : ", player);
			fgets(input, sizeof(input), stdin);
			scanf("%c", &buffer);

			int x = toupper(input[0]) - 65;
			int y = input[1] - 49;
			pos.x = x;
			pos.y = y;
		}

		if (inputBoard(board, pos, player)) {
			system("cls");
			printf("�÷��̾� %d �� %c%c�� ���� �ξ����ϴ�.\n", player, toupper(input[0]), input[1]);
			winner = checkWin(board, pos, player);
			if (winner == P1 || winner == P2)
				break;
			player = (player % 2) + 1;
		}
		else {
			system("cls");
			printf("�� ������ �� �� �����ϴ�.(%c%c)\n", toupper(input[0]), input[1]);
		}
	}
	if (winner != 0) {
		makeBoard(board, SIZE);
		printf("player %d�� �¸��Դϴ�!\n", winner);
		system("pause");
		return 0;
	}

	if (checkBoard(board, SIZE) == FALSE) {
		makeBoard(board, SIZE);
		printf("���º� �Դϴ�!\n");
		system("pause");
		return 0;
	}

}