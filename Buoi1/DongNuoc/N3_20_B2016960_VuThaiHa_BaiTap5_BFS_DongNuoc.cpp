#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <stack>

using namespace std;

#define tankCapacity_X 9
#define tankCapacity_Y 4
#define goal 6 
#define Empty 0

typedef struct {
	int x, y;
}State;

typedef struct Node{
	struct Node* Parent;
	State state; 
	int no_function; 
}Node;


const char * action[] = {"Frist state", "pour water full X", "pour water full Y", 
                        "pour water empty X","pour water empty Y", "pour water X to Y", "pour water Y to X"}; 
// Kiem tra luong nuoc co phai muc tieu hay chua
int goalCheck(State state) {
    return (state.x == goal || state.y == goal); 
}
// khoi tao trang thai bat dau
void makeNullState(State * state) {
    state->x = 0;
    state->y = 0;
}
// in trang thai
void printState(State state) {
    printf("\n   X: %d, Y: %d",state.x,state.y);
}
// Ham max min
int max(int  x, int y) {
    return x > y ? x : y;
}
int min(int x, int y) {
    return x < y ? x : y;
}
// Ham hanh dong thay doi trang thai
    int pourWaterFullX(State cur_state,State* result ) {
        if (cur_state.x < tankCapacity_X) {
            result->x = tankCapacity_X;
            result->y = cur_state.y;
            return 1;
        }
        return 0;
    }
    int pourWaterFullY(State cur_state,State* result ) {
        if (cur_state.y < tankCapacity_Y) {
            result->y = tankCapacity_Y;
            result->x = cur_state.x;
            return 1;
        }
        return 0;
    }
    int pourWaterEmptyX(State cur_state,State* result ) {
        if (cur_state.x > 0) {
            result->x = Empty;
            result->y = cur_state.y;
            return 1;
        }
        return 0;
    }
    int pourWaterEmptyY(State cur_state,State* result ) {
        if (cur_state.x > 0) {
            result->y = Empty;
            result->x = cur_state.x;
            return 1;
        }
        return 0;
    }
    int pourWaterXY(State cur_state,State* result ) {
        if (cur_state.x > 0 && cur_state.y < tankCapacity_Y) {
            // luong nuoc binh x - luong nuoc co the vao binh y; th: x > y => x = 0
            result->x = max(cur_state.x - (tankCapacity_Y - cur_state.y), Empty);
            // th: x + y > maxY
            result->y = min(cur_state.x + cur_state.y, tankCapacity_Y);
            return 1;
        }
        return 0;
    }
    int pourWaterYX(State cur_state,State* result ) {
        if (cur_state.y > 0 && cur_state.x < tankCapacity_X) {
            // luong nuoc binh y - luong nuoc co the vao binh y; th: y > x => y = 0
            result->y = max(cur_state.y - (tankCapacity_X - cur_state.x), Empty);
            // th: y + x > maxX
            result->x = min(cur_state.y + cur_state.x, tankCapacity_X);
            return 1;
        }
        return 0;
    }
// Goi cac phep toan tren trang thai
int call_operation(State cur_state, State* result, int option) {
    switch (option)
    {
        case 1: return pourWaterFullX(cur_state,result);
        case 2: return pourWaterFullY(cur_state,result);
        case 3: return pourWaterEmptyX(cur_state,result);
        case 4: return pourWaterEmptyY(cur_state,result);
        case 5: return pourWaterXY(cur_state,result);
        case 6: return pourWaterYX(cur_state,result);
    default:
       printf("Error calls operation");
       return 0;
    }
}
// tim trang thai
    int compare(State state1, State state2) {
        if (state1.x == state2.x && state1.y == state2.y) {
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
        printState(result.top()->state);
        no_action++;
        result.pop();
    }
}
int main() {
    State cur_state = {0,0};
    Node* p = BFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
} 

