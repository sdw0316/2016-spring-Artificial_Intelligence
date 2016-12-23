#include "board.h"
#include "mct.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>


int dir[4][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 } }; //승리 체크를 위한 검사 방향
/*player 1,2의 3*3 pattern set*/
int pattern3[3][7] = {
	{ 0,0,1,1,1,0,0 },
	{ 2,0,1,1,1,0,0 },
	{ 0,0,1,1,1,0,2 }
};
int pattern4[2][6] = {
	{ 0,1,1,0,1,0 },
	{ 0,1,0,1,1,0 }
};
int pattern3_2[3][7] = {
	{ 0,0,2,2,2,0,0 },
	{ 1,0,2,2,2,0,0 },
	{ 0,0,2,2,2,0,1 }
};
int pattern4_2[2][6] = {
	{ 0,2,2,0,2,0 },
	{ 0,2,0,2,2,0 }
};

int intcmp(int* A, int* B, int n) {
	for (int i = 0; i < n; i++) {
		if (A[i] != B[i])
			return FALSE;
	}
	return TRUE;
}

void init(State* state) {
	int newboard[SIZE][SIZE] = { 0, };
	memcpy(state->board, newboard, sizeof(int)*SIZE*SIZE);
	state->pos = { 0, 0 };	//초기엔 걍 0,0으로 설정
	state->action[0].x = SIZE/2;
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
					printf("%2d", 16-i);
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

/*바둑판에 돌을 두고 판의 가중치 변경*/
int inputBoard(State* state, Pos pos, int player) {
	int x = pos.x;
	int y = pos.y;
	if (inputChecker(state->board, pos)) {	//먼저 그 자리에 돌 있나 검사
		if (renju(state->board, pos, player))	//그 후 33위반하나 검사
			return FALSE;

		changeState(state, pos, player);//그 좌표 정보를 바탕으로 state의 정보 change
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

	board[pos.y][pos.x] = player;//임시로 바둑판 변경
	for (row = 0; row < 4; row++) {
		int flag = FALSE;//각 방향마다의 3의 개수 -> 이놈이 딱 1개만 있어야 3으로 인정

		/*pure three check*/
		for (int i = 0; i < 3; i++) {
			int tsample[7];//바둑판 상에서 match할 부분
			/*인접한 것들이 0이면 이거 볼필요도 없이 break*/
			int ny = pos.y + dir[row][0], nx = pos.x + dir[row][1];
			int ny2 = pos.y - dir[row][0], nx2 = pos.x - dir[row][1];

			/*해당 방향에서 3찾으면 더이상 볼 필요 x*/
			if (flag)
				break;

			/*인접한놈이 다르거나 boundary일시 check 할필요 x*/
			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
				break;
			if (nx2 < 0 || nx2 >= SIZE || ny2 < 0 || ny2 >= SIZE)
				break;
			if (board[ny][nx] != board[pos.y][pos.x] && board[ny2][nx2] != board[pos.y][pos.x])
				break;

			getPattern1(board, pos, tsample, i, row);

			/*pattern set과 비교*/
			for (int j = 0; j < 3; j++) {
				if (player == P1) {
					if (intcmp(tsample, pattern3[j], 7)) {
						flag = TRUE;
						break;
					}
				}
				else {
					if (intcmp(tsample, pattern3_2[j], 7)) {
						flag = TRUE;
						break;
					}
				}

			}
		}

		/*3 못찾으면 pattern 2와 매칭*/
		if (flag == FALSE) {
			for (int i = 0; i < 4; i++) {
				int tsample[6];//바둑판 상에서 match할 부분

				if (flag)
					break;

				getPattern2(board, pos, tsample, i, row);

				/*pattern set과 비교*/
				for (int j = 0; j < 2; j++) {
					if (player == P1) {
						if (intcmp(tsample, pattern4[j], 6)) {
							flag = TRUE;
							break;
						}
					}
					else {
						if (intcmp(tsample, pattern4_2[j], 6)) {
							flag = TRUE;
							break;
						}
					}

				}
			}
		}

		/*요 라인에서 3 발견시 전체 num3 증가*/
		if (flag)
			num3++;

		/*3이 두개만 되도 33이니까 return true*/
		if (num3 == 2) {
			board[pos.y][pos.x] = 0;
			return TRUE;
		}
	}
	board[pos.y][pos.x] = 0;
	return FALSE;
}

void getPattern1(int(*board)[SIZE], Pos pos, int* temp, int set, int row) {
	int sx, sy;

	switch (set) {
		/*base middle*/
	case 0:
		sx = pos.x + (-3)*dir[row][1];
		sy = pos.y + (-3)*dir[row][0];
		break;
		/*base left*/
	case 1:
		sx = pos.x + (-2)*dir[row][1];
		sy = pos.y + (-2)*dir[row][0];
		break;
		/*base right*/
	case 2:
		sx = pos.x + (-4)*dir[row][1];
		sy = pos.y + (-4)*dir[row][0];
		break;
	}
	for (int i = 0; i < 7; i++) {
		int cx = sx + i*dir[row][1];
		int cy = sy + i*dir[row][0];
		/*bound일경우 무조건 막혔으니 그냥 별개의 상대것으로 본다.*/
		if (cx < 0 || cx >= SIZE || cy < 0 || cy >= SIZE)
			temp[i] = 3;
		else
			temp[i] = board[cy][cx];
	}
}

void getPattern2(int(*board)[SIZE], Pos pos, int* temp, int set, int row) {
	int sx, sy;

	switch (set) {
		/*base middle*/
	case 0:
		sx = pos.x + (-2)*dir[row][1];
		sy = pos.y + (-2)*dir[row][0];
		break;
		/*base left*/
	case 1:
		sx = pos.x + (-1)*dir[row][1];
		sy = pos.y + (-1)*dir[row][0];
		break;
		/*base right*/
	case 2:
		sx = pos.x + (-4)*dir[row][1];
		sy = pos.y + (-4)*dir[row][0];
		break;
	case 3:
		sx = pos.x + (-3)*dir[row][1];
		sy = pos.y + (-3)*dir[row][0];
		break;
	}
	for (int i = 0; i < 6; i++) {
		int cx = sx + i*dir[row][1];
		int cy = sy + i*dir[row][0];
		/*bound일경우 무조건 막혔으니 그냥 별개의 상대것으로 본다.*/
		if (cx < 0 || cx >= SIZE || cy < 0 || cy >= SIZE)
			temp[i] = 3;
		else
			temp[i] = board[cy][cx];
	}
}

/*해당 좌표에 놓았을시 가장 긴 연속return*/
int checkLine(int(*board)[SIZE], Pos pos, int player) {
	int max = 1;
	/*네 방향 전부 돌면서 가장 긴거 return 4를 max로 보고 4 이상이면 그냥 무시*/
	for (int row = 0; row < 4; row++) {
		int len = 1;
		int flag = FALSE; //막힘 체크용
		int nx = pos.x;
		int ny = pos.y;
		/*정방향 check*/
		while (1) {
			/*한 방향으로 쭉 탐색*/
			nx += dir[row][1];
			ny += dir[row][0];

			/*bound 탐색*/
			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) {
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}

			//만약 비어있는 칸이면 break
			if (board[ny][nx] == 0)
				break;
			//다른 돌로 막아져 있으면 사실상 1칸 무시하므로 길이 1개 줄이고 break;
			else if (board[ny][nx] == (player % 2 + 1)) {
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}
			//아니면 len하나 늘리고 계속 탐색
			else
				len++;
		}

		nx = pos.x;
		ny = pos.y;
		/*역방향 check*/
		while (1) {
			nx -= dir[row][1];
			ny -= dir[row][0];

			/*bound 탐색*/
			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) {
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}

			//만약 비어있는 칸이면 break
			if (board[ny][nx] == 0)
				break;
			//다른 돌로 막아져 있으면 사실상 1칸 무시하므로 길이 1개 줄이고 break 반대쪽도 막혀있으면 의미가 없으니까 걍 원래길이 리턴;
			else if (board[ny][nx] == (player % 2 + 1)) {
				if (flag) {
					if (len >= 3)
						break;
					else {
						len = 0;
					}
				}
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}
			//아니면 len하나 늘리고 계속 탐색
			else
				len++;
		}
		if (len > max)
			max = len;
	}
	return max;
}