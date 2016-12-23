#include "board.h"
#include "mct.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>


int dir[4][2] = { { -1,-1 },{ -1,0 },{ -1,1 },{ 0,-1 } }; //�¸� üũ�� ���� �˻� ����
int sample3[5] = { 0,1,1,1,0 };	//���������� 3�� ���ϱ� ���� ��
int sample3_2[5] = { 0,2,2,2,0 };
int sample4[6] = { 0,1,1,1,1,0 }; //�ϳ� �� ������ 4�� �Ǵ� 3�� ���ϱ� ���� ��
int sample4_2[6] = { 0,2,2,2,2,0 };

void init(State* state) {
	int newboard[SIZE][SIZE] = { 0, };
	memcpy(state->board, newboard, sizeof(int)*SIZE*SIZE);
	state->pos = { 0, 0 };	//�ʱ⿣ �� 0,0���� ����
	state->action[0].x = SIZE / 2;
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
					printf("%2d", i);
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
\

/*�ٵ��ǿ� ���� �ΰ� ���� ����ġ ����*/
int inputBoard(State* state, Pos pos, int player) {
	int x = pos.x;
	int y = pos.y;
	if (inputChecker(state->board, pos)) {	//���� �� �ڸ��� �� �ֳ� �˻�
		if (renju(state->board, pos, player))	//�� �� 33�����ϳ� �˻�
			return FALSE;

		changeState(state, pos, player);//�� ��ǥ ������ �������� state�� ���� change
		//showState(state);
		//system("pause");
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
	for (row = 0; row < 4; row++) {
		int temp = 0;//�� ���⸶���� 3�� ���� -> �̳��� �� 1���� �־�� 3���� ����
					 /*���θ��� 3�� ����  �ľ�*/
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

	/*�� �ڸ��� ���� �� 4�� ����� �ִ� 3 �ľ�*/
	for (int i = start; i <= 4 + start; i++) {
		int ny = pos.y + i*dir[row][0];
		int nx = pos.x + i*dir[row][1];

		if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
			return FALSE;

		if (i == 0) {
			continue;
		}
		if (sample[i - start] != board[ny][nx]) {//���� �̰� �ٸ��� �߰��� ���� ���Ե��ְų� �����ְų� 4��
			return 0;
		}
	}
	/*���� 3����� ���� �� 3�� �� ���� ���� ������, ���� 4 ����� ���� �˻�*/
	Pos spos = { pos.x + start*dir[row][1], pos.y + (start)*dir[row][0] };
	Pos lpos = { pos.x + (4 + start)*dir[row][1], pos.y + (4 + start)*dir[row][0] };
	return (checkFour(board, spos, row, 1, player) || checkFour(board, lpos, row, -1, player));
}

int checkFour(int(*board)[SIZE], Pos pos, int row, int set, int player) {
	int temp = 0;//�Ѱ� 0�� ��� ok
	int sample[6];
	/*check�ϴ� starting point, end point*/
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

		/*���� �ٱ����� ������ 33�� ���� ���� �����Ƿ� false*/
		if (nx < 0 || nx >= SIZE || ny < 0 || ny >= SIZE)
			return FALSE;

		/*���� ������*/
		if (i == 0) {
			continue;
		}
		if (sample[i] != board[ny][nx]) {//���� �̰� �ٸ��� �߰��� ���� ���Ե��ְų� �����ְų� 4��
			if (board[ny][nx] == (player % 2 + 1))//���� ���� ���������� ���� 4 �ȳ����ϱ� �׳� 0 ����
				return FALSE;
			temp++;
			if (temp < 2)//�ƴϸ�(0) �Ѱ������� ������ �ְ� �� �ܿ� 0 ����
				continue;
			else
				return FALSE;
		}
	}

	/*bound ó��*/
	if (sx < 0 || sx >= SIZE || sy < 0 || sy >= SIZE)
		return FALSE;
	if (lx < 0 || lx >= SIZE || ly < 0 || ly >= SIZE)
		return FALSE;
	/*�� ���� �ٸ��� ������ FALSE(�����ִ�)*/
	if (sample[0] != board[sy][sx]) {
		return FALSE;
	}
	else if (sample[0] != board[ly][lx]) {
		return FALSE;
	}

	if (temp == 1)//�ϳ� �ּ� 4��°� ������ �߰��� 0�ϳ��� �־�� �˴ϴ�.
		return 1;
	else//�װ� �ƴϸ� �׳� 4���� (3,4)üũ�� �ȵǿ�.
		return 0;
}