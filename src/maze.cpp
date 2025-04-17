

#include <iostream>
#include <stack>
#include <time.h>
#include <SFML/Graphics.hpp>

#include "mazeHelper.h"

void addWalls(std::vector<Wall>* wallVec, Node nodeList[], int col, int row)
{
    Node* node1 = &nodeList[col + row * GRID_WIDTH];
    for (int side = 0; side < 4; ++side)
    {
        int next_col = nextCol(col, side);
        int next_row = nextRow(row, side);
        if (indexIsValid(next_col, next_row))
        {
            Wall wall;
            wall.node1 = node1;
            wall.node2 = &nodeList[next_col + next_row * GRID_WIDTH];
            wallVec->push_back(wall);
        }
    }
}

