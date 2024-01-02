#include "GameController.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-command
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    gravity = game.gravity_mode_on;
    ifstream file(commands_file);

    if (file.is_open()) {
        Block* currentBlock = game.initial_block;
        game.active_rotation = currentBlock;
        main_point = 0;
        if(addBlock(game.grid, game.active_rotation, main_point)){
            return false;
        }
        writeBlock(game.grid, game.active_rotation, main_point);
        vector<vector<bool>> block;
        std::string command;
        while (std::getline(file, command)) {
            temp_error = false;
            
            if(command == "DROP"){
                drop(game.grid, game.active_rotation, main_point);
                if(gravity) {
                    gravity_drop(game.grid);
                }
                
                if(game.active_rotation->next_block == nullptr){
                    return true;
                }
                game.active_rotation = game.active_rotation->next_block;
                main_point = 0;
                temp_error = true;

            } 
            else if(command == "GRAVITY_SWITCH") {
                if(game.gravity_mode_on) {
                    game.gravity_mode_on = false;
                    gravity = game.gravity_mode_on;
                } else {
                    game.gravity_mode_on = true;
                    gravity = game.gravity_mode_on;
                    gravity_drop(game.grid);
                    for(int i = 0; i < game.grid.size(); i++) {
                        if(checkRow(game.grid[i])) {
                            removeRowAndInsertEmpty(game.grid, i);
                        }
                    }
                }
            }
            else if(command == "ROTATE_RIGHT"){
                deleteBlockFromGrid(game.grid, game.active_rotation, main_point);
                rotateBlock(game, 'r');
                if(main_point + game.active_rotation->shape[0].size() <= game.cols) {
                    if(addBlock(game.grid, game.active_rotation, main_point)){
                        rotateBlock(game, 'l');
                        writeBlock(game.grid, game.active_rotation, main_point);
                        
                    }else {
                        writeBlock(game.grid, game.active_rotation, main_point);
                    }
                } else {
                    rotateBlock(game, 'l');
                    writeBlock(game.grid, game.active_rotation, main_point);
                }
            }
            else if(command == "ROTATE_LEFT"){
                deleteBlockFromGrid(game.grid, game.active_rotation, main_point);
                rotateBlock(game, 'l');
                if(main_point + game.active_rotation->shape[0].size() <= game.cols) {
                    if(addBlock(game.grid, game.active_rotation, main_point)){
                        rotateBlock(game, 'r');
                        writeBlock(game.grid, game.active_rotation, main_point);
                        
                    }else {
                        writeBlock(game.grid, game.active_rotation, main_point);
                    } 
                } else {
                    rotateBlock(game, 'r');
                    writeBlock(game.grid, game.active_rotation, main_point);                    
                }
            }
            else if(command == "MOVE_RIGHT"){
                if(main_point + game.active_rotation->shape[0].size() < game.cols) {
                    deleteBlockFromGrid(game.grid, game.active_rotation, main_point);
                    main_point++;
                    if(addBlock(game.grid, game.active_rotation, main_point)) {
                        main_point--;
                        writeBlock(game.grid, game.active_rotation, main_point);
                        
                    } else {
                        writeBlock(game.grid, game.active_rotation, main_point);
                    }
                }
            }
            else if(command == "MOVE_LEFT"){
                if(main_point - 1 >= 0) {
                    deleteBlockFromGrid(game.grid, game.active_rotation, main_point);
                    main_point--;
                    if(addBlock(game.grid, game.active_rotation, main_point)) {
                        main_point++;
                        writeBlock(game.grid, game.active_rotation, main_point);
                        
                    } else {
                        writeBlock(game.grid, game.active_rotation, main_point);
                    }
                }
            }
            
            
            
            if (file && game.active_rotation->next_block != nullptr && !canFitBlock(game.grid, game.active_rotation->next_block->shape) && temp_error){
                return false;
            }
        }

            
        
        
        file.close();
        if (game.active_rotation->next_block != nullptr) {
            return true;
        }
    }

    return false;
}




int GameController::checkCellDrop(vector<vector<int>>& grid, int row, int col) {
    size_t i;
    size_t gridRows = grid.size();

    for (i = 1; i < gridRows - row; ++i) {
        size_t nextRow = i + row;
        if (nextRow < gridRows && grid[nextRow][col] == 1) {
            return static_cast<int>(nextRow - 1);
        }
    }

    return static_cast<int>(i + row - 1);
}


bool GameController::addBlock(vector<vector<int>>& grid, Block* block, int main_point) {
    const auto& shape = block->shape;

    for (size_t i = 0; i < shape.size(); ++i) {
        int j = main_point;
        for (size_t k = 0; k < shape[0].size(); ++k) {
            if (shape[i][k] && grid[i][j] == 1) {
                return true;
            }
            ++j;
        }
    }
    return false;
}


void GameController::writeBlock(vector<vector<int>>& grid, Block* block, int main_point) {
    const auto& shape = block->shape;

    for (size_t i = 0; i < shape.size(); ++i) {
        int j = main_point;
        for (size_t k = 0; k < shape[0].size(); ++k) {
            if (shape[i][k]) {
                grid[i][j] = 1;
            }
            ++j;
        }
    }
}


void GameController::deleteBlockFromGrid(vector<vector<int>>& grid, Block* block, int main_point) {
    const auto& shape = block->shape;

    for (size_t i = 0; i < shape.size(); ++i) {
        int j = main_point;
        for (size_t k = 0; k < shape[0].size(); ++k) {
            if (shape[i][k]) {
                grid[i][j] = 0;
            }
            ++j;
        }
    }
}

void GameController::rotateBlock(BlockFall& game, char rotationDirection) {
    if (rotationDirection == 'r') {
        game.active_rotation = game.active_rotation->right_rotation;
    } else if (rotationDirection == 'l') {
        game.active_rotation = game.active_rotation->left_rotation;
    }
}

void GameController::drop(vector<vector<int>>& grid, Block * block, int main_point){

    std::ofstream outputFile("output.txt", std::ios::app);

    deleteBlockFromGrid(grid, block, main_point);

    int rowIndex = checkDrop(grid, block);
    
    for(int i=0; i<block->shape.size(); i++){
        for(int j=0; j<block->shape[0].size(); j++){
            if(block->shape[i][j]){
                grid[i+rowIndex][j+main_point] = 1;
            }
        }
    }

    if(gravity) {
        gravity_drop(grid);
        for(int i = 0; i < grid.size(); i++) {
        if(checkRow(grid[i])) {
            removeRowAndInsertEmpty(grid, i);
        }
    }
    }


    for(int i = 0; i < grid.size(); i++) {
        if(checkRow(grid[i])) {
            removeRowAndInsertEmpty(grid, i);
        }
    }
    
}

void GameController::gravity_drop(vector<vector<int>>& grid) {
    size_t numRows = grid.size();
    size_t numCols = grid[0].size();

    for (size_t i = numRows - 2; i != static_cast<size_t>(-1); --i) {
        for (size_t j = 0; j < numCols; ++j) {
            if (grid[i][j] == 1) {
                grid[i][j] = 0;
                size_t row = checkCellDrop(grid, static_cast<int>(i), static_cast<int>(j));
                grid[row][j] = 1;
            }
        }
    }
}

int GameController::checkDrop(vector<vector<int>>& grid, Block* block) {
    int h = 1;
    int need = 1;

    for(int h = 1; h < grid.size() - block->shape.size() + 1; h++) {
        for(int i = 0; i < block->shape.size(); i++) {
            for(int j = 0; j < block->shape[0].size(); j++) {
                if(block->shape[i][j] && grid[i+h][j+main_point] == 1) {
                    return need - 1;
                }
            }
        }
        need++;
    }

    return need - 1;
}

void GameController::appendGridToFile(const std::vector<std::vector<int>>& grid) {
    // Open the output file in append mode
    std::ofstream outputFile("output.txt", std::ios::app);

    // Check if the file is opened successfully
    if (!outputFile.is_open()) {
        std::cerr << "Error: Unable to open output.txt" << std::endl;
        return;
    }

    // Iterate through the rows and columns of the grid and write to the file
    for (const auto& row : grid) {
        for (int value : row) {
            outputFile << value << " ";
        }
        outputFile << std::endl; // Move to the next command for the next row
    }


    // Close the output file
    outputFile.close();
}

void GameController::removeRowAndInsertEmpty(vector<vector<int>>& gameGrid, int rowIndex) {
    if (isValidRowIndex(rowIndex, gameGrid)) {
        gameGrid.erase(gameGrid.begin() + rowIndex);
        gameGrid.insert(gameGrid.begin(), std::vector<int>(gameGrid[0].size(), 0));
    }
}

bool GameController::isValidRowIndex(int index, const vector<vector<int>>& gameGrid) {
    return (index >= 0 && index < gameGrid.size());
}


bool GameController::checkRow(vector<int>& row) {
    return std::all_of(row.begin(), row.end(), [](int value) { return value == 1; });
}

bool GameController::canFitBlock(const std::vector<std::vector<int>>& grid, const std::vector<std::vector<bool>>& shape) {
    int blockRows = shape.size();
    int blockCols = shape[0].size();

    // Unrolled loop for potential optimization
    for (int i = 0; i < blockRows; ++i) {
        for (int j = 0; j < blockCols; j += 2) {
            if (shape[i][j] && grid[i][j] == 1) {
                return false;
            }

            // Check the next element in the row
            int nextJ = j + 1;
            if (nextJ < blockCols && shape[i][nextJ] && grid[i][nextJ] == 1) {
                return false;
            }
        }
    }

    return true;
}

