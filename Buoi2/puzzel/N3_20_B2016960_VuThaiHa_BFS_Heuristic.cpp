#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
using namespace std;
#define ROWS 3 
#define COLS 3
#define EMPTY 0
#define MaxLength 1000

const char * action[] = {"Frist state", "Move cell EMPTY to UP", "Move cell EMPTY to DOWN",
                             "Move cell EMPTY to LEFT", "Move cell EMPTY to RIGHT"};
typedef struct{
    int Puzzel[ROWS][COLS];
    int emptyRow; 
    int emptyCol;
}State; 

State goal;

void print_state(State state) {
    printf("--------------\n");
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            printf("| %d ", state.Puzzel[i][j]);
        }
        printf("|\n");
    }
        printf("--------------\n");
}

int compare_state(State state1, State state2) {
    if (state1.emptyCol != state2.emptyCol || state1.emptyRow != state2.emptyRow) return 0;
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (state1.Puzzel[i][j] != state2.Puzzel[i][j]) return 0;
        }
    }
    return 1;
}
int goalCheck(State state) {
    return compare_state(state,goal);
}

int upOperator(State state, State * result){
    *result = state; 
    if (state.emptyRow > 0) {
        result->Puzzel[state.emptyRow][state.emptyCol] = state.Puzzel[state.emptyRow-1][state.emptyCol];
        result->Puzzel[state.emptyRow-1][state.emptyCol] = EMPTY;
        result->emptyRow--;
        return 1;
    } return 0;
}
int downOperator(State state, State * result){
    *result = state; 
    if (state.emptyRow < ROWS-1) {
        result->Puzzel[state.emptyRow][state.emptyCol] = state.Puzzel[state.emptyRow+1][state.emptyCol];
        result->Puzzel[state.emptyRow+1][state.emptyCol] = EMPTY;
        result->emptyRow++;
        return 1;
    } return 0;
}
int leftOperator(State state, State * result){
    *result = state; 
    if (state.emptyCol > 0) {
        result->Puzzel[state.emptyRow][state.emptyCol] = state.Puzzel[state.emptyRow][state.emptyCol-1];
        result->Puzzel[state.emptyRow][state.emptyCol-1] = EMPTY;
        result->emptyCol--;
        return 1;
    } return 0;
}
int rightOperator(State state, State * result){
    *result = state; 
    if (state.emptyCol < COLS-1) {
        result->Puzzel[state.emptyRow][state.emptyCol] = state.Puzzel[state.emptyRow][state.emptyCol+1];
        result->Puzzel[state.emptyRow][state.emptyCol+1] = EMPTY;
        result->emptyCol++;
        return 1;
    } return 0;
}
int call_operation(State state, State * result,int opt){
    switch (opt){
        case 1: return upOperator(state, result); 
        case 2: return downOperator(state, result); 
        case 3: return leftOperator(state, result); 
        case 4: return rightOperator(state, result); 
    }
}
int heuristis2(State state, State goal) {
    int count = 0;
    for(int i = 0; i<ROWS; i++) 
        for (int j = 0; j<COLS; j++) 
            if (state.Puzzel[i][j] != EMPTY) 
                for(int i_g = 0; i_g<ROWS; i_g++) 
                    for (int j_g = 0; j_g<COLS; j_g++) 
                        if (state.Puzzel[i][j] == goal.Puzzel[i_g][j_g]) {
                            count += abs(i-i_g)+abs(j-j_g);
                            j_g = COLS; i_g = ROWS;
                        }
    return count;
}
int heuristis1(State state, State goal) {
    int count = 0;
        for(int i = 0; i<ROWS; i++) 
            for (int j = 0; j<COLS; j++) 
             if (state.Puzzel[i][j] != goal.Puzzel[i][j]) count++;
    return count;
}
// node
typedef struct Node{
    State state; 
    struct Node* parent;
    int no_function;
    int heuristis; 
}Node;
bool sosanh(Node *a, Node *b) {
    return a->heuristis > b->heuristis;
}
Node *find_state(State state,vector<Node *> v, vector<Node *>::iterator *p ) {
    vector<Node *>::iterator it = v.begin();
    while (it != v.end()) {
        if (compare_state((*it)->state,state)){
            *p = it; 
            return *it;
        }
        it = v.erase(it);
    }
    return NULL;
}
int dem = 0;
Node *best_first_sreach(State state, State goal) {
    vector<Node*> open, close; 
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->parent = NULL;
    root->no_function = 0; 
    root->heuristis = heuristis1(state,goal); 
    open.push_back(root);
    while (!open.empty()) {
        Node *node = open.back();
        open.pop_back();
        close.push_back(node); 
        if (goalCheck(node->state)) return node;      
        for (int i = 1; i <= 4; i++) {
            State new_state = node->state; 
            if (call_operation(node->state, &new_state, i)) {
                Node* new_node = (Node*)malloc(sizeof(Node));
                new_node->state = new_state;
                new_node->parent = node;
                new_node->no_function = i; 
                new_node->heuristis = heuristis1(new_state,goal); 
                // kt trang thai moi 
                vector<Node*>::iterator p_open, p_close;
                Node *node_open = find_state(new_state,open,&p_open);
                Node *node_close = find_state(new_state,close,&p_close);
                if (node_open == NULL && node_close == NULL) {
                    open.push_back(new_node);
                
                } else if (node_open != NULL && node_open->heuristis > new_node->heuristis) { 
                    // xoa pt trong open tai pos open
                    open.erase(p_open);
                    open.push_back(new_node);
                } else if (node_close != NULL && node_close->heuristis > new_node->heuristis) {
                     // xoa pt trong close tai pos close
                    close.erase(p_close);
                    open.push_back(new_node);
                }
            }
        }
        sort(open.begin(), open.end(), sosanh); 
    }
    return NULL;
}
void print_wayToGetGoal(Node* node) {
    vector<Node*> list; 
    while (node -> parent != NULL) {
        list.push_back(node); 
        node = node -> parent;
    } 
    list.push_back(node); 
    for (int i = list.size()-1; i>=0; i--) {
        printf("Action %d: %s\n", list.size()-1-i,action[list.at(i)->no_function]);
        print_state(list.at(i)->state); 
    }
}
int main() {
    int a[3][3] = { {3, 4, 5}, {1, 0, 2}, {6, 7, 8} }; 
    int b[3][3] = { {0, 1, 2}, {3, 4, 5}, {6, 7, 8} }; 
    State state; 
    goal.emptyCol = 0; 
    goal.emptyRow = 0; 
    state.emptyCol = 1; 
    state.emptyRow = 1; 
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) {
            state.Puzzel[i][j] = a[i][j];
            goal.Puzzel[i][j] = b[i][j];
        }
    
    Node *result; 
    result = best_first_sreach(state, goal); 
    print_wayToGetGoal(result);
}
