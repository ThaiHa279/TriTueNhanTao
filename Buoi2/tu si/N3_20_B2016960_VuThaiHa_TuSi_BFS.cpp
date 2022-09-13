#include <stdio.h>
#include <stdlib.h>
#include <stack>
#include <queue>
using namespace std; 
#define Max 3
typedef struct State
{
    int quy,nguoi;
    char ab;  
} State;
int checkState(State state) {
    return (state.nguoi >= state.quy || state.quy == 0 || state.nguoi == 0);
}

int Move1Monter(State *state) {
    if (state->ab == 'A') {
        if (state->quy >= 1) {
            state->quy--; 
            state->ab = 'B'; 
            if (!checkState(*state)) return 0;
            return 1;
        }
        return 0; 
    } 
    if (state->quy < Max) {
        state->quy++; 
        state->ab = 'A'; 
            if (!checkState(*state)) return 0;
        return 1;
    }
    return 0; 
}

int Move2Monter(State *state) {
    if (state->ab == 'A') {
        if (state->quy >= 2) {
            state->quy-=2; 
            state->ab = 'B'; 
            if (!checkState(*state)) return 0;
            return 1;
        }
        return 0; 
    } 
    if (state->quy < Max-1) {
        state->quy+=2; 
        state->ab = 'A'; 
            if (!checkState(*state)) return 0;
        return 1;
    }
    return 0; 

}
int Move1Human(State *state) {
    if (state->ab == 'A') {
        if (state->nguoi >= 1) {
            state->nguoi--; 
            state->ab = 'B'; 
            if (!checkState(*state)) return 0;
            return 1;
        }
        return 0; 
    } 
    if (state->nguoi < Max) {
        state->ab = 'A'; 
        state->nguoi++;
            if (!checkState(*state)) return 0;
        return 1;
    }
    return 0; 
}
int Move2Human(State *state) {
   if (state->ab == 'A') {
        if (state->nguoi >= 2) {
            state->nguoi-=2; 
            state->ab = 'B'; 
            if (!checkState(*state)) return 0;
            return 1;
        }
        return 0; 
    } 
    if (state->nguoi < Max-1) {
        state->ab = 'A'; 
        state->nguoi+=2;   
            if (!checkState(*state)) return 0;
        return 1;
    }
    return 0; 

}
int Move1Human1Monter(State *state) {
    if (state->ab == 'A') {
        if (state->nguoi >= 1 && state->quy >= 1) {
            state->nguoi--; 
            state->quy--; 
            state->ab = 'B'; 
            if (!checkState(*state)) return 0;
            return 1;
        }
        return 0; 
    } 
    if (state->nguoi < Max && state->quy < Max) {
            state->nguoi++; 
            state->ab = 'A'; 
            state->quy++; 
            
            if (!checkState(*state)) return 0;
            return 1;
        }
        return 0;
}
void print_state(State state){
    printf(" Nguoi bo A: %d  -----  Quy bo A: %d -- thuyen bo %c\n",state.nguoi,state.quy,state.ab);
}
int goalCheck(State state) {
    return (state.nguoi == 0 && state.quy == 0);
}
int call_operation(State cur_state, State *state, int opt) {
    *state = cur_state;
    switch (opt)
    {
    case 1: return Move1Monter(state); 
    case 2: return Move2Monter(state); 
    case 3: return Move1Human(state); 
    case 4: return Move2Human(state); 
    case 5: return Move1Human1Monter(state); 
    default:
        printf("Error!");
    }
}
// khai bao node
typedef struct Node {
    State state; 
    struct Node* Parent;
    int no_function;
}Node;
int compare(State state1, State state2) {
        if (state1.nguoi == state2.nguoi && state1.quy == state2.quy && state1.ab == state2.ab) {
            return 1;
        } else return 0;
    }

int find(State state, queue<Node *> q) {
    while (!q.empty()) {
		if (compare(state, q.front()->state)) return 1;
		q.pop();
	} 
	return 0;
}
// bfs 
Node *BFS_Algorithm(State state) {
	// khai bao queue open , close
	queue<Node *> open, close; 
	// tao root -> open 
	Node *root = (Node*)malloc(sizeof(Node));
	root->no_function = 0;
	root->state = state; 
	root->Parent = NULL;
	open.push(root);
	 //duyet 
	while (!open.empty()) {
		// lay node cua open.front
		Node *node = open.front();
		open.pop(); 
		close.push(node);
		// kt goal
		if (goalCheck(node->state)) return node; 
		// chay cac operator 	
		for (int i = 1; i<=5; i++) {
			State newState;
			if (call_operation(node->state, &newState, i)) {	
				// kt newState da co trong open hoac close 
				if (find(newState, open) || find(newState, close)) 	continue;
				// them vao open 
				Node * newNode = (Node*)malloc(sizeof(Node));
				newNode->no_function = i;
				newNode->state = newState; 
				newNode->Parent = node;
				open.push(newNode);
			}
		}
	}
}
const char* action[] = {"Frist state","Di chuyen 1 quy" ,"Di chuyen 2 quy", "Di chuyen 1 nguoi", "Di chuyen 2 nguoi", "Di chuyen 1 quy 1 nguoi" };
// in ket qua
void print_wayToGetGoal(Node* node) {
    stack<Node *> result; 
    while (node->Parent != NULL) {
        result.push(node);
        node = node->Parent; 
    }
    result.push(node);
    // in ra thu tu
    int no_action = 0;
    while (!result.empty()){
        printf("\nAction %d: %s \n",no_action,action[result.top()->no_function]);
        print_state(result.top()->state);
        no_action++;
        result.pop();
    }
}
// main
int main() {
	State cur_state = {3, 3, 'A'};
    
     Node* p = BFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
}