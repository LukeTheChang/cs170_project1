#include <iostream>
#include <string>
#include <vector>
#include <queue>

//using namespace std;void search_alg(std::vector<int> prob, int queueing_function);
/* std::vector<int> move_up(std::vector<int> puzzle, int tile);
std::vector<int> move_down(std::vector<int> puzzle, int tile);
std::vector<int> move_left(std::vector<int> puzzle, int tile);
std::vector<int> move_right(std::vector<int> puzzle, int tile);
void print_puzzle(std::vector<int> puzzle);
int offBy(std::vector<int> input); */

class node {
    public:
        std::vector<int> state;
        int weight = 0; //accumulated cost g(n)
        int heur = 0; //heuristic h(n)

        //basically check if 'tile' is zero and the tile above is within boundary of the puzzle; if so, swap tiles with the tile above
        void move_up(int tile) {
            if( (state.at(tile) == 0) && (tile-3) >= 0) {
                state.at(tile) = state.at(tile-3);
                state.at(tile-3) = 0;
                ++weight;
            } else {
                return;
            }
        }

        void move_down( int tile) {
            if( (state.at(tile) == 0) && (tile+3) < 8) {
                state.at(tile) = state.at(tile+3);
                state.at(tile+3) = 0;
                ++weight;
            } else {
                return;
            }
        }

        void move_left( int tile) {
            if( (tile%3==0) ) {
                return;
                //forgot to use return rather than exit(1)
            }
            if ( state.at(tile) == 0 ) {
                state.at(tile) = state.at(tile-1);
                state.at(tile-1) = 0;
                ++weight;
            } else {
                return;
            }
        }

        void move_right( int tile) {
            if( ((tile+1)%3==0) ) {
                return;
            }
            if ( state.at(tile) == 0 ) {
                state.at(tile) = state.at(tile+1);
                state.at(tile+1) = 0;
                ++weight;
            } else {
                return;
            }
        }

        void print_puzzle() const {
            for(int i = 0; i < state.size(); ++i) {
                if(i == 0) {
                    std::cout << state.at(i) << ", ";
                    continue;
                }
                int x = i + 1;
                if(x%3 == 0) {
                    std::cout << state.at(i) << std::endl;
                }
                else {
                    std::cout << state.at(i) << ", ";
                }
            }
        }

        void misplaced() {
            std::vector<int> solution = {1,2,3,4,5,6,7,8,0};
            int off_by = 0; //essentially g(n) - how for off we are from the ideal state; how many more moves are needed to reach solution state
            for(int i = 0; i < solution.size(); ++i) {
                if (solution.at(i) != state.at(i)) {
                    //std::cout << solution.at(i) << " vs " << input.at(i) << std::endl;
                    ++off_by;
                }
            }
            heur = off_by;
        } 
};

struct compare_weight {
    bool operator()(node n1, node n2) {
        return n1.weight < n2.weight;
    }
};

struct compare_heur {
    bool operator()(node n1, node n2) {
        return n1.heur < n2.heur;
    }
};


void search_alg(std::vector<int> prob, int queueing_function);
std::priority_queue<node, std::vector<node>, compare_heur> add_children(std::priority_queue<node, std::vector<node>, compare_heur> root);

//unnecessary
/*class tree {
    public:
        std::priority_queue<node, std::vector<node>, compare_heur> base; //top is parent puzzle and everything below is the possible movements

        void add_children() {

        }
}; */ 

void search_algo(node prob, int queueing_function) {
    //pseudo code from slides
    /*function general-search(problem, QUEUEING-FUNCTION)  
        nodes = MAKE-QUEUE(MAKE-NODE(problem.INITIAL-STATE)) 
        loop do
            if EMPTY(nodes) then return "failure" (we have proved there is no solution!)
                node = REMOVE-FRONT(nodes) 
            if problem.GOAL-TEST(node.STATE) succeeds then return node
                nodes = QUEUEING-FUNCTION(nodes, EXPAND(node, problem.OPERATORS))  
    end */
    //std::priority_queue<node, compare_heur> search;
    //search.push(prob);
    std::vector<int> sol_state = {1,2,3,4,5,6,7,8,0};
    node curr;
    if(queueing_function == 1) { //UCS search
        //std::cout << "test" << std::endl;
        std::priority_queue<node, std::vector<node>, compare_heur> ucs_tree;
        ucs_tree.push(prob);
        
        //std::cout << "test" << std::endl;
        do {
            //std::cout << "test" << std::endl;
            if(ucs_tree.empty()) {
                std::cout << "no solution found" << std::endl;
                break;
            }
            //std::cout << "test" << std::endl;
            node curr = ucs_tree.top();
            if (curr.state == sol_state) {
                std::cout << "solution found at depth or weight: " << curr.weight << std::endl;
                ucs_tree.top().print_puzzle();
                return;
            } else {
                std::cout << "current puzzle is the below with weight: " << ucs_tree.top().weight << std::endl;
                ucs_tree.top().print_puzzle();
            }
            ucs_tree = add_children(ucs_tree);
        } while(!ucs_tree.empty());
    }
}

std::priority_queue<node, std::vector<node>, compare_heur> add_children(std::priority_queue<node, std::vector<node>, compare_heur> root) {
    std::priority_queue<node, std::vector<node>, compare_heur> new_tree = root;
    node base = new_tree.top();
    int i = 0;
    new_tree.pop(); //remove root since it was scanned and deemed not to be the desired state
    int find_zero = 0;
    for(i = 0; i < base.state.size(); ++i) {
        if (base.state.at(i) == 0) {
            find_zero = i;
            break;
        }
    }

    //add duplicates of parent to be further modified with node's move operators
    node up = base;
    up.move_up(find_zero);
    node down = base;
    down.move_down(find_zero);
    node left = base;
    left.move_left(find_zero);
    node right = base;
    right.move_right(find_zero);

    //check if puzzle has been modified; if so, push up to queue or tree
    if(up.state != base.state) {new_tree.push(up);}
    if(down.state != base.state) {new_tree.push(down);}
    if(left.state != base.state) {new_tree.push(left);}
    if(right.state != base.state) {new_tree.push(right);} 

    return new_tree;
}

int main() {
    int default_puzzle = 1;
    int search_alg = 0;
    std::vector<int> puzzle = {1, 2, 3, 4, 0, 5, 6, 7, 8}; //default puzzle
    node basic;
    int x, y, z;

    while (1)
    {
        std::cout << "Type \"1\" to use a default puzzle, or \"2\" to enter your own puzzle: ";
        std::cin >> default_puzzle;
        if(default_puzzle == 2) {
            std::cout << "custom\n";
            std::cout << "Enter your puzzle, use zero to represent the blank: \n";
            for(int i = 0; i < 3; ++i) {
                std::cout << "Enter the first row's values': \n";
                std::cin >> x;
                puzzle.at(i) = x;
            }
            for(int i = 3; i < 6; ++i) {
                std::cout << "Enter the second row's values': \n";
                std::cin >> x;
                puzzle.at(i) = x;
            }
            for(int i = 6; i < 9; ++i) {
                std::cout << "Enter the third row's values': \n";
                std::cin >> x;
                puzzle.at(i) = x;
            }
            basic.state = puzzle;
            break;
        }   else if (default_puzzle == 1) {
            basic.state = puzzle;
            std::cout << "default\n";
            break;
        }
    }

    while(1) {
        std::cout << "Enter your choice of algorithm\n";
        std::cout << "\t 1. Uniform Cost Search \n";
        std::cout << "\t 2. A* with the Misplaced Tile heuristic \n";
        std::cout << "\t 3. A* with the Manhattan distance heuristic \n";
        std::cin >> search_alg;

        if(search_alg == 1 || search_alg == 2 || search_alg == 3) {
            break;
        }
    }
    
    /* std::vector<int> mod_puzzle = move_up(puzzle,7);
    print_puzzle(basic.state);
    std::cout << "default off by: " << offBy(puzzle) << std::endl; */
    
    //std::priority_queue<node, std::vector<node>, compare_heur> ucs_tree;
    //ucs_tree.push(basic);
    //std::cout << "test1" << std::endl;
    //basic.move_up(7);
    //std::cout << "test2" << std::endl;
    //ucs_tree.push(basic);
    //std::cout << "test3" << std::endl;
    /*ucs_tree.top().print_puzzle();
    ucs_tree.pop();
    ucs_tree.top().print_puzzle(); */

    search_algo(basic, search_alg);
    
}



