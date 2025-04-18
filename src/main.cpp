// ============================================================================
// main.cpp — Entry point for “Labyrinth: Classical vs Quantum” demo
//
// This program opens an SFML 3 window, instantiates a classical and a quantum
// particle, and steps them through a (currently static) maze grid.  The
// classical particle obeys Newtonian kinematics, while the quantum particle
// performs a discrete quantum walk and can be collapsed with the SPACE key.
//
// Keyboard controls:
//   • SPACE  — collapse the quantum particle’s probability field
//   • window close button / Alt+F4 — exit
//
// Build requirements:
//   • C++17 (or later)
//   • SFML 3 (graphics, window, system)
//
// ============================================================================

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <time.h>
#include "mazeHelper.h"              // grid constants, Node, drawMaze()
#include <SFML/Graphics.hpp>
#include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle

/**
 * @brief Program entry point.
 *
 * 1. Creates an SFML window sized to the maze grid.
 * 2. Spawns one ClassicalParticle and one QuantumParticle.
 * 3. Runs the main loop: event processing → simulation update → rendering.
 *
 * @return `int` — exit status (0 = success).
 */
int main()
{
    // ---------------------------------------------------------------------
    // Window creation
    // ---------------------------------------------------------------------
    sf::RenderWindow window(
        sf::VideoMode(sf::Vector2u(GRID_WIDTH * NODE_SIZE,
                                    GRID_HEIGHT * NODE_SIZE)),
        "Labyrinth: Classical vs Quantum");
   
        
    window.setSize({640, 480});

    auto desktopMode = sf::VideoMode::getDesktopMode();
    sf::Vector2u desktopSize = desktopMode.size;  
    sf::Vector2u winSize = window.getSize();
    window.setPosition(sf::Vector2i(
        static_cast<int>((desktopSize.x - winSize.x) / 2),
        static_cast<int>((desktopSize.y - winSize.y) / 2)
    ));
    // ---------------------------------------------------------------------
    // Maze initialisation (placeholder — generation disabled for now)
    // ---------------------------------------------------------------------
    Node nodeList[GRID_WIDTH * GRID_HEIGHT];
    // initializeGrid(nodeList);
    // generateMaze(nodeList);

    // pick a random starting cell
    int cur_col = std::rand() % GRID_WIDTH;
    int cur_row = std::rand() % GRID_HEIGHT;
    nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;

    // initialize frontier walls
    std::vector<Wall> wallVec;
    addWalls(wallVec, nodeList, cur_col, cur_row);
    // ---------------------------------------------------------------------
    // Particle instantiation
    // ---------------------------------------------------------------------
    ClassicalParticle classical{
        sf::Vector2f(0.f, 0.f),   // initial position (top‑left corner)
        sf::Vector2f(5.f, 1.f),   //(x,y) velocity (5 px/s to the right)
        sf::Vector2f(0.f, 0.f)    // acceleration
    };

    QuantumParticle quantum;
    quantum.initialize();

    sf::Clock clock; // used to compute per‑frame Δt

    // ---------------------------------------------------------------------
    // Main loop
    // ---------------------------------------------------------------------
    while (window.isOpen())
    {
        // ——— Event handling ————————————————————————————————
        while (const auto event = window.pollEvent())  // optional<sf::Event>
        {
            if (event->is<sf::Event::Closed>())        // window close request
                window.close();
            // (add other event types here as needed)
        }
        //--——————————————————————————————— the maze
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



        // ——— Simulation update ———————————————————————————————
        float dt = clock.restart().asSeconds();        // frame time in seconds
        classical.update(dt);

  
        if (!quantum.collapsed)
            quantum.evolve(nodeList);

        // Collapse quantum particle when SPACE is pressed
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
            !quantum.collapsed)
        {
            quantum.collapse();
        }

        // ——— Rendering ———————————————————————————————————————
        window.clear(sf::Color::Black);
        drawMaze(window, nodeList, -1, -1); // -1 disables path highlighting
        classical.draw(window);
        quantum.draw(window);
        window.display();
    }

    return 0;
}
