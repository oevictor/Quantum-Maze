#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <time.h>
#include "mazeHelper.h"
#include <SFML/Graphics.hpp>



int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Node nodeList[GRID_WIDTH * GRID_HEIGHT];

    // pick a random starting cell
    int cur_col = std::rand() % GRID_WIDTH;
    int cur_row = std::rand() % GRID_HEIGHT;
    nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;

    // initialize frontier walls
    std::vector<Wall> wallVec;
    addWalls(wallVec, nodeList, cur_col, cur_row);

    // create SFML window
    sf::RenderWindow window(
        sf::VideoMode(
            sf::Vector2u(GRID_WIDTH * NODE_SIZE,
                         GRID_HEIGHT * NODE_SIZE)
        ),
        "Maze Generator"
    );

    while (window.isOpen()) {
        // SFML 3.x event loop
        while (auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // one step of the randomized‐wall algorithm
        if (!wallVec.empty()) {
            int idx = std::rand() % wallVec.size();
            Wall w = wallVec[idx];
            Node* a = w.node1;
            Node* b = w.node2;

            if ((a->visited && !b->visited) || (!a->visited && b->visited)) {
                joinNodes(nodeList, a, b);
                Node* next = a->visited ? b : a;
                next->visited = true;

                int ni = static_cast<int>(next - nodeList);
                cur_row = ni / GRID_WIDTH;
                cur_col = ni % GRID_WIDTH;
                addWalls(wallVec, nodeList, cur_col, cur_row);
            }

            wallVec.erase(wallVec.begin() + idx);
        }

        // draw
        window.clear(sf::Color::Black);
        drawMaze(window, nodeList, cur_col, cur_row);
        window.display();
    }

    return 0;
}
