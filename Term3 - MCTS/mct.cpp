#include "mct.h"

/***************노드 관련 함수들*********************/
void nodeInfo(Node* node) {
	showState(node->state);
	printf("depth : %d\n", node->depth);
	printf("# of child : %d\n", node->childnum);
	printf("score : %f visit : %f val %f\n", node->score, node->visit, node->val);
}

Node * makeNode(State* state, int depth) {
	Node *newnode = (Node*)malloc(sizeof(Node));

	newnode->state = state;
	newnode->depth = depth;
	newnode->val = 0.0f;
	newnode->score = 0.0f;
	newnode->visit = 0.0f;	//초기에 점수관련은 모두 0 (backprogation하면서 update)
	newnode->unable = FALSE;
	newnode->parent = NULL;	//자식노드 설정 전에는 부모 없음 expanded시에 설정
	newnode->childnum = 0;

	return newnode;
}
void makeChilde(Node *main, Node *sub) {
	main->child[(main->childnum)++] = sub;
	sub->parent = main;	//main을 sub의 부모로
}
void cpyState(State* s1, State* s2) {
	memcpy(s1->board, s2->board, sizeof(int)*SIZE*SIZE);	//s2의 board상태 s1에 복사
	s1->pos = s2->pos;
	for (int i = 0; i < s2->len; i++)
		s1->action[i] = s2->action[i];
	s1->len = s2->len;
}

/**************x트리 관련 함수들*********************/
Node * MCTS(Node* root) {
	int max = MIN;
	Node *best = NULL;	//다음에 둘 수 있는 최적 노드
	/*
	Loop를 계속 돌면서 4단계를 거쳐 트리 형성
	*/	
	for (int i = 0; i < LOOP; i++) {
		//printf("Loop %d\n", i);
		int result;	//playout 결과
		Node *select = selection(root);
		select = expand(select);
		result = playout(select);
		backPro(select, result);
	}
	/*
	자식이 없으면(terminal) 자기 자식 return
	*/
	if (root->childnum == 0)
		return root;
	/*
	자식 있으면 개중에 점수 젤 큰놈 select
	*/

	for (int i = 0; i < root->childnum; i++) {
		Node* child = root->child[i];
		/*만약 이 child가 룰 위반시엔 선택 안함*/
		if (child->unable)
			continue;
		//printf("depth : %d ucb : %f\n", child->depth,ucb);
		if (child->val > max) {
			max = child->val;
			best = child;
		}
	}
	nodeInfo(best);
	return best;
}

Node * selection(Node* node){
	/*만약 fully expanded or terminal 아니면 현재 노드 select*/
	if (notExpand(node)||!(checkBoard(node->state))) {
		return node;
	}
	/*fully expanded시엔 자식중에 expanded 안됬고, 가장 가치가 큰 놈 select*/
	else {
		Node* selected=NULL;
		double max = (double)MIN;	//selection의 척도가 될 ucb점수
		for (int i = 0; i < node->childnum; i++) {
			Node* child = node->child[i];
			/*만약 이 child가 룰 위반시엔 선택 안함*/
			if (child->unable)
				continue;
			/*아니면 ucb 점수 계산해서 가중치 제일 높은놈 select*/
			double ucb;
			ucb = child->val + sqrt((2 * log(node->visit)) / child->visit);
			//printf("depth : %d ucb : %f\n", child->depth,ucb);
			if (ucb > max) {
				max = ucb;
				selected = child;
			}
		}
		/*expand 안될때까지 반복 select*/
		return selection(selected);
	}
}

Node* expand(Node* node) {
	State *prestate = node->state;
	/*만약 이 노드가 terminal node일 시엔 expand안하고 이 노드 반환*/
	if ((!checkBoard(prestate))||checkWin(prestate->board,prestate->pos,P1) || checkWin(prestate->board, prestate->pos, P2)){
		return node;
	}

	Node* newnode;
	State* newstate = (State*)malloc(sizeof(State));
	cpyState(newstate, node->state);
	Pos temp;
	int len = node->state->len - node->childnum;
	int random = (rand() % len);	//action중 expand할 것 하나 선택
	int player = (node->depth % 2) + 1;
	Pos newpos = node->state->action[random];
	SWAP(node->state->action[random], node->state->action[len-1],temp);

	changeState(newstate, newpos, player);
	newnode = makeNode(newstate, (node->depth) + 1);
	makeChilde(node, newnode);
	return newnode;
}

int notExpand(Node* node) {
	//printf("expand function %d %d\n",node->state->len, node->childnum);
	//만약 다음에 행할 action보다 자식노드 수 작으면 notexpanded
	if (node->state->len > node->childnum) {
		return TRUE;
	}
	return FALSE;
}

int playout(Node* node) {
	State *play = (State*)malloc(sizeof(State));
	int next = (node->depth % 2) + 1; //다음 플레이어
	int player = next;	//
	int winner = 0;
	int gamelen = SIZE*SIZE-node->depth; //게임의 진행 상황, 더 일찍 이기면 가중치가 커짐

	cpyState(play, node->state);
	winner = checkWin(play->board, play->pos, (player % 2) + 1);	//먼저 현재 플레이어가 승리 가능한지 check
	/*판이 다 차거나, 중간에 승자 나올때 까지 loop*/

	/*****************************
	default policy가 정교할수록 playout 수도 줄고, value도 의미있어질것 이다
	*****************************/
	while (checkBoard(play)) {
		if (winner != 0)
			break;
		int random = (rand() % (play->len));
		Pos pos = play->action[random];
		/*이 수를 두는게 가능하다면, state change 후 승자 검사*/
		if (inputBoard(play, pos, player)) {
			changeState(play, pos, player);
			winner = checkWin(play->board,pos,player);
		}
		/*아니면 다른곳에 둡니다.*/
		else
			continue;
		player = (player%2)+1;
		//showState(play);
	}
	free(play);


	if (winner == ((next%2)+1))
		return 1;
	else if (winner == next)
		return -1;
	else if (winner == 0)
		return 0;
}

void backPro(Node* node, int result) {
	int point = result;//실제로 더해줄 점수
	while (node != NULL) {
		(node->visit) += 1;
		(node->score) += point;
		point *= -1;
		node->val = (node->score / node->visit);
		node = node->parent;
	}
}