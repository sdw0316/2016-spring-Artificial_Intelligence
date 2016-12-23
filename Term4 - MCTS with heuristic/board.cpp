#include "board.h"
#include "mct.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>


int dir[4][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 } }; //�¸� üũ�� ���� �˻� ����
/*player 1,2�� 3*3 pattern set*/
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
	state->pos = { 0, 0 };	//�ʱ⿣ �� 0,0���� ����
	state->action[0].x = SIZE/2;
	state->action[0].y = SIZE / 2;
	state->len = 1;
}

void showState(State* state) {
	for (int i = 0; i < SIZE; i++) {
		for (int j = 0; j < SIZE; j++) {
			if (state->board[i][j] == 0)
				printf("��");
			else if (state->board[i][j] == 1)
				printf("��");
			else
				printf("��");
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

	state->board[y][x] = player;	//�� ����
	state->pos = pos;	//��ǥ �ٲٰ�
						/*���� �δ� ��ǥ�� �� action�� �ֳ� �˻�*/
	for (int i = 0; i < state->len; i++) {
		/*���� action�ȿ� �ִ� ��ǥ�� �θ� ��ü action �ϳ� ����*/
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
						printf("��");
					else if (board[i - 1][j - 1] == P2)
						printf("��");
					else {
						if (i == 1) {
							if (j == 1)
								printf("��");
							else if (j == n)
								printf("��");
							else
								printf("��");
						}
						else if (i == n) {
							if (j == 1)
								printf("��");
							else if (j == n)
								printf("��");
							else
								printf("��");
						}
						else {
							if (j == 1)
								printf("��");
							else if (j == n)
								printf("��");
							else
								printf("��");
						}
					}
				}
			}
		}
		printf("\n");
	}
}

/*����� (0)�� ������ TRUE �ƴϸ� FALSE*/
int checkBoard(State* state) {
	if (state->len == 0)
		return FALSE;
	else
		return TRUE;
}

/*�ٵ��ǿ� ���� �ΰ� ���� ����ġ ����*/
int inputBoard(State* state, Pos pos, int player) {
	int x = pos.x;
	int y = pos.y;
	if (inputChecker(state->board, pos)) {	//���� �� �ڸ��� �� �ֳ� �˻�
		if (renju(state->board, pos, player))	//�� �� 33�����ϳ� �˻�
			return FALSE;

		changeState(state, pos, player);//�� ��ǥ ������ �������� state�� ���� change
		return TRUE;
	}
	else {
		return FALSE;
	}
}


/*������ �ش� ��ġ�� ���� �� �� �ִ°� �˻�*/
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
/*�� �ڸ��� ������ �¸� �˻�*/
int checkWin(int(*board)[SIZE], Pos pos, int player) {
	for (int row = 0; row < 4; row++) {
		int len = 1;
		len += checkDir(board, pos, row, 1); //������ �˻�
		len += checkDir(board, pos, row, -1);//������ �˻�
		if (len == WINLEN) {
			return player;
		}
	}
	return FALSE;
}

/*�������� line �˻�*/
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
tree�� ��Ʈ���� ���� ���� ū child����� value ���� ������ �ִ�.
�� ���� ���� value�� ���� child��带 ������ ��
�� ��忡�� ���� ���� ��ǥ���� return �Ѵ�.
*/

/*
���� ��
*/
int renju(int(*board)[SIZE], Pos pos, int player) {
	int num3 = 0; //3�� ���� �̳��� 2�Ǹ� ���� True;
	int row;

	board[pos.y][pos.x] = player;//�ӽ÷� �ٵ��� ����
	for (row = 0; row < 4; row++) {
		int flag = FALSE;//�� ���⸶���� 3�� ���� -> �̳��� �� 1���� �־�� 3���� ����

		/*pure three check*/
		for (int i = 0; i < 3; i++) {
			int tsample[7];//�ٵ��� �󿡼� match�� �κ�
			/*������ �͵��� 0�̸� �̰� ���ʿ䵵 ���� break*/
			int ny = pos.y + dir[row][0], nx = pos.x + dir[row][1];
			int ny2 = pos.y - dir[row][0], nx2 = pos.x - dir[row][1];

			/*�ش� ���⿡�� 3ã���� ���̻� �� �ʿ� x*/
			if (flag)
				break;

			/*�����ѳ��� �ٸ��ų� boundary�Ͻ� check ���ʿ� x*/
			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
				break;
			if (nx2 < 0 || nx2 >= SIZE || ny2 < 0 || ny2 >= SIZE)
				break;
			if (board[ny][nx] != board[pos.y][pos.x] && board[ny2][nx2] != board[pos.y][pos.x])
				break;

			getPattern1(board, pos, tsample, i, row);

			/*pattern set�� ��*/
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

		/*3 ��ã���� pattern 2�� ��Ī*/
		if (flag == FALSE) {
			for (int i = 0; i < 4; i++) {
				int tsample[6];//�ٵ��� �󿡼� match�� �κ�

				if (flag)
					break;

				getPattern2(board, pos, tsample, i, row);

				/*pattern set�� ��*/
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

		/*�� ���ο��� 3 �߽߰� ��ü num3 ����*/
		if (flag)
			num3++;

		/*3�� �ΰ��� �ǵ� 33�̴ϱ� return true*/
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
		/*bound�ϰ�� ������ �������� �׳� ������ �������� ����.*/
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
		/*bound�ϰ�� ������ �������� �׳� ������ �������� ����.*/
		if (cx < 0 || cx >= SIZE || cy < 0 || cy >= SIZE)
			temp[i] = 3;
		else
			temp[i] = board[cy][cx];
	}
}

/*�ش� ��ǥ�� �������� ���� �� ����return*/
int checkLine(int(*board)[SIZE], Pos pos, int player) {
	int max = 1;
	/*�� ���� ���� ���鼭 ���� ��� return 4�� max�� ���� 4 �̻��̸� �׳� ����*/
	for (int row = 0; row < 4; row++) {
		int len = 1;
		int flag = FALSE; //���� üũ��
		int nx = pos.x;
		int ny = pos.y;
		/*������ check*/
		while (1) {
			/*�� �������� �� Ž��*/
			nx += dir[row][1];
			ny += dir[row][0];

			/*bound Ž��*/
			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) {
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}

			//���� ����ִ� ĭ�̸� break
			if (board[ny][nx] == 0)
				break;
			//�ٸ� ���� ������ ������ ��ǻ� 1ĭ �����ϹǷ� ���� 1�� ���̰� break;
			else if (board[ny][nx] == (player % 2 + 1)) {
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}
			//�ƴϸ� len�ϳ� �ø��� ��� Ž��
			else
				len++;
		}

		nx = pos.x;
		ny = pos.y;
		/*������ check*/
		while (1) {
			nx -= dir[row][1];
			ny -= dir[row][0];

			/*bound Ž��*/
			if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE) {
				if (!flag) {
					len--;
					flag = TRUE;
				}
				break;
			}

			//���� ����ִ� ĭ�̸� break
			if (board[ny][nx] == 0)
				break;
			//�ٸ� ���� ������ ������ ��ǻ� 1ĭ �����ϹǷ� ���� 1�� ���̰� break �ݴ��ʵ� ���������� �ǹ̰� �����ϱ� �� �������� ����;
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
			//�ƴϸ� len�ϳ� �ø��� ��� Ž��
			else
				len++;
		}
		if (len > max)
			max = len;
	}
	return max;
}