#pragma once

#include <vector>

#define TRUE 1
#define FALSE 0
#define P1 1
#define P2 2
#define SIZE 15
#define WINLEN	5
#define SWAP(x,y,t) ((t)=(x),(x)=(y),(y)=(t)) 

using namespace std;

typedef struct _pos {
	int x;
	int y;
} Pos;

/*각 판의 상태 저장하는 state함수*/
typedef struct _state {
	int board[SIZE][SIZE];	//현재 판의 상태
	Pos pos;	//이 판에 새로 둔 좌표
	Pos action[SIZE*SIZE];	//다음에 둘 수들의 좌표
	int len;	//action의 lenth;
} State;

/**********state 관련 함수들**********************/
void init(State* state);	//게임 시작시 state 초기화
void showState(State* state);
void changeState(State* state, Pos pos, int player);

/*********************************게임판 검사**************************/
void makeBoard(int(*board)[SIZE], int n); //현재 바둑판의 상태를 표현
int checkBoard(State* state); //바둑판이 가득 찼는지 검사
int inputBoard(State* state, Pos pos, int player); //바둑판에 돌 놓기
int inputChecker(int(*board)[SIZE], Pos pos); //바둑판에 놓을 돌 위치 검증
int isEmpty(int(*board)[SIZE]);	//바둑판 비어있나 검사
int checkWin(int(*board)[SIZE], Pos pos, int player); //새로 돌 놓은 곳 기준으로 3개연속의 돌이 놓여있는지 검사
int checkDir(int(*board)[SIZE], Pos pos, int row, int set); //checkWin 안에서 동작하는 함수, 한 방향으로 돌 연속으로 놓여있는지 검사

															/**************3*3체크 함수******************/
int renju(int(*board)[SIZE], Pos pos, int player); //player1 차례에 pos에 둔 돌이 금수인지 검사.
void getPattern1(int(*board)[SIZE], Pos pos, int* temp, int set, int row); //보드에서 33패턴 얻어옴
void getPattern2(int(*board)[SIZE], Pos pos, int* temp, int set, int row); //보드에서 열린 3 패턴 얻어옴

																		 /*****************AI heuristic*****************/
int checkLine(int(*board)[SIZE], Pos pos, int player); //해당 라인에 player기준으로 몇개 연속인지 check
