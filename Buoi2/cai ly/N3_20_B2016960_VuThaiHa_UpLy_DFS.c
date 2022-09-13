#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int a[7]; 
}State; 
int operation(State *state, int idx ){
    if (idx <= 4 && idx >=1 ) {
        for (int i = idx; i <= idx+2; i++) {
            state->a[i] = !state->a[i]; 
        }
        return 1;
    }
    return 0;
}
int call_operation(State cur_state, State *result, int opt) {
    *result = cur_state;
    switch (opt)
    {
    case 1: return operation(result, 1);
    case 2: return operation(result, 2);
    case 3: return operation(result, 3);
    case 4: return operation(result, 4);
    default:
       printf("Error!"); 
    }
}
int goalCheck(State state) {
    for (int i=1; i<=6; i++) {
        if (state.a[i] == 0) return 0;
    }
    return 1;
}
void print_state(State state) {
    printf("( "); 
    for (int i=1; i<=6; i++) 
        printf(" %d ", state.a[i]);
     printf(" ) \n"); 
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
        for (int i= 1; i<=6; i++) {
            if (state1.a[i] != state2.a[i]) return 0; 
        }
        return 1;
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

int find(State state, Stack queue) {
	while (!emptyStack(queue)) {
		if (compare(state, top(queue)->state)) return 1;
		pop(&queue);
	} 
	return 0;
}
// bfs 
Node *DFS_Algorithm(State state) {
	// khai bao queue open , close

	Stack open, close; 
	makeNullStack(&open); 
	makeNullStack(&close); 

	// tao root -> open 
	Node *root = (Node*)malloc(sizeof(Node));
	root->no_function = 0;
	root->state = state; 
	root->Parent = NULL;
	push(root, &open);
	 //duyet 
	while (!emptyStack(open)) {
		// lay node cua open.front
		Node *node = top(open);
		pop(&open); 
		push(node, &close);
		// kt goal
		if (goalCheck(node->state)) return node; 
		// chay cac operator 	
		for (int i = 1; i<=4; i++) {
			State newState;
			if (call_operation(node->state, &newState, i)) {	
				// kt newState da co trong open hoac close 
				if (find(newState, open) || find(newState, close)) 	continue;
				// them vao open 
				Node * newNode = (Node*)malloc(sizeof(Node));
				newNode->no_function = i;
				newNode->state = newState; 
				newNode->Parent = node;
				push(newNode, &open);
			}
		}
	}
}
const char* action[] = {"Frist state","Up ly 1->3" ,"Up ly 2->4", "Up ly 3->5", "Up ly 4->6"};
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
        printf("\nAction %d: %s \n",no_action,action[top(result)->no_function]);
        print_state(top(result)->state);
        no_action++;
        pop(&result);
    }
}
// main
int main() {
    int a[] = {0, 1 , 0, 1, 0 ,1 , 0};
	State cur_state;
    for (int i = 1; i<= 6; i++)
    cur_state.a[i] = a[i];
     Node* p = DFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
}
