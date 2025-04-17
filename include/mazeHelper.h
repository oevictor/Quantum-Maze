// include/mazeHelper.h
#ifndef MAZE_HELPER_H
#define MAZE_HELPER_H

#include <SFML/Graphics.hpp> // For graphics rendering
#include <vector>            // For std::vector usage

// Maze grid dimensions
constexpr int GRID_WIDTH  = 30;  // Number of columns
constexpr int GRID_HEIGHT = 20;  // Number of rows
constexpr int NODE_SIZE   = 15;  // Pixel size of each cell

// Direction indices for cell walls
enum {
    SIDE_RIGHT = 0, // Right wall
    SIDE_DOWN  = 1, // Bottom wall
    SIDE_LEFT  = 2, // Left wall
    SIDE_TOP   = 3  // Top wall
};

/// Represents a single maze cell
struct Node {
    bool walls[4] = { true, true, true, true }; // Walls [right, bottom, left, top]
    bool visited  = false; // Visitation flag for maze generation
};

/// Represents a wall between two adjacent cells
struct Wall {
    Node* node1; // First adjacent cell
    Node* node2; // Second adjacent cell
};

/// Adds walls of a specific cell to the wall list
/// @param wallVec Target vector to store walls
/// @param nodeList Array of all grid cells
/// @param col Column of target cell
/// @param row Row of target cell
void addWalls(std::vector<Wall>& wallVec, Node nodeList[], int col, int row);

/// Draws the entire maze grid and highlights current cell
/// @param window SFML render window
/// @param nodeList Array of all cells
/// @param curCol Current cell column (for highlighting)
/// @param curRow Current cell row (for highlighting)
void drawMaze(sf::RenderWindow& window, Node nodeList[], int curCol, int curRow);

/// Draws a single cell and its walls
/// @param window SFML render window
/// @param nodeList Array of cells
/// @param col Cell column
/// @param row Cell row
/// @param isCurrent If true, highlights cell in blue
void drawNode(sf::RenderWindow& window, Node nodeList[], int col, int row, bool isCurrent = false);

/// Validates grid coordinates
/// @param col Column to check
/// @param row Row to check
/// @return True if (col,row) is within grid bounds
bool indexIsValid(int col, int row);

/// Calculates adjacent column based on direction
/// @param cur_col Current column
/// @param side Direction (use enum values)
/// @return New column or -1 for invalid movement
int nextCol(int cur_col, int side);

/// Calculates adjacent row based on direction
/// @param cur_row Current row
/// @param side Direction (use enum values)
/// @return New row or -1 for invalid movement
int nextRow(int cur_row, int side);

/// Finds connecting wall between two adjacent cells
/// @param idx1 First cell index
/// @param idx2 Second cell index
/// @return Connecting wall side (enum value) or -1 if not adjacent
int connectingSide(int idx1, int idx2);

/// Removes walls between two adjacent cells
/// @param nodeList Array of cells
/// @param n1 First cell
/// @param n2 Second cell
void joinNodes(Node nodeList[], Node* n1, Node* n2);

#endif // MAZE_HELPER_H