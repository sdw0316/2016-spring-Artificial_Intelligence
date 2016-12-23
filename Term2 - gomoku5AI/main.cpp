#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "board.h"
#include "tree.h"


int main() {
	int board[SIZE][SIZE] = { 0, }; //board에 player1 은 1, player2 는 2로 저장
	int player = P1;			 //플레이어 1부터 시작
	int winner = 0;			//초기 승자는 미정

	while (checkBoard(board, SIZE)) {
		Pos pos;
		char input[3];
		char buffer;
		makeBoard(board, SIZE);

		/*
		player 1은 AI, player 2는 유저
		*/
		if (player == P1) {
			pos = Ai(board);
			/*
			AI가 두는 좌표 확인을 위한 부분
			printf("%d %d\n", pos.x, pos.y);
			system("pause");
			*/
			input[0] = pos.x + 65;
			input[1] = pos.y + 49;
			input[2] = NULL;
		}

		else if (player == P2) {
			printf("플레이어 %d 의 차례입니다. 돌을 둘 곳을 입력하세요 (예: A3) : ", player);
			fgets(input, sizeof(input), stdin);
			scanf("%c", &buffer);

			int x = toupper(input[0]) - 65;
			int y = input[1] - 49;
			pos.x = x;
			pos.y = y;
		}

		if (inputBoard(board, pos, player)) {
			system("cls");
			printf("플레이어 %d 가 %c%c에 돌을 두었습니다.\n", player, toupper(input[0]), input[1]);
			winner = checkWin(board, pos, player);
			if (winner == P1 || winner == P2)
				break;
			player = (player % 2) + 1;
		}
		else {
			system("cls");
			printf("그 곳에는 둘 수 없습니다.(%c%c)\n", toupper(input[0]), input[1]);
		}
	}
	if (winner != 0) {
		makeBoard(board, SIZE);
		printf("player %d의 승리입니다!\n", winner);
		system("pause");
		return 0;
	}

	if (checkBoard(board, SIZE) == FALSE) {
		makeBoard(board, SIZE);
		printf("무승부 입니다!\n");
		system("pause");
		return 0;
	}

}