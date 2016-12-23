#include "board.h"
#include "mct.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>


int dir[4][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 } }; //승리 체크를 위한 검사 방향
int sample3[5] = { 0,1,1,1,0 };	//놓았을때의 3을 비교하기 위한 셋
int sample3_2[5] = { 0,2,2,2,0 };
int sample4[6] = { 0,1,1,1,1,0 }; //하나 더 놓으면 4가 되는 3을 비교하기 위한 셋
int sample4_2[6] = { 0,2,2,2,2,0 };

void init(State* state) {
	int newboard[SIZE][SIZE] = { 0, };
	memcpy(state->board, newboard, sizeof(int)*SIZE*SIZE);
	state->pos = { 0, 0 };	//초기엔 걍 0,0으로 설정
	state->action[0].x = SIZE / 2;
	state->action[0].y = SIZE / 2;
	state->len = 1;
}

void showState(State* state) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (state->board[i][j] == 0)
				printf("ㅁ");
			else if (state->board[i][j] == 1)
				printf("●");
			else
				printf("○");
		}
		printf("\n");
	}
	printf("Pos : (%d, %d)\nActions : ", state->pos.x, state->pos.y);
	for (int i = 0; i < state->len; i++) {
		printf("(%d %d) ", state->action[i].x, state->action[i].y);
	}
	printf("\n# of actions : %d \n", state->len);
}

void changeState(State* state, Pos pos, int player) {
	int x = pos.x; int y = pos.y;

	state->board[y][x] = player;	//돌 놓고
	state->pos = pos;	//좌표 바꾸고
	/*지금 두는 좌표가 현 action에 있나 검사*/
	for (int i = 0; i < state->len; i++) {
		/*만약 action안에 있는 좌표에 두면 전체 action 하나 줄임*/
		if (pos.x == state->action[i].x && pos.y == state->action[i].y) {
			Pos temp;
			SWAP(state->action[i], state->action[state->len - 1], temp);
			(state->len)--;
		}
	}

	for (int row = 0; row < 4; row++) {
		Pos newpos;
		int ny = y + dir[row][0];
		int nx = x + dir[row][1];
		int flag;
		if (ny >= 0 && ny < SIZE&&nx >= 0 && nx < SIZE) {
			if (state->board[ny][nx] == 0) {
				flag = TRUE;
				newpos = { nx,ny };
				for (int i = 0; i < state->len; i++) {
					if (newpos.x == state->action[i].x && newpos.y == state->action[i].y) {
						flag = FALSE;
						break;
					}
				}
				if (flag)
					state->action[(state->len)++] = newpos;
			}
		}
		ny = y - dir[row][0];
		nx = x - dir[row][1];
		if (ny >= 0 && ny < SIZE&&nx >= 0 && nx < SIZE) {
			if (state->board[ny][nx] == 0) {
				flag = TRUE;
				newpos = { nx,ny };
				for (int i = 0; i < state->len; i++) {
					if (newpos.x == state->action[i].x && newpos.y == state->action[i].y) {
						flag = FALSE;
						break;
					}
				}
				if (flag)
					state->action[(state->len)++] = newpos;
			}
		}
	}
}

void makeBoard(int(*board)[SIZE], int n) {
	for (int i = 0; i <= n; i++) {
		for (int j = 0; j <= n; j++) {
			if (i == 0) {
				if (j == 0)
					printf("   ");
				else
					printf("%c ", 64 + j);
			}
			else {
				if (j == 0)
					printf("%2d", i);
				else {
					if (board[i - 1][j - 1] == P1)
						printf("○");
					else if (board[i - 1][j - 1] == P2)
						printf("●");
					else {
						if (i == 1) {
							if (j == 1)
								printf("┌");
							else if (j == n)
								printf("┐");
							else
								printf("┬");
						}
						else if (i == n) {
							if (j == 1)
								printf("└");
							else if (j == n)
								printf("┘");
							else
								printf("┴");
						}
						else {
							if (j == 1)
								printf("├");
							else if (j == n)
								printf("┤");
							else
								printf("┼");
						}
					}
				}
			}
		}
		printf("\n");
	}
}

/*빈공간 (0)이 있으면 TRUE 아니면 FALSE*/
int checkBoard(State* state) {
	if (state->len == 0)
		return FALSE;
	else
		return TRUE;
}
\

/*바둑판에 돌을 두고 판의 가중치 변경*/
int inputBoard(State* state, Pos pos, int player) {
	int x = pos.x;
	int y = pos.y;
	if (inputChecker(state->board, pos)) {	//먼저 그 자리에 돌 있나 검사
		if (renju(state->board, pos, player))	//그 후 33위반하나 검사
			return FALSE;

		changeState(state, pos, player);//그 좌표 정보를 바탕으로 state의 정보 change
		//showState(state);
		//system("pause");
		return TRUE;
	}
	else {
		return FALSE;
	}
}


/*보드의 해당 위치에 돌을 둘 수 있는가 검사*/
int inputChecker(int(*board)[SIZE], Pos pos) {
	int x = pos.x;
	int y = pos.y;
	if (x<0 || x >= SIZE || y < 0 || y >= SIZE)
		return FALSE;
	if (board[y][x] != 0)
		return FALSE;
	return TRUE;
}

int isEmpty(int(*board)[SIZE]) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (board[i][j] != 0)
				return FALSE;
		}
	}
	return TRUE;
}
/*그 자리에 뒀을때 승리 검사*/
int checkWin(int(*board)[SIZE], Pos pos, int player) {
	for (int row = 0; row < 4; row++) {
		int len = 1;
		len += checkDir(board, pos, row, 1); //정방향 검사
		len += checkDir(board, pos, row, -1);//역방향 검사
		if (len == WINLEN) {
			return player;
		}
	}
	return FALSE;
}

/*방향으로 line 검사*/
int checkDir(int(*board)[SIZE], Pos pos, int row, int set) {
	int result = 0;
	Pos npos = { pos.x + dir[row][1] * set, pos.y + dir[row][0] * set };
	int x = pos.x;
	int y = pos.y;
	int nx = npos.x;
	int ny = npos.y;

	if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
		return 0;
	else {
		if (board[ny][nx] == board[y][x] && board[y][x] != 0) {
			result++;
			result += checkDir(board, npos, row, set);
		}
	}
	return result;
}

/*
tree의 루트노드는 값이 가장 큰 child노드의 value 값을 가지고 있다.
이 값과 같은 value를 가진 child노드를 선택한 후
그 노드에서 새로 놓은 좌표값을 return 한다.
*/

/*
렌주 룰
*/
int renju(int(*board)[SIZE], Pos pos, int player) {
	int num3 = 0; //3의 개수 이놈이 2되면 리턴 True;
	int row;
	for (row = 0; row < 4; row++) {
		int temp = 0;//각 방향마다의 3의 개수 -> 이놈이 딱 1개만 있어야 3으로 인정
					 /*라인마다 3의 개수  파악*/
		temp += checkThree(board, pos, row, -2, player);
		temp += checkThree(board, pos, row, -3, player);
		temp += checkThree(board, pos, row, -1, player);
		if (temp == 0) {
			temp += checkFour(board, pos, row, -1, player);
			temp += checkFour(board, pos, row, 1, player);
		}
		if (temp == 1)
			num3++;
		if (num3 == 2) {
			return TRUE;
		}
	}
	return FALSE;
}

int checkThree(int(*board)[SIZE], Pos pos, int row, int start, int player) {
	int temp = 0;
	int sample[5];

	if (player == P1)
		memcpy(sample, sample3, sizeof(int) * 5);
	else if (player == P2)
		memcpy(sample, sample3_2, sizeof(int) * 5);

	/*현 자리에 뒀을 시 4를 만들수 있는 3 파악*/
	for (int i = start; i <= 4 + start; i++) {
		int ny = pos.y + i*dir[row][0];
		int nx = pos.x + i*dir[row][1];

		if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
			return FALSE;

		if (i == 0) {
			continue;
		}
		if (sample[i - start] != board[ny][nx]) {//만약 이게 다르면 중간에 흰돌이 포함되있거나 막혀있거나 4임
			return 0;
		}
	}
	/*만약 3만들어 지면 그 3의 양 끝에 돌을 뒀을시, 열린 4 만들어 지나 검사*/
	Pos spos = { pos.x + start*dir[row][1], pos.y + (start)*dir[row][0] };
	Pos lpos = { pos.x + (4 + start)*dir[row][1], pos.y + (4 + start)*dir[row][0] };
	return (checkFour(board, spos, row, 1, player) || checkFour(board, lpos, row, -1, player));
}

int checkFour(int(*board)[SIZE], Pos pos, int row, int set, int player) {
	int temp = 0;//한개 0은 취급 ok
	int sample[6];
	/*check하는 starting point, end point*/
	int sx = pos.x - dir[row][1] * set;
	int sy = pos.y - dir[row][0] * set;
	int lx = pos.x + (4 * set)*dir[row][1];
	int ly = pos.y + (4 * set)*dir[row][0];

	if (player == P1)
		memcpy(sample, sample4, sizeof(int) * 6);
	else if (player == P2)
		memcpy(sample, sample4_2, sizeof(int) * 6);

	for (int i = 0; i < 4; i++) {
		int ny = pos.y + dir[row][0] * i*set;
		int nx = pos.x + dir[row][1] * i*set;

		/*보드 바깥까지 나가면 33이 나올 일이 없으므로 false*/
		if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
			return FALSE;

		/*현재 포지션*/
		if (i == 0) {
			continue;
		}
		if (sample[i] != board[ny][nx]) {//만약 이게 다르면 중간에 흰돌이 포함되있거나 막혀있거나 4임
			if (board[ny][nx] == (player % 2 + 1))//적군 돌로 막혀있으면 열린 4 안나오니까 그냥 0 리턴
				return FALSE;
			temp++;
			if (temp < 2)//아니면(0) 한개까지는 인정해 주고 그 외엔 0 리턴
				continue;
			else
				return FALSE;
		}
	}

	/*bound 처리*/
	if (sx < 0 || sx >= SIZE || sy < 0 || sy >= SIZE)
		return FALSE;
	if (lx < 0 || lx >= SIZE || ly < 0 || ly >= SIZE)
		return FALSE;
	/*양 값이 다르면 무조건 FALSE(막혀있다)*/
	if (sample[0] != board[sy][sx]) {
		return FALSE;
	}
	else if (sample[0] != board[ly][lx]) {
		return FALSE;
	}

	if (temp == 1)//하나 둬서 4라는건 무조건 중간에 0하나가 있어야 됩니다.
		return 1;
	else//그게 아니면 그냥 4에요 (3,4)체크가 안되요.
		return 0;
}