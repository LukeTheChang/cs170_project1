#include <iostream>
#include <vector>
#include <string>

class puzzle_class {
    public:
        std::vector<int> puzzle(8,0);

        puzzle_class() {
            puzzle = {1,2,3,4,0,5,6,7,8};
        }

        puzzle_class(std::vector<int> puzzle);
        void move_up(int tile);
        void move_down(int tile);
        void move_left(int tile);
        void move_right(int tile);
        void print_puzzle();
};

