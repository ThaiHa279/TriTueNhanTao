#include <stdio.h>
#include <stdlib.h>
#define ROWS 3
#define COLS 3
#define EMPTY 0
#define MAX_OPERATOR 4
#define Maxlength 500

const char* action[] = {
    "First state",
    "Move UP",
    "Move Down",
    "Move Left",
    "Move Right"
};

typedef struct 
{
    int eightPuzzel[ROWS][COLS];
    int emptyRow;   //luu chi muc dong
    int emptyCol;   //luu chi muc cot
}State;

void printState(State state){
    int row,col;
    printf("\n----------\n");
    for(row=0;row<ROWS;row++){
        for(col=0;col<COLS;col++){
            printf("|%d ",state.eightPuzzel[row][col]);
        }
        printf("|\n");
    }
}

int compareStates(State state1,State state2){
    if(state1.emptyCol != state2.emptyCol || state1.emptyRow != state2.emptyRow)
        return 0;
    int row,col;
    for(row=0;row<ROWS;row++){
        for(col=0;col<COLS;col++){
            if(state1.eightPuzzel[row][col]!=state2.eightPuzzel[row][col])
                return 0;
        }
    }
    return 1;
}

int goalCheck(State state,State goal){
    return compareStates(state,goal);
}

//Xay dung cac hanh dong
int upOperator(State state,State *result){
    *result = state;
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
    if(empRowCurrent > 0){
        result->emptyRow = empRowCurrent -1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent-1][empColCurrent];
        result->eightPuzzel[empRowCurrent-1][empColCurrent] = EMPTY;
        return 1;
    }
    return 0;
}

int downOperator(State state,State *result){
    *result = state;
    int empRowCurrent = state.emptyRow, empColCurrent= state.emptyCol;
    if(empRowCurrent < 2){
        result->emptyRow = empRowCurrent + 1;
        result->emptyCol = empColCurrent;
        result->eightPuzzel[empRowCurrent][empColCurrent]= state.eightPuzzel[empRowCurrent+1][empColCurrent];
        result->eightPuzzel[empRowCurrent+1][empColCurrent]= EMPTY;
        return 1;
    }
    return 0;
}

int leftOperator(State state,State *result){
    *result = state;
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
    if(empColCurrent > 0){
        result->emptyRow = empRowCurrent;
        result->emptyCol = empColCurrent-1;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent-1];
        result->eightPuzzel[empRowCurrent][empColCurrent-1] = EMPTY;
        return 1;
    }
    return 0;
}

int rightOperator(State state,State *result){
    *result = state;
    int empRowCurrent = state.emptyRow, empColCurrent = state.emptyCol;
    if(empColCurrent < 2){
        result->emptyRow = empRowCurrent;
        result->emptyCol = empColCurrent+1;
        result->eightPuzzel[empRowCurrent][empColCurrent] = state.eightPuzzel[empRowCurrent][empColCurrent+1];
        result->eightPuzzel[empRowCurrent][empColCurrent+1] = EMPTY;
        return 1;
    }
    return 0;
}

int callOperators(State state,State *result,int opt){
    switch (opt)
    {
    case 1: return upOperator(state,result);
    case 2: return downOperator(state,result);
    case 3: return leftOperator(state,result);
    case 4: return rightOperator(state,result);

    default:
        printf("Khong the goi hanh dong");
        break;
    }
}

//Ham heuristic 1: Dem so o sai khac so voi trang thai muc tieu
int heuristic1(State state,State goal){
    int row,col,cnt=0;
    for(row=0;row<=ROWS;row++){
        for ( col = 0; col < COLS; col++){
            if (state.eightPuzzel[row][col]!= goal.eightPuzzel[row][col])
            {
                cnt++;
            }
            
        }
    }
    return cnt;
}

//Ham heuristic 2:Dem so buoc chuyen o sai ve o dung cua trang thai muc tieu
int heuristic2(State state,State goal){
    int cnt=0;
    int row,col,row_g,col_g;
    for ( row = 0; row < ROWS; row++){
        for ( col = 0; col < COLS; col++){
            if(state.eightPuzzel[row][col]!=EMPTY){
                for ( row_g = 0; row_g < ROWS; row_g++){
                    for ( col_g = 0; col_g < COLS; col_g++){
                        if(state.eightPuzzel[row][col] == goal.eightPuzzel[row_g][col_g]){
                            cnt+=abs(row-row_g) + abs(col- col_g);
                            col_g=COLS; //break loop col_g
                            row_g=ROWS; //break loop row_g
                        }
                    }
                }
            }
        }
    }
    return cnt;
}

typedef struct Node
{
    State state;
    struct Node* parent;
    int no_function;
    int heuristic;
}Node;

//Khai bao cau truc danh sach
typedef struct 
{
    Node* Elements[Maxlength];
    int size;
}List;

void makeNull_List(List *list){
    list->size = 0;
}
int emptyList(List list){
    return list.size == 0;
}
int fullList(List list){
    return list.size == Maxlength - 1;
}
Node* element_at(int p,List list){
    return list.Elements[p-1];
}
void push_List(Node* x, int position, List *list){
    if(!fullList(*list)){
        int q;
        for ( q = list->size ; q >= position; q--)
        {
            list->Elements[q] = list->Elements[q-1];
        }
        list->Elements[position - 1]= x;
        list->size++;
    }
    else
        printf("List is full\n");
}
void delete_List(int position,List *list){
    if(emptyList(*list)){
        printf("Danh sach rong\n");
    }
    else if(position<1 || position > list->size){
        printf("Vi tri khong the xoa\n");
    }
    else{
        int i;
        for(i=position-1;i<list->size;i++){
            list->Elements[i] = list->Elements[i+1];
        }
        list->size--;
    }
}

//Tim trang thai state co thuoc Open hoac Close khong
//Luu vi tri tim duoc vao bien *position
Node* findState(State state,List list, int *position){
    int i;
    for(i=1;i<=list.size;i++){
        if(compareStates(element_at(i,list)->state,state)){
            *position=i;
            return element_at(i,list);
        }
    }
    return NULL;
}

//Sap xep ds theo trong so Heuristic
void sort_List(List *list){
    int i,j;
    for ( i = 0; i < list->size -1; i++)
    {
        for ( j = i+1; j < list->size; j++)
        {
            if(list->Elements[i]->heuristic > list->Elements[j]->heuristic){
                Node* node = list->Elements[i];
                list->Elements[i]= list->Elements[j];
                list->Elements[j] = node;
            }
        }
        
    }
}
int dem = 0;
//Ham f = h
Node* BFS(State state,State goal){
    List Open;
    List Close;
    makeNull_List(&Open);
    makeNull_List(&Close);
    Node* root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->no_function = 0;
    root->parent = NULL;
    root->heuristic = heuristic1(root->state,goal);
    push_List(root,Open.size + 1, &Open);
    while (!emptyList(Open))
    {
        Node *node = element_at(1,Open);
        delete_List(1,&Open);
        push_List(node,Close.size+1,&Close);
        if(goalCheck(node->state,goal)){
            return node;
        }
                      
        int opt;
        for ( opt = 1; opt <= MAX_OPERATOR; opt++)
        {
            State newstate;
            newstate = node->state;
			if(callOperators(node->state,&newstate,opt)){
                Node *newNode = (Node*)malloc(sizeof(Node));
                newNode->state = newstate;
                newNode->parent=node;
                newNode->no_function=opt;
                newNode->heuristic = heuristic1(newstate,goal);

                //Kiem tra trang thai moi co thuoc Open/Close khong
                int pos_Open,pos_Close;
                Node *nodeFoundOpen = findState(newstate,Open,&pos_Open);
                Node *nodeFoundClose = findState(newstate,Close,&pos_Close);
                if(nodeFoundOpen == NULL && nodeFoundClose == NULL){ //Dieu kien nay luon chay
                   push_List(newNode,Open.size+1,&Open);
                } 
                else if(nodeFoundOpen != NULL && nodeFoundOpen->heuristic > newNode->heuristic){
                    delete_List(pos_Open,&Open);
                    push_List(newNode,pos_Open + 1,&Open);
                }
                else if(nodeFoundClose !=NULL && nodeFoundClose->heuristic > newNode->heuristic){
                    delete_List(pos_Close,&Close);
                    push_List(newNode,Open.size+1,&Open);
                }
                sort_List(&Open);
            }
        }

    }
}

void print_WaysToGetGoal(Node* node){
    List L;
    makeNull_List(&L);
    while (node->parent!=NULL)
    {
        push_List(node,L.size+1,&L);
        node = node->parent;
    }
    push_List(node,L.size+1,&L);
    int no_function=0;
    int i;
    for ( i = L.size; i>=1 ; i--)
    {
        printf("\n Action %d: %s",no_function,action[element_at(i,L)->no_function]);
        printState(element_at(i,L)->state);
        no_function++;
    }
    
}

int main(){
    State state;
    state.emptyRow = 1;
    state.emptyCol = 1;
    state.eightPuzzel[0][0]=3;
    state.eightPuzzel[0][1]=4;
    state.eightPuzzel[0][2]=5;
    state.eightPuzzel[1][0]=1;
    state.eightPuzzel[1][1]=0;
    state.eightPuzzel[1][2]=2;
    state.eightPuzzel[2][0]=6;
    state.eightPuzzel[2][1]=7;
    state.eightPuzzel[2][2]=8;

    State goal;
    goal.emptyRow = 0;
    goal.emptyCol = 0;
    goal.eightPuzzel[0][0]=0;
    goal.eightPuzzel[0][1]=1;
    goal.eightPuzzel[0][2]=2;
    goal.eightPuzzel[1][0]=3;
    goal.eightPuzzel[1][1]=4;
    goal.eightPuzzel[1][2]=5;
    goal.eightPuzzel[2][0]=6;
    goal.eightPuzzel[2][1]=7;
    goal.eightPuzzel[2][2]=8;

    print_WaysToGetGoal(BFS(state,goal));
    return 0;
}
