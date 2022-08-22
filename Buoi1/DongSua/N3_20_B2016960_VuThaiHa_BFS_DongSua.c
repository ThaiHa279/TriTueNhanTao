#include <stdio.h>
#include <stdlib.h>

#define goal 8
const int TankCapacity[4] = {0, 10, 5, 6}; 

const char * action[] = {"Frist state", "Transfer X to Y", "Transfer X to Z", "Transfer Y to X", 
                        "Transfer Y to Z", "Transfer Z to X", "Transfer Z to Y"}; 
int max(int x, int y) {
	return (x > y) ? x : y;
}
int min(int x, int y) {
	return (x < y) ? x : y;
}
// khai bao state
typedef struct {
	int Tank[4]; 
} State;
void makeNullState(State* state) {
	state->Tank[0] = 0;
	state->Tank[1] = 0;
	state->Tank[2] = 0;
	state->Tank[3] = 0;
};
int goalCheck(State state) {
	return ( state.Tank[1] == goal || state.Tank[2] == goal || state.Tank[3] == goal); 
}
// operation transfer a to b
int transfer(State cur_state, State *result, int x,int y, int z) {
	if (cur_state.Tank[x] > 0 && cur_state.Tank[y] < TankCapacity[y] ) {
		result->Tank[x] = max(cur_state.Tank[x] - (TankCapacity[y] - cur_state.Tank[y]), 0); 
 		result->Tank[y] = min(cur_state.Tank[x] + cur_state.Tank[y], TankCapacity[y]) ;
		result->Tank[z] = cur_state.Tank[z]; 
		return 1;
 	}
	return 0; 
}
int call_operation(State cur_state, State *result, int opt) {
	switch(opt) {
		case 1: return transfer(cur_state, result, 1, 2, 3); 
		case 2: return transfer(cur_state, result, 1, 3, 2);
		case 3: return transfer(cur_state, result, 2, 1, 3);
		case 4: return transfer(cur_state, result, 2, 3, 1);
		case 5: return transfer(cur_state, result, 3, 1, 2);
		case 6: return transfer(cur_state, result, 3, 2, 1);
		default: 
			printf("Ivaild call operation\n"); 
		return 0; 
	}
}
void print(State state) { 
	printf("\n  X: %d ---- Y: %d ---- Z: %d \n", state.Tank[1], state.Tank[2], state.Tank[3]); 
}
// khai bao node
typedef struct Node {
    State state; 
    struct Node* Parent;
    int no_function;
}Node;
// khai bao stack
#define MaxLength 100
typedef struct{
    Node* Elements[MaxLength];
    int top_idx;
}Stack;
void makeNullStack(Stack *stack ) {
	stack->top_idx = MaxLength;
}
int emptyStack(Stack stack) {
	return stack.top_idx == MaxLength;
}
int fullStack(Stack stack) {
	return stack.top_idx == 0;
}
Node* top(Stack stack) {
	if (!emptyStack(stack)) {
		return stack.Elements[stack.top_idx];
	}
	return NULL;
}
void pop(Stack * stack) {
	if (!emptyStack(*stack)) {
		stack->top_idx++;
	}
	else printf("Error! Stack is empty!");
}
void push(Node *x, Stack *stack) {
	if (fullStack(*stack)) 
		printf("Error! Stack is full.");
	else {
		stack->top_idx -= 1;
		stack->Elements[stack->top_idx] = x;
	}
}
int compare(State state1, State state2) {
        if (state1.Tank[1] == state2.Tank[1] && state1.Tank[2] == state2.Tank[2] && state1.Tank[3] == state2.Tank[3]) {
            return 1;
        } else return 0;
    }
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

int find(State state, Queue queue) {
	while (!emptyQueue(queue)) {
		if (compare(state, front(queue)->state)) return 1;
		popQueue(&queue);
	} 
	return 0;
}
// bfs 
Node *BFS_Algorithm(State state) {
	// khai bao queue open , close
	Queue open, close; 
	makeNullQueue(&open); 
	makeNullQueue(&close); 
	// tao root -> open 
	Node *root = (Node*)malloc(sizeof(Node));
	root->no_function = 0;
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
			if (call_operation(node->state, &newState, i)) {	
				// kt newState da co trong open hoac close 
				if (find(newState, open) || find(newState, close)) 	continue;
				// them vao open 
				Node * newNode = (Node*)malloc(sizeof(Node));
				newNode->no_function = i;
				newNode->state = newState; 
				newNode->Parent = node;
				pushQueue(newNode, &open);
			}
		}
	}
}
// in ket qua
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
        printf("\nAction %d: %s",no_action,action[top(result)->no_function]);
        print(top(result)->state);
        no_action++;
        pop(&result);
    }
}
// main
int main() {
	State cur_state;
	makeNullState(&cur_state);
	cur_state.Tank[1] = 10;
    Node* p = BFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
}
 
