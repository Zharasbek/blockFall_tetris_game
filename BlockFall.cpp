#include "BlockFall.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

vector<vector<int>> initial_block;

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    ifstream file(input_file);

    if (file.is_open()) {
        while (!file.eof()) {
            vector<vector<bool>> block_data;
            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    vector<bool> row;

                    for (char ch : line) {
                        if (ch == '1' || ch == '0') {
                            row.push_back({ch == '1'});
                        }
                    }
                    if (!row.empty()) {
                        block_data.push_back(row);
                    }

                    int pos = line.find(']');

                    if (pos != std::string::npos) {
                        break;
                    }
                }
                
            }

            if (!line.empty()) {
                Block* new_blk = new Block();
                new_blk->shape = block_data;
                insert_block(new_blk);

            }
            
        }
        delete_last_block();
        file.close();
    } else {
        cerr << "Error: Unable to open file " << input_file << endl;
    }

}



void BlockFall::insert_block(Block* new_blk) {
    Block* current_blk = initial_block;  
    
    if (current_blk == nullptr) {
        initial_block = new_blk;
        applyRotation(initial_block);
    } else {
        while (current_blk->next_block != nullptr) {
            current_blk = current_blk->next_block;
        }
        applyRotation(new_blk);

        current_blk->next_block = new_blk;
        current_blk->left_rotation->next_block = new_blk;
        current_blk->left_rotation->left_rotation->next_block = new_blk;
        current_blk->left_rotation->left_rotation->left_rotation->next_block = new_blk;

        power_up = new_blk->shape;
    }
}


void BlockFall::delete_last_block() {
    if (initial_block == nullptr || initial_block->next_block == nullptr) {
        // No or only one block in the list
        return;
    }

    Block sentinel; // Sentinel node
    sentinel.next_block = initial_block;

    Block* current = &sentinel;

    while (current->next_block->next_block != nullptr) {
        current = current->next_block;
    }

    Block* lastBlock = current->next_block;

    // Update pointers before deleting nodes
    Block* temp = lastBlock->right_rotation;
    Block* temp1 = nullptr;

    do {
        temp1 = temp->right_rotation;
        if (temp != lastBlock) {
            delete temp;
        }
        temp = temp1;
    } while (temp != lastBlock);

    // Delete the last block after updating pointers
    current->next_block = nullptr;
    delete lastBlock;
}



void BlockFall::applyRotation(Block* blk) {
    
    if(blk->shape != rotate(blk->shape)) {
        Block* leftBlk1 = new Block();
        leftBlk1->shape = rotate(blk->shape);

        blk->left_rotation = blk->right_rotation = leftBlk1;
        leftBlk1->left_rotation = leftBlk1->right_rotation = blk;

        if(blk->shape != rotate(leftBlk1->shape)) {
            Block* leftBlk2 = new Block();
            leftBlk2->shape = rotate(leftBlk1->shape);

            blk->right_rotation = leftBlk2;

            leftBlk1->left_rotation = leftBlk2;

            leftBlk2->left_rotation = blk;
            leftBlk2->right_rotation = leftBlk1;

            if(blk->shape != rotate(leftBlk2->shape)) {
                Block* leftBlk3 = new Block();
                leftBlk3->shape = rotate(leftBlk2->shape);

                blk->right_rotation = leftBlk3;
                
                leftBlk2->left_rotation = leftBlk3;

                leftBlk3->left_rotation = blk;
                leftBlk3->right_rotation = leftBlk2;
            }
        }
    } else {
        blk->left_rotation = blk;
        blk->right_rotation = blk;
    }
}


vector<vector<bool>> BlockFall::rotate(vector<vector<bool>> block_shape) {
    size_t rows = block_shape.size();
    size_t cols = block_shape[0].size();

    vector<vector<bool>> rotated_shape(cols, vector<bool>(rows, false));
    

    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols; ++j) {
            rotated_shape[j][i] = block_shape[i][j];
        }
    }

    vector<vector<bool>> result_shape(rotated_shape.rbegin(), rotated_shape.rend());
    return result_shape;
}



void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    // Open the file
    std::ifstream inputFile(input_file);
    rows = cols = 0;
    // Check if the file is open
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << input_file << std::endl;
    }

    // Read each line from the file
    std::string line;
    while (std::getline(inputFile, line)) {
        // Increment the number of rows
        rows++;
        // Create a stringstream from the line
        std::istringstream iss(line);

        // Create a vector to store the values in the current line
        std::vector<int> row;

        // Read each integer from the stringstream and add it to the row vector
        int value;
        while (iss >> value) {
            row.push_back(value);
        }

        // If this is the first line, save the number of columns
        if (rows == 1) {
            cols = row.size();
        }

        // Add the row vector to the grid
        grid.push_back(row);
    }
    // Close the file
    inputFile.close();
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks

    if(initial_block != nullptr){
        Block* currentBlock = initial_block;
        Block* temp = nullptr;
        Block* temp1 = nullptr;

        while (currentBlock != nullptr) {
            temp = currentBlock->right_rotation;

            do {
                temp1 = temp->right_rotation;
                if (temp != currentBlock) {
                    delete temp;
                    temp = nullptr;
                }
                temp = temp1;
            } while (temp != currentBlock);

            Block* nextBlock = currentBlock->next_block;

            delete temp;
            temp = nullptr;

            currentBlock = nextBlock;
        }

        initial_block = nullptr;
        temp = nullptr;
        temp1 = nullptr;


    }

}
