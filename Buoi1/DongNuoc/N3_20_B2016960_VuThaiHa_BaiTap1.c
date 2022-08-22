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
int main() {
    State cur_state = {5 , 4}, result;
    printf("Trang thai bat dau: ");
    printState(cur_state);
    for (int opt = 1; opt<=6; opt++) {
        int thuchien = call_operation(cur_state,&result, opt); 
        if (thuchien) {
            printf("\nThuc hien hanh dong %s thanh cong.", action[opt]);
            printState(result);
        } else  printf("\nThuc hien hanh dong %s KHONG thanh cong.", action[opt]);
    }
} 
