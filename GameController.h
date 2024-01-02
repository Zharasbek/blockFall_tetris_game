#ifndef PA2_GAMECONTROLLER_H
#define PA2_GAMECONTROLLER_H

#include "BlockFall.h"

using namespace std;

class GameController {
public:
    bool play(BlockFall &game, const string &commands_file); // Function that implements the gameplay

    void print_grid(vector<vector<int>> grid);
    bool addBlock(vector<vector<int>>& grid, Block * initial_block, int j);
    void rotateBlock(BlockFall& game, char direction);
    void writeBlock(vector<vector<int>>& grid, Block * block, int j);
    void deleteBlockFromGrid(vector<vector<int>>& grid, Block * block, int j);
    void appendGridToFile(const std::vector<std::vector<int>>& grid);
    void drop(vector<vector<int>>& grid, Block * block, int main_point);
    void deleteRow(vector<vector<int>>& grid, int rowIndex);
    bool checkRow(vector<int>& row);
    int  checkDrop(vector<vector<int>>& grid, Block* block);
    void gravity_drop(vector<vector<int>>& grid);
    int  checkCellDrop(vector<vector<int>>& grid ,int row, int col);
    bool canFitBlock(const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& shape);
    bool isValidRowIndex(int index, const vector<vector<int>>& gameGrid);
    void removeRowAndInsertEmpty(vector<vector<int>>& gameGrid, int rowIndex);
    bool gravity;
    bool temp_error;

private:
    int main_point;
};


#endif //PA2_GAMECONTROLLER_H
