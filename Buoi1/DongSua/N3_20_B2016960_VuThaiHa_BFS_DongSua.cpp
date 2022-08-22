#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>

using namespace std;

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
// tim trang thai
int compare(State state1, State state2) {
    if (state1.Tank[1] == state2.Tank[1] && state1.Tank[2] == state2.Tank[2] && state1.Tank[3] == state2.Tank[3]) {
        return 1;
    } else return 0;
}
int find(State state,queue <Node*> que ){
        while(!que.empty()) {
        	if (compare(que.front()->state, state)) {
				return 1;
			}
			que.pop();
		}
        return 0;
    }
// Thuat toan theo chieu rong
Node* BFS_Algorithm(State state){
	// khoi tao queue open, close
	queue <Node*> open; 
	queue <Node*> close; 
	// tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node)); 
    root->Parent = NULL;
    root->no_function= 0;
    root->state= state; 
    open.push(root); 
	// duyet BFS
	while (!open.empty()){
		Node* node = open.front();
		open.pop();
		if (goalCheck(node->state)) {
			return node;
		}
		for (int i=1; i<=6; i++) {
			State newState; 
			makeNullState(&newState);
			if (call_operation(node->state,&newState,i)){
				if (find(newState,open) || find(newState,close)){
					continue;
				}
				Node* n = (Node*)malloc(sizeof(Node)); 
			    n->Parent = node;
			    n->no_function= i;
			    n->state= newState; 
			    open.push(n);
			} 
		}
	}
	return NULL;
}
// in ket qua
void print_wayToGetGoal(Node* node) {
    stack <Node*> result; 
    while (node->Parent != NULL) {
        result.push(node);
        node = node->Parent; 
    }
    result.push(node);
    // in ra thu tu
    int no_action = 0;
    while (!result.empty()){
        printf("\nAction %d: %s",no_action,action[result.top()->no_function]);
        print(result.top()->state);
        no_action++;
        result.pop();
    }
}
int main() {
	State cur_state;
	makeNullState(&cur_state);
	cur_state.Tank[1] = 10;
    Node* p = BFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
}

