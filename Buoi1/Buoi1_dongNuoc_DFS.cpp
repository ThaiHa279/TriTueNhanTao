#include <stdio.h>
#include <stdlib.h>

#define tankCapacity_X 9
#define tankCapacity_Y 4

#define empty 0
#define goal 6

typedef struct {
    int x,y;
}State;
// khoi tao trang thai bat dau
void makeNullState(State * state) {
    state->x = 0;
    state->y = 0;
}
// in trang thai
void printState(State state) {
    printf("\n   X: %d, Y: %d",state.x,state.y);
}
// Kiem tra luong nuoc co phai muc tieu hay chua
int goalCheck(State state) {
    return (state.x == goal || state.y == goal); 
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
            result->x = empty;
            result->y = cur_state.y;
            return 1;
        }
        return 0;
    }
    int pourWaterEmptyY(State cur_state,State* result ) {
        if (cur_state.x > 0) {
            result->y = empty;
            result->x = cur_state.x;
            return 1;
        }
        return 0;
    }
    int pourWaterXY(State cur_state,State* result ) {
        if (cur_state.x > 0 && cur_state.y < tankCapacity_Y) {
            // luong nuoc binh x - luong nuoc co the vao binh y; th: x > y => x = 0
            result->x = max(cur_state.x - (tankCapacity_Y - cur_state.y), empty);
            // th: x + y > maxY
            result->y = min(cur_state.x + cur_state.y, tankCapacity_Y);
            return 1;
        }
        return 0;
    }
    int pourWaterYX(State cur_state,State* result ) {
        if (cur_state.y > 0 && cur_state.x < tankCapacity_X) {
            // luong nuoc binh y - luong nuoc co the vao binh y; th: y > x => y = 0
            result->y = max(cur_state.y - (tankCapacity_X - cur_state.x), empty);
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
//  BAI TAP 1: 
const char * action[] = {"Frist state", "pour water full X", "pour water full Y", 
                        "pour water empty X","pour water empty Y", "pour water X to Y", "pour water Y to X"}; 
/*int main() {
    State cur_state{5 , 4}, result;
    printf("Trang thai bat dau: ");
    printState(cur_state);
    for (int opt = 1; opt<=6; opt++) {
        int thuchien = call_operation(cur_state,&result, opt); 
        if (thuchien) {
            printf("\nThuc hien hanh dong %s thanh cong.", action[opt]);
            printState(result);
        } else  printf("\nThuc hien hanh dong %s KHONG thanh cong.", action[opt]);
    }
} */
// khai bao cay tim kiem 
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
    // tim trang thai
    int compare(State state1, State state2) {
        if (state1.x == state2.x && state1.y == state2.y) {
            return 1;
        } else return 0;
    }
    int find(State state,Stack stack){
        for (int i = stack.top_idx; i<MaxLength; i++) {
            if (compare(state, stack.Elements[i]->state)) return 1;
        }
        return 0;
    }
// thuat toan theo chieu sau
Node* DFS_Algorithm(State state){
    //khoi tao stack open, close 
    Stack open, close; 
    makeNullStack(&open);
    makeNullStack(&close);
    // tao nut trang thai cha
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->no_function = 0;
    push(root,&open);
    //duyet dfs
    while (!emptyStack(open)) {
        // lay dinh trong open => close
        Node* node = top(open);
        pop(&open);
        push(node,&close);
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
                push(newNode,&open);
            } 
        }
    }
    return NULL;
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
        printState(top(result)->state);
        no_action++;
        pop(&result);
    }
}
int main() {
    State cur_state = {0,0};
    Node* p = DFS_Algorithm(cur_state);
    print_wayToGetGoal(p);
}