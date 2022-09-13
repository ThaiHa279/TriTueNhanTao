#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 100
// cau truc toa do 
typedef struct {
    int x,y;
}Coord; 

//cau truc danh sach toa do 
typedef struct {
    Coord data[MAX_LENGTH];
    int size; 
} listCoord; 
void initListCoord(listCoord *list) {
    list ->size = 0; 
}
void appendListCoord(listCoord *list, Coord coord) {
    list->data[list->size++] = coord; 
}
// cau truc rang buoc
#define NB_ROWS 9 
#define NB_COLS 9
typedef struct {
    int data[NB_ROWS*NB_COLS][NB_ROWS*NB_COLS];
    int n;
} Constraints; 
// khoi tao rang buoc rong
void initConstraints(Constraints *constraints) {
    for (int i = 0; i < NB_COLS*NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS*NB_ROWS; j++) 
            constraints->data[i][j] = 0; 
    constraints->n = NB_ROWS*NB_COLS; 
}
//chuyen doi gia tri
int indexOf(Coord coord) {
    return coord.x*NB_COLS + coord.y; 
}
Coord positionOf(int vertex) {
    Coord c; 
    c.x = vertex / NB_COLS;
    c.y = vertex % NB_COLS;  
    return c;
}
// them rang buoc
int addConstraints(Constraints *constraints, Coord source, Coord taget) {
    int u = indexOf(source); 
    int v = indexOf(taget);
    if (constraints->data[u][v] == 0) {
        constraints->data[u][v] = 1;
        constraints->data[v][u] = 1;
        return 1;
    }
    return 0; 
}
// tap rang buoc cua 1 dinh 
listCoord getConstraints(Constraints constraints, Coord coord) {
    int v = indexOf(coord); 
    listCoord result; 
    initListCoord(&result);
    for (int i = 0; i < constraints.n; i++) {
        if (constraints.data[v][i] == 1) {
            appendListCoord(&result, positionOf(i));
        }
    }    
    return result;
}
// cau truc bang sudoku 
#define MAX_VALUE 10
#define EMPTY 0 
#define AREA_SQUARE_VALUE 3
#define INF 999999
typedef struct{
    int cells[MAX_VALUE][MAX_VALUE]; 
    Constraints constraints; 
} Sudoku; 
// khoi tao bang sudoku rong
void initSudoku(Sudoku *sudoku) {
    for (int i = 0; i < NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS; j++)
            sudoku->cells[i][j] = EMPTY;
    initConstraints(&sudoku->constraints);
}
// khoi tao bang sudoku co gia tri
void initSudokuAndValue(Sudoku *sudoku, int value[NB_ROWS][NB_COLS]) {
    for (int i = 0; i < NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS; j++)
            sudoku->cells[i][j] = value[i][j];
    initConstraints(&sudoku->constraints);
}
// in bang 
void printSudoku(Sudoku sudoku) {
    for (int i = 0; i < NB_ROWS; i++){
        if (i % AREA_SQUARE_VALUE == 0) printf("----------------------\n"); 
        for (int j = 0; j < NB_COLS; j++) {
            if (j % AREA_SQUARE_VALUE == 0) printf("|" );
            printf("%d ", sudoku.cells[i][j]); 
        }
        printf("|\n"); 
    }
    printf("----------------------\n");
}
// kiem tra trang thai ket thuc
int isFilledSudoku(Sudoku sudoku) {
    for (int i = 0; i < NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS; j++) 
            if (sudoku.cells[i][j] == EMPTY) return 0;
}
// tao rang buoc tu vi tri cho truoc
void spreadConstrainsFrom(Coord p, Constraints *constraints, listCoord* changeds) {
    int row = p.x, col = p.y;
    // tao rang buoc theo cot
    for (int i = 0; i < NB_ROWS; i++) {
        if (i != row) {
            Coord pos = {i, col}; 
            if (addConstraints(constraints, p, pos)) appendListCoord(changeds, pos);
        }
    }
    // tao rang buoc theo hang
    for (int i = 0; i < NB_COLS; i++) {
        if (i != col) {
            Coord pos = {row, i}; 
            if (addConstraints(constraints, p, pos)) appendListCoord(changeds, pos);
        }
    }
    // tao rang buoc theo khoi 
    for (int i = 0; i < AREA_SQUARE_VALUE; i++) 
        for (int j = 0; j < AREA_SQUARE_VALUE; j++) {
            int x = (row/AREA_SQUARE_VALUE)*AREA_SQUARE_VALUE;
            int y = (col/AREA_SQUARE_VALUE)*AREA_SQUARE_VALUE;
            if (x+i != row || y+j != col) {
                Coord pos = {x+i, y+j}; 
                if (addConstraints(constraints, p, pos)) appendListCoord(changeds, pos);
            }
        }
}
//Khai bao cau truc danh sach 
typedef struct 
{
    int Elements[MAX_LENGTH];
    int size;
}List;

void initList(List *list){
    list->size = 0;
}
int emptyList(List list){
    return list.size == 0;
}
int fullList(List list){
    return list.size == MAX_LENGTH - 1;
}

// tim mien gia tri cua o trong
List getAvailableValues(Coord position, Sudoku sudoku) {
    listCoord posList = getConstraints(sudoku.constraints, position); 
    int available[MAX_VALUE]; 
    for (int i=0;i<MAX_VALUE;i++) {
        available[i] = 1; 
    }
    for (int i = 0; i < posList.size; i++){
        Coord pos = posList.data[i];
        if (sudoku.cells[pos.x][pos.y] != EMPTY) {
            available[sudoku.cells[pos.x][pos.y]] = 0;
        }
    }
    List result; 
    initList(&result);
    for (int i = 1; i < MAX_VALUE; i++) {
        if (available[i] == 1) {
            result.Elements[result.size++] = i;
        }
    }
    return result;
}
// xac dinh o ua tien dien truoc
Coord getNextEmptyCell(Sudoku sudoku) {
    for (int i = 0; i < NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS; j++) {
            Coord pos = {i,j};
            if (sudoku.cells[i][j] == EMPTY) return pos;
        }
}
Coord getNextMinDomainCell(Sudoku sudoku) {
    int minLength = INF; 
    Coord result; 
    for (int i = 0; i < NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS; j++) {
            if (sudoku.cells[i][j] == EMPTY) {
                Coord pos = {i,j};
                int availableLength = getAvailableValues(pos,sudoku).size;
                if (availableLength < minLength) {
                    minLength = availableLength;
                    result = pos;
                }
            }
        }
    return result;
}

// giai quyet bai toan 
int explored = 0;
int sudokuBackTracking(Sudoku *sudoku) {
    if (isFilledSudoku(*sudoku)) return 1;
//  Coord position = getNextEmptyCell(*sudoku);
    Coord position = getNextMinDomainCell(*sudoku);
    List available = getAvailableValues(position,*sudoku);
    if (available.size == 0) return 0; 
    for (int i = 0; i < available.size; i++) {
        int value = available.Elements[i];
        sudoku->cells[position.x][position.y] = value;
        explored++;
        listCoord history; 
        initListCoord(&history); 
        spreadConstrainsFrom(position, &sudoku->constraints, &history);
        if (sudokuBackTracking(sudoku)) return 1;
        sudoku -> cells[position.x][position.y] = EMPTY; 
    }
    return 0;
}
// lan truyen rang buoc
Sudoku solveSudoku(Sudoku sudoku) {
    initConstraints(&sudoku.constraints);
    for (int i = 0; i < NB_ROWS; i++) 
        for (int j = 0; j < NB_COLS; j++) 
            if (sudoku.cells[i][j] != EMPTY) {
                listCoord history; 
                initListCoord(&history);
                Coord pos = {i, j}; 
                spreadConstrainsFrom(pos, &sudoku.constraints,&history);
            }
    
    explored = 0;
    if (sudokuBackTracking(&sudoku)) printf("Solved\n"); 
    else printf("Can not solved\n");
    printf("Explored %d states\n",explored);
    return sudoku; 
}
int input[9][9] = {
    {0, 6, 1, 0, 0, 7, 0, 0, 3}, 
    {0, 9, 2, 0, 0, 3, 0, 0, 0}, 
    {0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 8, 5, 3, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 5, 0, 4},
    {5, 0, 0, 0, 0, 8, 0, 0, 0},
    {0, 4, 0, 0, 0, 0, 0, 0, 1},
    {0, 0, 0, 1, 6, 0, 8, 0, 0},
    {6, 0, 0, 0, 0, 0, 0, 0, 0},
};
// main
int main() {
    Sudoku sudoku; 
    initSudokuAndValue(&sudoku, input); 
    printSudoku(sudoku);
    Sudoku result = solveSudoku(sudoku);
    printSudoku(result);
}

