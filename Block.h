#ifndef PA2_BLOCK_H
#define PA2_BLOCK_H

#include <vector>
#include <iostream>

using namespace std;

class Block {
public:

    vector<vector<bool>> shape; // Two-dimensional vector corresponding to the block's shape
    Block * right_rotation = nullptr; // Pointer to the block's clockwise neighbor block (its right rotation)
    Block * left_rotation = nullptr; // Pointer to the block's counter-clockwise neighbor block (its left rotation)
    Block * next_block = nullptr; // Pointer to the next block to appear in the game
    // Constructor to initialize the block with a given shape

    bool operator==(const Block& other) const {
        // TODO: Overload the == operator to compare two blocks based on their shapes
        return this->shape == other.shape;
    }

    bool operator!=(const Block& other) const {
        // TODO: Overload the != operator to compare two blocks based on their shapes
        return !(*this == other);
    }

};


#endif //PA2_BLOCK_H
