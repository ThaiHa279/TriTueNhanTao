#include <stdio.h>
#include <stdlib.h>
#include <stack>

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
int find(State state,stack <Node*> s ){
    while(!s.empty()) {
        if (compare(s.top()->state, state)) {
            return 1;
        }
        s.pop();
    }
    return 0;
}
// Thuat toan theo chieu sau
Node* DFS_Algorithm(State state){
    //khoi tao stack open, close 
    stack<Node*> open, close; 
    // tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    open.push(root);
    //duyet dfs
    while (!open.empty()) {
        // lay dinh trong open => close
        Node* node = open.top();
        open.pop();
        close.push(node);
        //kiem tra node co phai la goal
        if (goalCheck(node->state)) {
            return node;
        }
        // goi cac phep toan tren trang thai
        for (int i= 1; i<=6; i++) {
            State newState; 
            makeNullState(&newState);
            if (call_operation(node->state,&newState,i)) {
                // kiem tra trang thai da co chua
                if (find(newState,open) || find(newState,close)) {
                    continue;
                }
                // chua co them vao open
                Node* newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newState;
                newNode->Parent = node;
                newNode->no_function = i;
                open.push(newNode);
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
// main
int main() {
	State cur_state;
	makeNullState(&cur_state);
	cur_state.Tank[1] = 10;
    Node* p = DFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
}
 
