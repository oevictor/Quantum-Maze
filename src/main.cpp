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
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <time.h>
#include "mazeHelper.h"              // grid constants, Node, drawMaze()
#include <SFML/Graphics.hpp>
#include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle
#include <SFML/Audio.hpp>  //audio

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
    // Load a music to play
    sf::Music music;      
    if (!music.openFromFile("music/Elmshore - Justin Bell.mp3")){
        std::cerr << "Failed to load music\n";
    }

    music.play();
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
    bool mazeReady = false;// just to check if the maze is ready

    // ---------------------------------------------------------------------
    // Particle instantiation
    // ---------------------------------------------------------------------
    ClassicalParticle classical{
        sf::Vector2f(0.f, 0.f),   // initial position (top‑left corner)
        sf::Vector2f(4.f, 1.f),   //(x,y) velocity (5 px/s to the right)
        sf::Vector2f(0.f, 0.f)    // acceleration
    };

    // // Place it at the randomly chosen start cell (cur_col, cur_row):
    // classical.setPosition(cur_col, cur_row, nodeList);

    QuantumParticle quantum;
    quantum.initialize();
    //seting the collapese to make it stops only when the space key is pressed
    bool autoCollapse = true;
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

          // toggle behaviour when SPACE *goes down* (no key repeat)
          if (auto key = event->getIf<sf::Event::KeyPressed>())
          {
              if (key->code == sf::Keyboard::Key::Space)
              {
                  autoCollapse = !autoCollapse;
                //   std::cout << "Auto‑collapse: "
                //             << (autoCollapse ? "ON" : "OFF") << '\n';
              }
          }

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

            // // Suppose newCol/newRow is your target cell:
            // classical.setPosition(cur_col, cur_row, nodeList);


            wallVec.erase(wallVec.begin() + idx);
        }
        else if (!mazeReady) {
            // Maze generation is complete
            mazeReady = true;
            std::cout << "Maze generation complete.\n";
            classical.setPosition(cur_col, cur_row, nodeList);
        }


        // Place it at the randomly chosen start cell (cur_col, cur_row):
        // classical.setPosition(cur_col, cur_row, nodeList);
        // Suppose newCol/newRow is your target cell:
        // classical.setPosition(cur_col, cur_row, nodeList);

        // ——— Simulation update ———————————————————————————————
        float dt = clock.restart().asSeconds();        // frame time in seconds
        // classical.update(dt);
        if(mazeReady)
        {
            // at the top of your loop, before classical.update(dt,nodeList):
            const float speed = 100.f;  // pixels per second

            sf::Vector2f dir{0.f, 0.f};
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) dir.y -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) dir.y += 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) dir.x -= 1.f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) dir.x += 1.f;

            // normalize so diagonal isn’t faster:
            if (dir.x != 0.f || dir.y != 0.f) {
                float len = std::sqrt(dir.x*dir.x + dir.y*dir.y);
                dir /= len;
            }
            classical.velocity = dir * speed;

            // now integrate & collide:
            classical.update(dt, nodeList);

            // classical.update(dt);
            // classical.update(dt, nodeList); // update the position of the particle
            // classical.setPosition(cur_col, cur_row, nodeList);
        }

        // if (!quantum.collapsed)
        //     quantum.evolve(nodeList);

        // // Collapse quantum particle when SPACE is pressed
        // if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) &&
        //     !quantum.collapsed)
        // {   
        //     quantum.evolve(nodeList);
        //     quantum.collapse();
        // }
        //trying to make it stops colaps only when the space key is pressed
        if (autoCollapse)
        {
            // prepare the next frame
            if (quantum.collapsed)                     // was frozen last frame
                quantum.collapsed = false;             // “un‑collapse” so it can walk
    
            quantum.evolve(nodeList);                 // quantum walk
            quantum.collapse();                       // immediate measurement
        }

        // ——— Rendering ———————————————————————————————————————
        window.clear(sf::Color::Black);
        drawMaze(window, nodeList, -1, -1); // -1 disables path highlighting
        
        if(mazeReady){
            classical.draw(window);
            quantum.draw(window);
        }
        
        
        
        // classical.draw(window);
        // quantum.draw(window);
        
        
        window.display();
        // music.setLoop(true);  
        // music.play();
    

    }
    return 0;
}
