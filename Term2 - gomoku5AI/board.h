#pragma once

#define TRUE 1
#define FALSE 0
#define P1 1
#define P2 2
#define SIZE 5
#define WINLEN 5

typedef struct _pos {
	int x;
	int y;
} Pos;

void makeBoard(int(*board)[SIZE], int n); //현재 바둑판의 상태를 표현
void printBoard(int(*board)[SIZE]);	//바둑판 출력
int checkBoard(int(*board)[SIZE], int n); //바둑판이 가득 찼는지 검사
int inputBoard(int(*board)[SIZE], Pos pos, int player); //바둑판에 돌 놓기
int inputChecker(int(*board)[SIZE], Pos pos); //바둑판에 놓을 돌 위치 검증
int isEmpty(int(*board)[SIZE]);	//바둑판 비어있나 검사
/*
아래 두 함수는 tree에서도 사용할 것이기에 구조체가 아니고 x,y로 좌표 받음
*/
int checkWin(int(*board)[SIZE], Pos pos, int player); //새로 돌 놓은 곳 기준으로 3개연속의 돌이 놓여있는지 검사
int checkDir(int(*board)[SIZE], Pos pos, int row, int set); //checkWin 안에서 동작하는 함수, 한 방향으로 돌 연속으로 놓여있는지 검사

Pos Ai(int(*board)[SIZE]);	//현재 board 입력받고 놓아야 할 좌표 return

/*
2주차 추가 함수
*/
int renju(int(*board)[SIZE], Pos pos); //player1 차례에 pos에 둔 돌이 금수인지 검사.
int checkThree(int(*board)[SIZE], Pos pos, int row, int start); //열린 3인지 판단.
int checkFour(int(*board)[SIZE], Pos pos, int row, int set); //열린 4판단

int checkLine(int(*board)[SIZE], Pos pos, int num);//한 라인에서 돌 수 검사(checkwin의 업그레이드 버전) 휴리스틱 함수에서 사용