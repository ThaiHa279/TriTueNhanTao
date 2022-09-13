#include <stdio.h>
#include <stdlib.h>
#define MAX_VERTICES 5
#define MAX_LENGTH 1000

// xay dung dinh do thi 
typedef struct {
    int neighbour[MAX_VERTICES];
    int h;
} Vertices; 
// xay dung do thi
typedef struct {
    int num_vertices;
    Vertices v[MAX_VERTICES];
}Graph; 
// khoi tao do thi
void initGraph(int num_vertices, Graph *G) {
    G->num_vertices = num_vertices;
    for (int i = 0; i < G->num_vertices; ++i) {
        for (int j = 0; j < G->num_vertices; ++j) {
            G->v[i].neighbour[j] = 0;
        }
    }
}
const char name[] = { 'A', 'B', 'C', 'D', 'G'}; 

// khai bao trang thai 
typedef struct {
    int vertex; 
} State; 

// In trang thai
void print_state(State state) {
    printf("%c ", name[state.vertex]); 
}
// so sanh trang thai
int compare_state(State state1, State state2) {
    return (state1.vertex == state2.vertex);
}
// cau truc Node
typedef struct Node {
    struct Node * Parent; 
    State state; 
    int g,h,f; 
} Node; 
//Khai bao cau truc danh sach
typedef struct 
{
    Node* Elements[MAX_LENGTH];
    int size;
}List;

void makeNull_List(List *list){
    list->size = 0;
}
int emptyList(List list){
    return list.size == 0;
}
int fullList(List list){
    return list.size == MAX_LENGTH - 1;
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
Node* findState(State state,List list, int *position){
    int i;
    for(i=1;i<=list.size;i++){
        if(compare_state(element_at(i,list)->state,state)){
            *position=i;
            return element_at(i,list);
        }
    }
    return NULL;
}

void sort_List(List *list){
    int i,j;
    for ( i = 0; i < list->size -1; i++)
        for ( j = i+1; j < list->size; j++)
            if(list->Elements[i]->f > list->Elements[j]->f){
                Node* node = list->Elements[i];
                list->Elements[i]= list->Elements[j];
                list->Elements[j] = node;
            }
}
// kt goal 
int goalCheck(State state, State goal) {
    return compare_state(state, goal);
}
int dem = 0;
Node* A_Star(Graph G, State state, State goal) {
    List open, close; 
    open.size = 0; 
    close.size = 0;
    Node *root = (Node*)malloc(sizeof(Node));
    root->state = state;
    root->Parent = NULL;
    root->g = 0; 
    root->h = G.v[state.vertex].h; 
    root->f = root->g + root->h;
    push_List(root,1, &open);
    while (!emptyList(open)) {
        Node *node = element_at(1, open);
        delete_List(1, &open); 
        push_List(node,close.size+1,&close);
        if (goalCheck(node->state, goal)) return node;
        // printf("%d - ",node->g);
        // print_state(node->state);
       for (int i = 0; i < G.num_vertices; i++) { // duyet tung dinh
            if (G.v[node->state.vertex].neighbour[i] > 0) { // kt > 0 tung dinh
                Node* new_node = (Node*)malloc(sizeof(Node));
                new_node->state.vertex = i;
                new_node->Parent = node;
                new_node->g = node->g+ G.v[node->state.vertex].neighbour[i];
                new_node->h =G.v[i].h;
                new_node->f = new_node->g+new_node->h;

                int p_open, p_close;
                Node *node_open = findState(new_node->state,open,&p_open);
                Node *node_close = findState(new_node->state,close,&p_close);
                if (node_open == NULL && node_close == NULL) {
                    push_List(new_node, open.size +1, &open);
                }
                else if (node_open != NULL && node_open->g > new_node->g) { 
                   // xoa pt trong open tai pos open
                   delete_List(p_open,&open);
                   push_List(new_node,p_open,&open);
               } else if (node_close != NULL && node_close->g > new_node->g) {
                       // xoa pt trong close tai pos close
                   delete_List(p_close,&close);
                   push_List(new_node,open.size+1,&open);
               }
            }
       }
         sort_List(&open); 
        //  if (dem > 100 ) return NULL; 
        //  else dem++;
	   }
}
void print_wayToGetGoal(Node* node) {
    List list; 
    list.size = 0;
    while (node -> Parent != NULL) {
        push_List(node,list.size+1, &list);
        node = node -> Parent;
    } 
    push_List(node,list.size+1, &list); 
    for (int i = list.size; i>0; i--) {
        print_state(element_at(i,list)->state); 
        if (i != 1){
            printf("->"); 
        }
    }
}
int main() {
    Graph graph; 
    freopen("test.txt", "r", stdin);
    initGraph(MAX_VERTICES, &graph);
    for (int i = 0; i < MAX_VERTICES; i++) {
        int x; 
        scanf("%d", &x);
        graph.v[i].h = x; 
        for (int j = 0; j < MAX_VERTICES; j++) {
            scanf("%d", &x); 
            graph.v[i].neighbour[j] = x; 
        }
    }
    State A, G; 
    A.vertex = 0; 
    G.vertex = 4; 
    Node *result = A_Star(graph,A, G); 
    print_wayToGetGoal(result); 
}



