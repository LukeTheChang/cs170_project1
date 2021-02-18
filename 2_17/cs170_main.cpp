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
void search_alg(std::vector<int> prob, int queueing_function);

class node {
    public:
        std::vector<int> state;
        int weight = 0; //accumulated cost g(n)
        int heur = 0; //heuristic h(n)

        //basically check if 'tile' is not zero and the tile above is; if so, swap tiles with the tile above
        void move_up(int tile) {
            if( (tile-3) < 0) {
                exit(1);
            }
            if( (state.at(tile) != 0) && (state.at(tile-3) == 0)) {
                state.at(tile-3) = state.at(tile);
                state.at(tile) = 0;
                ++weight;
            } else {
                exit(1);
            }
        }

        void move_down( int tile) {
            if( (tile+3) < 8) {
                exit(1);
            }
            if( (state.at(tile) != 0) && (state.at(tile+3) == 0)) {
                state.at(tile+3) = state.at(tile);
                state.at(tile) = 0;
                ++weight;
            } else {
                exit(1);
            }
        }

        void move_left( int tile) {
            if( (tile%3==0) ) {
                exit(1);
            }
            if ( (state.at(tile) != 0) && (state.at(tile-1) == 0) ) {
                state.at(tile-1) = state.at(tile);
                state.at(tile) = 0;
                ++weight;
            } else {
                exit(1);
            }
        }

        void move_right( int tile) {
            if( ((tile+1)%3==0) ) {
                exit(1);
            }
            if ( (state.at(tile) != 0) && (state.at(tile+1) == 0) ) {
                state.at(tile+1) = state.at(tile);
                state.at(tile) = 0;
                ++weight;
            } else {
                exit(1);
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
//unnecessary
/* class tree {
    public:
        std::queue<node> base; //top is parent puzzle and everything below is the possible movements
}; */

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
    
    std::priority_queue<node, std::vector<node>, compare_heur> ucs_tree;
    ucs_tree.push(basic);
    basic.move_up(7);
    ucs_tree.push(basic);
    ucs_tree.top().print_puzzle();
    ucs_tree.pop();
    ucs_tree.top().print_puzzle();
}

void search_alg(node prob, int queueing_function) {
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
        std::priority_queue<node, std::vector<node>, compare_heur> ucs_tree;
        ucs_tree.push(prob);
        do {
            if(ucs_tree.empty()) {
                break;
            }
            node curr = ucs_tree.top();
            if (curr.state == sol_state) {
                std::cout << "solution found at depth or weight: " << curr.weight << std::endl;
                exit(1);
            }

        } while(!ucs_tree.empty());
    }
}