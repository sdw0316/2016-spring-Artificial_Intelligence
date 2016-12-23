#include "mct.h"

/***************��� ���� �Լ���*********************/
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
	newnode->visit = 0.0f;	//�ʱ⿡ ���������� ��� 0 (backprogation�ϸ鼭 update)
	newnode->unable = FALSE;
	newnode->terminal = FALSE;
	newnode->parent = NULL;	//�ڽĳ�� ���� ������ �θ� ���� expanded�ÿ� ����
	newnode->childnum = 0;

	return newnode;
}
void makeChild(Node *main, Node *sub) {
	main->child[(main->childnum)++] = sub;
	sub->parent = main;	//main�� sub�� �θ��
}
void cpyState(State* s1, State* s2) {
	memcpy(s1->board, s2->board, sizeof(int)*SIZE*SIZE);	//s2�� board���� s1�� ����
	s1->pos = s2->pos;
	for (int i = 0; i < s2->len; i++)
		s1->action[i] = s2->action[i];
	s1->len = s2->len;
}

/**************Ʈ�� ���� �Լ���*********************/
Node * MCTS(Node* root) {
	int max = MIN;
	int loop = 0;
	Node *best = NULL;	//������ �� �� �ִ� ���� ���
						/*
						Loop�� ��� ���鼭 4�ܰ踦 ���� Ʈ�� ����
						*/
	clock_t before;
	before = clock();

	/***********�ð� ����***************/
	while (((double)(clock() - before) / CLOCKS_PER_SEC) < LIMIT) {
		int result;	//playout ���
		Node *select = selection(root);
		/*terminal�� �ƴϸ� expand*/
		if (select->terminal == FALSE)
			select = expand(select);
		result = playout(select);
		backPro(select, result);
		loop++;
	}
	/*
	�ڽ��� ������(terminal) �ڱ� �ڽ� return
	*/
	if (root->childnum == 0)
		return root;
	/*
	�ڽ� ������ ���߿� ���� �� ū�� select
	defence�� attack�� �������� ���̽��� ���� ��� ���� NULL�� �ƴϸ� �̰� ����
	*/

	Node *defence, *attack;
	defence = NULL;
	attack = NULL;
	for (int i = 0; i < root->childnum; i++) {
		Node* child = root->child[i];
		State* cur = child->state;
		int player = (root->depth % 2) + 1;
		int next = (player % 2) + 1;

		/*���� �� child�� �� ���ݽÿ� ���� ���ϰ� ���� child Ȯ��*/
		if (child->unable)
			continue;

		/*
		���� ���̽� (�޸���ƽ)
		���� �ڽ��� terminal�̶�� �¸��̹Ƿ� �װ� ����
		*/
		if (child->terminal)
			return child;
		else if (checkWin(cur->board, cur->pos, next) == next) {
			return child;
		}

		/*���� �� �ڸ��� ��밡 ������ 4�� �Ǹ� ���ƾ� �ϹǷ� �װ��� temp�� ����*/
		else if (checkLine(cur->board, cur->pos, next) >= 4) {
			defence = child;
		}
		/*���� �� �ڸ��� ���� ������ 4�� �Ǹ� �����̴ϱ� �� �� node ����*/
		else if (checkLine(cur->board, cur->pos, player) >= 3) {
			attack = child;
		}

		//printf("depth : %d ucb : %f\n", child->depth,ucb);
		else if (child->val > max) {
			max = child->val;
			best = child;
		}
	}
	if (defence != NULL)
		return defence;
	else if (attack != NULL)
		return attack;
	else
		return best;
}

Node * selection(Node* node) {
	/*���� fully expanded or terminal �ƴϸ� ���� ��� select*/
	if (notExpand(node)) {
		return node;
	}
	/*fully expanded�ÿ� �ڽ��߿� expanded �ȉ��, ���� ��ġ�� ū �� select*/
	else {
		Node* selected = NULL;
		double max = (double)MIN;	//selection�� ô���� �� ucb����
		for (int i = 0; i < node->childnum; i++) {
			Node* child = node->child[i];
			/*���� �� child�� �� ���ݽÿ� ���� ����*/
			if (child->unable)
				continue;
			/*�ƴϸ� ucb ���� ����ؼ� ����ġ ���� ������ select*/
			double ucb;
			ucb = child->val + 0.7*sqrt((2 * log(node->visit)) / child->visit);
			//printf("depth : %d ucb : %f\n", child->depth,ucb);
			if (ucb > max) {
				max = ucb;
				selected = child;
			}
		}
		/*expand �ȵɶ����� �ݺ� select*/
		return selection(selected);
	}
}

Node* expand(Node* node) {
	State *prestate = node->state;
	/*���� �� ��尡 terminal node�� �ÿ� state�� terminal�� �ٲٰ� �� ��� ��ȯ*/
	if ((!checkBoard(prestate)) || checkWin(prestate->board, prestate->pos, P1) || checkWin(prestate->board, prestate->pos, P2)) {
		node->terminal = TRUE;
		return node;
	}

	Node* newnode;
	State* newstate = (State*)malloc(sizeof(State));
	cpyState(newstate, node->state);
	Pos temp;
	int len = node->state->len - node->childnum;	//������ expand�� �� ���� = ���� �� action - �̹� ������� �ڽ� ��
	int random = (rand() % len);	//action�� expand�� �� �ϳ� ����
	int player = (node->depth % 2) + 1;
	Pos newpos = node->state->action[random];
	SWAP(node->state->action[random], node->state->action[len - 1], temp);

	changeState(newstate, newpos, player);
	newnode = makeNode(newstate, (node->depth) + 1);

	/*���� ���� ������� node�� 3*3 ���ݽÿ� �� ���� unable*/
	if (renju(prestate->board, newpos, player)) {
		newnode->unable = TRUE;
	}

	makeChild(node, newnode);
	return newnode;
}

int notExpand(Node* node) {
	//printf("expand function %d %d\n",node->state->len, node->childnum);
	//���� ������ ���� action���� �ڽĳ�� �� ������ notexpanded
	if (node->state->len > node->childnum) {
		return TRUE;
	}
	return FALSE;
}

int playout(Node* node) {
	State *play = (State*)malloc(sizeof(State));
	int next = (node->depth % 2) + 1; //���� �÷��̾�
	int player = next;	//
	int winner = 0;

	cpyState(play, node->state);
	winner = checkWin(play->board, play->pos, (player % 2) + 1);	//���� ���� �÷��̾ �¸� �������� check
																	/*���� �� ���ų�, �߰��� ���� ���ö� ���� loop*/

																	/*****************************
																	default policy�� �����Ҽ��� playout ���� �ٰ�, value�� �ǹ��־����� �̴�
																	*****************************/
	while (checkBoard(play)) {
		if (winner != 0)
			break;
		int random = (rand() % (play->len));
		Pos pos = play->action[random];

		/*�� ���� �δ°� �����ϴٸ�, state change �� ���� �˻�*/
		changeState(play, pos, player);
		winner = checkWin(play->board, pos, player);

		player = (player % 2) + 1;
		//showState(play);
	}
	free(play);

	if (winner == ((next % 2) + 1))
		return 1;
	else if (winner == next)
		return -1;
	else if (winner == 0)
		return 0;
}

void backPro(Node* node, int result) {
	int point = result;//������ ������ ����
	while (node != NULL) {
		(node->visit) += 1;
		(node->score) += point;
		point *= -1;
		node->val = (node->score / node->visit);
		node = node->parent;
	}
}