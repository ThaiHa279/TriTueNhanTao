#include <iostream>
#include <stack>
using namespace std; 

// dinh nghia
#define TankCapacity_X 9 
#define TankCapacity_Y 4
#define Empty 0
#define Goal 6
const char * action[] = {"Frist state", "pour water full X", "pour water full Y", 
                        "pour water empty X","pour water empty Y", "pour water X to Y", "pour water Y to X"}; 
// khai bao state
class State {
	private:
		int x, y;
	public: 
		State(State state) {
			x = state.x;
			y = state.y; 
		}
		void set(State state) {	
			x = state.x;
			y = state.y; 
		}
		void set(int a, int b) {
			x = a; y = b;
		}
		void edit_X(int x) {
			this->x = x;
		} 
		void edit_Y(int y) {
			this->y = y;
		} 
		void print() {
			cout << "   X: "<< x << " --- Y: "<< y; 
		}
		int compare(State state) {
			return (x == state.x && y == state.y); 
		}
		int goal() {
			return (x == Goal || y == Goal);
		}
		int full_X() {
			if (x != TankCapacity_X) {
				x = TankCapacity_X; 
				return 1;
			}
			return 0;
		}
		int full_Y() {
			if (y != TankCapacity_Y) {
				y = TankCapacity_Y; 
				return 1;
			}
			return 0;
		}
		int empty_X() {
			if (x != Empty) {
				x = Empty; 
				return 1;
			}
			return 0;
		}
		int empty_Y() {
			if (y != Empty) {
				y = Empty; 
				return 1;
			}
			return 0;
		}
		int transfer_XY() {
			int re_x = x; 
			int re_y = y; 
			if (x > Empty && y < TankCapacity_Y) {
				x = max(re_x - (TankCapacity_Y - re_y), Empty); 
				y = min(re_x + re_y, TankCapacity_Y);
				return 1;
			} 
			return 0;
		}
		int transfer_YX() {
			int re_x = x; 
			int re_y = y; 
			if (y > Empty && x < TankCapacity_X) {
				y = max(re_y - (TankCapacity_X - re_x), Empty); 
				x = min(re_x + re_y, TankCapacity_X);
				return 1;
			} 
			return 0;
		}
		int call_operator( int opt) {
			switch(opt) {
				case 1: return this->full_X(); 
				case 2: return this->full_Y(); 
				case 3: return this->empty_X(); 
				case 4: return this->empty_Y(); 
				case 5: return this->transfer_XY(); 
				case 6: return this->transfer_YX(); 
				default: 
					cout << "Invaild operator call!";
			}
		}
};

class Node{
	private: 
		int no_func; 
		State state; 
		Node* Parent; 
	public: 
		Node(Node* node) {
			no_func = node->no_func;
			state = node->state;
			Parent = node->Parent;
		}
		Node(Node *node, State sta, int number){
			Parent = node; 
			state = sta;
			no_func = number;  
		}
		State get_state() {
			return state; 
		}
		Node* get_parent() {
			return Parent; 
		}
		int get_no_func() {
			return no_func; 
		}
		void print_wayToGetGoal() {
			Node *node(this); 
		    stack<Node*> result; 
		    while (node->Parent != NULL) {
		        result.push(node);
		        node = node->Parent; 
		    }
		    result.push(node);
		    // in ra thu tu
		    int no_action = 0;
		    while (!result.empty()){
		        printf("\nAction %d: %s",no_action,action[result.top()->get_no_func()]);
		        result.top()->get_state().print();
		        no_action++;
		        result.pop();
		    }
	}	
};
class Stack: public stack<Node*> {
	public:
		int find(State state){
			stack<Node*> s = *this; 
			while (!s.empty()) {
				if (state.compare(s.top()->get_state())) return 1;
				s.pop();
			}
			return 0;
		}
};
Node::Node *DFS_Algorithm(State state) {
	// khai bao stack open close
	Stack open; 
	Stack close;
	// tao root -> open
	Node *root(NULL, state, 0);
	open.push(root);  
	// duyet
	while (!open.empty()) {
		// lay node trong open 
		Node *node(open.top());
		close.push(node);
		open.pop();
		// chay operator
		for (int i = 1; i<=6; i++){
			// kt newState
			State newState(node->get_state());
			if (newState.call_operator(i)) {
			  	if (open.find(newState) || close.find(newState)) {
			  		continue;
				}
				// them newNode vao open 
				Node* newNode(node,newState,i);
				open.push(newNode);
			}
		}
	}
}

int main() {
    State state; 
	state.set(0,0);
    Node* p;
	p->DFS_Algorithm(state);
    p->print_wayToGetGoal();
}	
