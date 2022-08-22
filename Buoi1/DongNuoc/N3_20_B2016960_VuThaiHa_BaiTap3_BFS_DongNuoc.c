#include <stdio.h>
#include <stdlib.h>
// dinh nghia 
#define TankCapacity_X 9 
#define TankCapacity_Y 4
#define goal 6
#define Empty 0 
#define MaxLength 100
const char * action[] = {"Frist state", "pour water full X", "pour water full Y", 
                        "pour water empty X","pour water empty Y", "pour water X to Y", "pour water Y to X"}; 
// khai bao state
typedef struct {
	int x,y; 
}State;
// liet ke operator + print
int PourWaterEmpty_X(State cur_state, State *result){
	if (cur_state.x > 0) {
		result->x = 0;
		result->y = cur_state.y; 
		return 1;
	}
	return 0;
}
int PourWaterEmpty_Y(State cur_state, State *result){
	if (cur_state.y > 0) {
		result->x = cur_state.x; 
		result->y = 0;
		return 1;
	}
	return 0;
}
int PourWaterFull_X(State cur_state, State *result){
	if (cur_state.x < TankCapacity_X) {
		result->x = TankCapacity_X;
		result->y = cur_state.y;
		return 1;
	}
	return 0;
}
int PourWaterFull_Y(State cur_state, State *result){
	if (cur_state.y < TankCapacity_Y) {
		result->x = cur_state.x;
		result->y = TankCapacity_Y;
		return 1;
	}
	return 0;
}
int max(int a, int b) {
	return a > b ? a : b; 
}
int min(int a, int b) {
	return a < b ? a : b; 
}
int PourWaterXY(State cur_state, State *result){
	if (cur_state.x > 0 && cur_state.y < TankCapacity_Y) {
		result->x = max(cur_state.x - (TankCapacity_Y - cur_state.y) , 0); 
		result->y = min(cur_state.x + cur_state.y ,TankCapacity_Y);
		return 1;
	}
	return 0;
}
int PourWaterYX(State cur_state, State *result){
	if (cur_state.y > 0 && cur_state.x < TankCapacity_X) {
		result->y = max(cur_state.y - (TankCapacity_X - cur_state.x) , 0); 
		result->x = min(cur_state.x + cur_state.y ,TankCapacity_X);
		return 1;
	}
	return 0;
}
int goalCheck(State state) {
    return (state.x == goal || state.y == goal); 
}
void print_state(State state) {
	printf("  X: %d --- Y: %d \n", state.x, state.y);
}
int call_operator(State cur_state, State *result, int opt){
	switch (opt) {
		case 1: return PourWaterFull_X(cur_state, result); 
		case 2: return PourWaterFull_Y(cur_state, result); 
		case 3: return PourWaterEmpty_X(cur_state, result); 
		case 4: return PourWaterEmpty_Y(cur_state, result); 
		case 5: return PourWaterXY(cur_state, result); 
		case 6: return PourWaterYX(cur_state, result); 	
		default: {
			printf("Invaild operation call!");
			return 0;
		}
	} 
}
// khai bao node 
typedef struct Node{
	int no_func; 
	State state; 
	struct Node* Parent; 
} Node; 
// khai bao queue 
typedef struct {
	Node* Elements[MaxLength]; 
	int front, back; 
}Queue;

void makeNullQueue(Queue *queue) {
	queue->front = -1;
	queue->back = -1; 
}
int fullQueue(Queue queue) {
	return  ((queue.back-queue.front+1)%MaxLength == 0);
}
int emptyQueue(Queue queue) {
	return queue.front == -1;
}
void popQueue(Queue *queue) {
	if (!emptyQueue(*queue)) {
		if (queue->front == queue->back ) {
			makeNullQueue(queue);
		} else queue->front = (queue->front+1)%MaxLength;
	} else {
		printf("Error Queue is Empty!");
	} 
}
void pushQueue(Node* node,Queue *queue) {
	if (!fullQueue(*queue)) {
		if (emptyQueue(*queue)) {
			queue->front = 0;
		}
		queue->back = (queue->back + 1) % MaxLength;
		queue->Elements[queue->back] = node;
	} else {
		printf("Error Queue is Full!");
	}
}
Node* front(Queue queue) {
	if (!emptyQueue(queue)) {
		return queue.Elements[queue.front];
	} else return NULL;
}
int compareState(State a, State b) {
	return (a.x == b.x && a.y == b.y) ? 1 : 0; 
}
int find(State state, Queue queue) {
	while (!emptyQueue(queue)) {
		if (compareState(state, front(queue)->state)) return 1;
		popQueue(&queue);
	} 
	return 0;
}
// in ket qua
// stack
typedef struct{
    Node* Elements[MaxLength];
    int top_idx;
}Stack;
// khoi tao ngan xep rong
void makeNullStack(Stack *stack ) {
    stack->top_idx = MaxLength;
}
//kiem tra ngan xep co rong khong
int emptyStack(Stack stack) {
    return stack.top_idx == MaxLength;
}
//kiem tra ngan xep co day khong
int fullStack(Stack stack) {
    return stack.top_idx == 0;
}
//tra ve pt tren dinh ngan xep 
Node* top(Stack stack) {
    if (!emptyStack(stack)) {
        return stack.Elements[stack.top_idx];
    }
    return NULL;
}
// xoa pt tai dinh ngan xep
void pop(Stack * stack) {
    if (!emptyStack(*stack)) {
        stack->top_idx++;
    }
    else printf("Error! Stack is empty!");
}
// dua pt vao ngan xep
void push(Node *x, Stack *stack) {
    if (fullStack(*stack)) 
        printf("Error! Stack is full.");
    else {
        stack->top_idx -= 1;
        stack->Elements[stack->top_idx] = x;
    }
}
void print_wayToGetGoal(Node* node) {
    Stack result; 
    makeNullStack(&result);
    while (node->Parent != NULL) {
        push(node,&result);
        node = node->Parent; 
    }
    push(node, &result);
    // in ra thu tu
    int no_action = 0;
    while (!emptyStack(result)){
        printf("\nAction %d: %s",no_action,action[top(result)->no_func]);
        print_state(top(result)->state);
        no_action++;
        pop(&result);
    }
}
// bfs 
Node *BFS_Algorithm(State state) {
	// khai bao queue open , close
	Queue open, close; 
	makeNullQueue(&open); 
	makeNullQueue(&close); 
	// tao root -> open 
	Node *root = (Node*)malloc(sizeof(Node));
	root->no_func = 0;
	root->state = state; 
	root->Parent = NULL;
	pushQueue(root, &open);

	// duyet 
	while (!emptyQueue(open)) {
		// lay node cua open.front
		Node *node = front(open);
		popQueue(&open); 
		pushQueue(node, &close);
		// kt goal
		if (goalCheck(node->state)) return node; 
		// chay cac operator 	
		for (int i = 1; i<=6; i++) {
			State newState;
			if (call_operator(node->state, &newState, i)) {	
				// kt newState da co trong open hoac close 
				if (find(newState, open) || find(newState, close)) 	continue;
				// them vao open 
				Node * newNode = (Node*)malloc(sizeof(Node));
				newNode->no_func = i;
				newNode->state = newState; 
				newNode->Parent = node;
				pushQueue(newNode, &open);
			}
		}
	}
}
// main
int main() {
    State cur_state = {0,0};
    Node* p = BFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
} 

