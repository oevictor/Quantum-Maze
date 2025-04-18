// /******************************************************************************/
// /*  mazeHelper.cpp                                                             */
// /*                                                                            */
// /*  Helper utilities for generating, manipulating and rendering a 2‑D maze    */
// /*  with SFML.  The file complements `mazeHelper.h` by providing the concrete  */
// /*  implementations of the drawing routines, neighbour calculations and wall  */
// /*  manipulation helpers used by a depth‑first / Prim / Kruskal style maze    */
// /*  generator.                                                                 */
// /*                                                                            */
// /*  Coordinate system                                                         */
// /*  ------------------------------------------------------------------------  */
// /*      col →  (x)                                                             */
// /*      row ↓  (y)                                                             */
// /*                                                                            */
// /*      GRID_WIDTH   : number of columns (declared in mazeHelper.h)           */
// /*      GRID_HEIGHT  : number of rows     (declared in mazeHelper.h)          */
// /*      NODE_SIZE    : side length of a single cell in pixels                 */
// /*                                                                            */
// /*  Public types (declared in mazeHelper.h)                                   */
// /*  ------------------------------------------------------------------------  */
// /*      struct Node                                                           */
// /*      {                                                                     */
// /*          bool  walls[4];      // TOP, RIGHT, DOWN, LEFT                    */
// /*          ...                                                               */
// /*      };                                                                    */
// /*      struct Wall                                                           */
// /*      {                                                                     */
// /*          Node* a;                    first  node of an edge                  */
// /*          Node* b;                   second node of an edge                  */                                            */
// /******************************************************************************/

#include <SFML/Graphics.hpp>
#include <vector>
#include "mazeHelper.h"

/* ------------------------------------------------------------------------- */
/* addWalls                                                                  */
/* ------------------------------------------------------------------------- */
/** Push every still‑standing wall that surrounds (col,row) into `wallVec`.
 *  The routine does **not** check if the wall already exists in the vector;
 *  it is the caller’s responsibility to avoid duplicates if required.
 *
 *  @param wallVec   Vector that will receive newly discovered walls.
 *  @param nodeList  Flat array that stores the entire maze grid.
 *  @param col,row   Grid coordinates of the current cell.
 */
void addWalls(std::vector<Wall>& wallVec, Node nodeList[], int col, int row)
{
    Node* base = &nodeList[col + row * GRID_WIDTH];

    for (int side = 0; side < 4; ++side)
    {
        int nc = nextCol(col, side);
        int nr = nextRow(row, side);

        if (indexIsValid(nc, nr))
        {
            wallVec.push_back(
                Wall{ base, &nodeList[nc + nr * GRID_WIDTH] }
            );
        }
    }
}

/* ------------------------------------------------------------------------- */
/* drawNode                                                                  */
/* ------------------------------------------------------------------------- */
/** Render a single maze cell *and* its knocked‑down walls.
 *
 *  A cell is drawn as a smaller white (or red, if it is the “current”
 *  cursor position) square centred inside the full node area.  Missing
 *  walls are drawn as rectangles that fill the corresponding gap between
 *  adjacent cells, visually merging passages.
 *
 *  @param window    SFML render target.
 *  @param nodeList  Flat array that stores the entire maze grid.
 *  @param col,row   Grid coordinates of the cell to draw.
 *  @param isCurrent If true, the inner square is tinted red.
 */
void drawNode(sf::RenderWindow& window,
              Node          nodeList[],
              int           col,
              int           row,
              bool          isCurrent)
{
    Node& n = nodeList[col + row * GRID_WIDTH];

    /* Only render interior if at least one wall has been removed        */
    if (!(n.walls[0] && n.walls[1] && n.walls[2] && n.walls[3]))
    {
        constexpr float scale     = 0.6f;                      // inner square %
        const     float innerSize = NODE_SIZE * scale;
        const     float thick     = (NODE_SIZE - innerSize) / 2.0f;

        /* Draw inner square ------------------------------------------ */
        sf::RectangleShape cell(sf::Vector2f(innerSize, innerSize));
        cell.setPosition(
            { col * NODE_SIZE + thick,
              row * NODE_SIZE + thick }
        );
        cell.setFillColor(isCurrent ? sf::Color::Red : sf::Color::White);
        window.draw(cell);

        /* Draw every absent wall ------------------------------------- */
        sf::RectangleShape wall;
        wall.setFillColor(sf::Color::White);

        // TOP
        if (!n.walls[SIDE_TOP])
        {
            wall.setSize({ innerSize, thick });
            wall.setPosition(
                { col * NODE_SIZE + thick,
                  row * NODE_SIZE }
            );
            window.draw(wall);
        }

        // RIGHT
        if (!n.walls[SIDE_RIGHT])
        {
            wall.setSize({ thick, innerSize });
            wall.setPosition(
                { col * NODE_SIZE + thick + innerSize,
                  row * NODE_SIZE + thick }
            );
            window.draw(wall);
        }

        // DOWN
        if (!n.walls[SIDE_DOWN])
        {
            wall.setSize({ innerSize, thick });
            wall.setPosition(
                { col * NODE_SIZE + thick,
                  row * NODE_SIZE + thick + innerSize }
            );
            window.draw(wall);
        }

        // LEFT
        if (!n.walls[SIDE_LEFT])
        {
            wall.setSize({ thick, innerSize });
            wall.setPosition(
                { col * NODE_SIZE,
                  row * NODE_SIZE + thick }
            );
            window.draw(wall);
        }
    }
}

/* ------------------------------------------------------------------------- */
/* drawMaze                                                                  */
/* ------------------------------------------------------------------------- */
/** Convenience wrapper that iterates over the whole grid and invokes
 *  `drawNode` for every cell.
 *
 *  @param window    SFML render target.
 *  @param nodeList  Flat array with GRID_WIDTH*GRID_HEIGHT nodes.
 *  @param curCol,row Coordinates of the “current” cell (highlighted red).
 */
void drawMaze(sf::RenderWindow& window,
              Node          nodeList[],
              int           curCol,
              int           curRow)
{
    for (int r = 0; r < GRID_HEIGHT; ++r)
        for (int c = 0; c < GRID_WIDTH; ++c)
            drawNode(window, nodeList, c, r, (c == curCol && r == curRow));
}

/* ------------------------------------------------------------------------- */
/* Utility helpers                                                           */
/* ------------------------------------------------------------------------- */

/** Check whether (col,row) lies inside the grid. */
bool indexIsValid(int col, int row)
{
    return col >= 0 && col < GRID_WIDTH &&
           row >= 0 && row < GRID_HEIGHT;
}

/** Compute neighbour column from a side constant. */
int nextCol(int cur_col, int side)
{
    if (side == SIDE_RIGHT) return cur_col + 1;
    if (side == SIDE_LEFT)  return cur_col - 1;
    return cur_col;
}

/** Compute neighbour row from a side constant. */
int nextRow(int cur_row, int side)
{
    if (side == SIDE_DOWN) return cur_row + 1;
    if (side == SIDE_TOP)  return cur_row - 1;
    return cur_row;
}

/** Given two node indices, return which side of *idx1* touches *idx2*.
 *  Returns ‑1 when the nodes are not orthogonally adjacent.
 */
int connectingSide(int idx1, int idx2)
{
    int diff = idx2 - idx1;
    if (diff ==  1)          return SIDE_RIGHT;
    if (diff == -1)          return SIDE_LEFT;
    if (diff ==  GRID_WIDTH) return SIDE_DOWN;
    if (diff == -GRID_WIDTH) return SIDE_TOP;
    return -1;    // nodes are not neighbours
}

/* ------------------------------------------------------------------------- */
/* joinNodes                                                                 */
/* ------------------------------------------------------------------------- */
/** Knock down the common wall between two adjacent cells.
 *
 *  The function first determines the relative position of *n2* with respect
 *  to *n1*, clears the wall flag on both nodes, and yields a continuous
 *  passage.
 *
 *  @param nodeList Global node array (needed to compute indices).
 *  @param n1,n2    Pointers to the two cells that will be joined.
 */
void joinNodes(Node nodeList[], Node* n1, Node* n2)
{
    int i1   = static_cast<int>(n1 - nodeList);
    int i2   = static_cast<int>(n2 - nodeList);
    int side = connectingSide(i1, i2);

    if (side < 0) return;            // not adjacent – nothing to do

    n1->walls[side]           = false;          // remove wall from n1
    n2->walls[(side + 2) % 4] = false;          // remove opposite wall
}
