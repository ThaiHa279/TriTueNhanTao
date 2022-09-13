#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>

#define ROWS 3 
#define COLS 3
#define EMPTY 0
#define MaxLength 5000

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
int heuristis1(State state, State goal) {
    int count = 0;
        for(int i = 0; i<ROWS; i++) 
            for (int j = 0; j<COLS; j++) 
                if (state.Puzzel[i][j] != goal.Puzzel[i][j]) count++;
    return count;
}
typedef struct Node {
    State state;
    struct Node *Parent;
    int no_function; 
    int f, g, h; 
}Node;
typedef struct {
    Node* Elements[MaxLength]; 
    int size; 
} List;
void push(Node* node, List *list) {
    list->Elements[list->size] = node;
    list->size++;
}
void delete(int p, List *list) {
   for (int i = p; i < list->size; i++) 
        list->Elements[i] = list->Elements[i+1];
   list->size--;
}
void sort(List *l) {
    for (int i = 0; i < l->size-1; i++) 
        for (int j = i+1; j < l->size; j++) 
            if (l->Elements[i]->f > l->Elements[j]->f) {
                Node *temp = l->Elements[i];
                l->Elements[i] = l->Elements[j]; 
                l->Elements[j] = temp; 
            }
}
Node *find_state(State state, List list, int *p) {
    for (int i = 0; i < list.size; i++) {
        if (compare_state(state, list.Elements[i]->state)) {
            *p = i;
            return list.Elements[i];
        }
    }
    return NULL;
}
int dem = 0;
Node* A_Star(State state, State goal) {
    List open, close; 
    open.size = 0; 
    close.size = 0;
    Node *root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    root->g = 0; 
    root->h = heuristis1(state, goal); 
    root->f = root->g + root->h;
    push(root,&open);
    while (open.size) {
        Node *node = open.Elements[0];
        delete(0, &open); 
        push(node,&close);
        if (goalCheck(node->state)) return node;
        // printf("%d\n",node->h);
        // print_state(node->state);
        for (int i = 1; i <= 4; i++) {
            State new_state;
            if (call_operation(node->state, &new_state, i)) {

                Node* new_node = (Node*)malloc(sizeof(Node));
                new_node->state = new_state;
                new_node->Parent = node;
                new_node->no_function = i;
                new_node->g = node->g+1;
                new_node->h = heuristis1(new_node->state, goal);
                new_node->f = new_node->g+new_node->h;

                int p_open, p_close;
                Node *node_open = find_state(new_state,open,&p_open);
                Node *node_close = find_state(new_state,close,&p_close);
                if (node_open == NULL && node_close == NULL) {
                    push(new_node, &open);
                }
                 else if (node_open != NULL && node_open->f > new_node->f) { 
                    // xoa pt trong open tai pos open
                    delete(p_open,&open);
                    push(new_node,&open);
                } else if (node_close != NULL && node_close->f > new_node->f) {
                        // xoa pt trong close tai pos close
                    delete(p_close,&open);
                    push(new_node,&open);
                }
                sort(&open); 
            }
         }
        // if (dem > 100 ) return NULL; 
        // else dem++;
    }
}
void print_wayToGetGoal(Node* node) {
    List list; 
    list.size = 0;
    while (node -> Parent != NULL) {
        push(node, &list); 
        node = node -> Parent;
    } 
    push(node, &list); 
    for (int i = list.size-1; i>=0; i--) {
        printf("Action %d: %s\n", list.size-1-i,action[list.Elements[i]->no_function]);
        print_state(list.Elements[i]->state); 
    }
}
int main() {
    int a[3][3] = { {1, 2, 3}, {8, 0, 4}, {7, 6, 5} }; 
    int b[3][3] = { {2, 8, 1}, {0, 4, 3}, {7, 6, 5} }; 
    State state; 
    goal.emptyCol = 0; 
    goal.emptyRow = 1; 
    state.emptyCol = 1; 
    state.emptyRow = 1; 
    for (int i = 0; i < 3; i++) 
        for (int j = 0; j < 3; j++) {
            state.Puzzel[i][j] = a[i][j];
            goal.Puzzel[i][j] = b[i][j];
        }
    
    Node *result; 
    result = A_Star(state, goal); 
    print_wayToGetGoal(result);
}