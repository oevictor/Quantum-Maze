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
#include <filesystem>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <stack>
#include <time.h>
#include "../include/mazeHelper.hpp"              // grid constants, Node, drawMaze()
#include <SFML/Graphics.hpp>
#include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle
#include <SFML/Audio.hpp>  //audio
#include "../include/gamesettings.hpp" // Game settings header


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
   
        
    // window.setSize({640, 480});
    window.setSize({720, 480});
    window.setVerticalSyncEnabled(true); // enable VSync
    // Load a music to play
    

    sf::Music music;
    if (!music.openFromFile("music/Elmshore - Justin Bell.mp3")){
        std::cerr << "Failed to load music\n";
    }

    music.play(); //to ounvido cartola agr
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
    // void drawFinish(sf::RenderWindow& window, int col, int row);
    
    // make it random the finish line
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    FINISH_COL = std::rand() % GRID_WIDTH;
    FINISH_ROW = std::rand() % GRID_HEIGHT;
    // FINISH_COL = 5;
    // FINISH_ROW = 5;

    // initialize frontier walls
    std::vector<Wall> wallVec;
    addWalls(wallVec, nodeList, cur_col, cur_row);
    bool mazeReady = false;// just to check if the maze is ready

    //bolean to make a pase buttum
    bool pause = false;
    //restar function

    // ---------------------------------------------------------------------
    // Particle instantiation
    // ---------------------------------------------------------------------

    ClassicalParticle classical;

    // ClassicalParticle classical{
    //     sf::Vector2f(0.f, 0.f),   // initial position (top‑left corner)
    //     sf::Vector2f(4.f, 1.f),   //(x,y) velocity (5 px/s to the right)
    //     sf::Vector2f(0.f, 0.f)    // acceleration
    // };

    // ClassicalParticle classical2{
    //     sf::Vector2f(0.f, 0.f),   // initial position (top‑left corner)
    //     sf::Vector2f(4.f, 1.f),   //(x,y) velocity (5 px/s to the right)
    //     sf::Vector2f(0.f, 0.f)    // acceleration
    // };

    // ClassicalParticle classical3{
    //     sf::Vector2f(0.f, 0.f),   // initial position (top‑left corner)
    //     sf::Vector2f(4.f, 1.f),   //(x,y) velocity (5 px/s to the right)
    //     sf::Vector2f(0.f, 0.f)    // acceleration
    // };

    // ClassicalParticle classical4{
    //     sf::Vector2f(-5.f, -5.f),   // initial position (top‑left corner)
    //     sf::Vector2f(40.f, 10.f),   //(x,y) velocity (5 px/s to the right)
    //     sf::Vector2f(1.f, 0.f)    // acceleration

    // };



    // classical.color = sf::Color::Black; // default colour
    // classical2.color = sf::Color::Red; // default colour
    // classical3.color = sf::Color::Blue; // default colour
    // classical4.color = sf::Color::Yellow; // default colour

    // // building a bot vector
    std::vector<ClassicalParticle*> bots={};

    generateBots(bots, 40, nodeList);


    // // Place it at the randomly chosen start cell (cur_col, cur_row):
    // classical.setPosition(cur_col, cur_row, nodeList);

    QuantumParticle quantum;
    quantum.initialize();
    //seting the collapese to make it stops only when the space key is pressed
    bool autoCollapse = true;
    sf::Clock clock; // used to compute per‑frame Δt

    //PlayerParticle player;
    PlayerParticle player{
        sf::Vector2f(0.f, 0.f),   // initial position (top‑left corner)
        sf::Vector2f(4.f, 1.f),   //(x,y) velocity (5 px/s to the right)
        sf::Vector2f(0.f, 0.f)    // acceleration
    };
    player.color = sf::Color::Green; // default colour


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
            //events must be here

            if(auto key = event->getIf<sf::Event::KeyPressed>()){
                if(key->code == sf::Keyboard::Key::P){
                    pause = !pause;
                    if(pause){
                        std::cout << "Pause: ON\n";
                    }else{
                        std::cout << "Pause: OFF\n";
                    }
                }
            }
            if (auto key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::R) { // Reset game with 'R'
                    resetGame(nodeList, wallVec, player, bots, mazeReady, cur_col, cur_row);
                }
            }


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


            wallVec.erase(wallVec.begin() + idx);
        }
        else if (!mazeReady) {
            // Maze generation is complete
            mazeReady = true;
            // std::cout << "Maze generation complete.\n";
            classical.setPosition(cur_col, cur_row, nodeList);
        }



        //put the pause
        if(!pause){
 
            // ——— Simulation update ———————————————————————————————
            float dt = clock.restart().asSeconds();        // frame time in seconds


            if(mazeReady)
            {

                const float speed = 50.f;  // pixels per second

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

                // set the player velocity:
                player.velocity = dir * speed;

                // now integrate & collide:
                player.update(dt, nodeList);
                // change the logic to suport the new version 

                for (auto& bot : bots) {
                    bot->update(dt, nodeList);
                }
                
                //trying to set the postion so the particle is in the right place and computs
                player.col = static_cast<int>(player.position.x / NODE_SIZE);
                player.row = static_cast<int>(player.position.y / NODE_SIZE);
                
                player.setPosition(player.col, player.row, nodeList);


                if (player.col == FINISH_COL && player.row == FINISH_ROW) {

                    pause = true; // Pause the game
                    sf::Texture winTexture;
                    if (!winTexture.loadFromFile("imagen/skeleton dude.jpg")) { // Replace with your image path
                        std::cerr << "Failed to load win image\n";
                    } else {

                        sf::Sprite winSprite(winTexture);
                        while (pause && window.isOpen()) {
                            if (const auto event = window.pollEvent()) { // Use std::optional<sf::Event>
                                if (event->is<sf::Event::Closed>()) {    // Check if the event is a window close request
                                    window.close();
                                }
                                if (event->is<sf::Event::KeyPressed>()) {
                                    if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                                        // Reset the game when 'R' is pressed
                                        resetGame(nodeList, wallVec, player, bots, mazeReady, cur_col, cur_row);
                                        pause = false; // Resume the game
                                    }
                                }
                            }
                            window.clear(sf::Color::Black);
                        
                            // Display the  image
                            window.draw(winSprite);
                            window.display();
                        }
                    }
                }


                // Ajusting the logic to the new version of the bots
                for (auto& bot : bots) {
                    bot->col = static_cast<int>(bot->position.x / NODE_SIZE);
                    bot->row = static_cast<int>(bot->position.y / NODE_SIZE);
                    bot->setPosition(bot->col, bot->row, nodeList);
                }
                // Check if any bot has reached the finish line


                // after you update your bots:
                for (auto& bot : bots) {
                    if (bot->col == FINISH_COL && bot->row == FINISH_ROW) {
                        pause = true; // Pause the game
                        sf::Texture loseTexture;
                        if (!loseTexture.loadFromFile("imagen/trem.jpg")) { // Replace with your image path
                            std::cerr << "Failed to load lose image\n";
                        } else {
                            sf::Sprite loseSprite(loseTexture);
                            std::cout << "YOU LOSE!\n";
                            while (pause && window.isOpen()) {
                                if (const auto event = window.pollEvent()) { // Use std::optional<sf::Event>
                                    if (event->is<sf::Event::Closed>()) {    // Check if the event is a window close request
                                        window.close();
                                    }
                                    if (event->is<sf::Event::KeyPressed>()) {
                                        if (event->getIf<sf::Event::KeyPressed>()->code == sf::Keyboard::Key::R) {
                                            // Reset the game when 'R' is pressed
                                            resetGame(nodeList, wallVec, player, bots, mazeReady, cur_col, cur_row);
                                            pause = false; // Resume the game
                                        }
                                    }
                                }
                
                                window.clear(sf::Color::Black);
                
                                // Display the  image
                                window.draw(loseSprite);
                                window.display();
                            }
                        }
                    }
                }
            }


            if (autoCollapse)
            {
                // prepare the next frame
                if (quantum.collapsed)                     // was frozen last frame
                    quantum.collapsed = false;             // “un‑collapse” so it can walk
        
                quantum.evolve(nodeList);                 // quantum walk
                quantum.collapse();                       // immediate measurement
            }
        }
        // ——— Rendering ———————————————————————————————————————
        window.clear(sf::Color::Black);
        drawMaze(window, nodeList, -1, 1); // -1 disables path highlighting
        
        if(mazeReady){

            drawFinish(window, FINISH_COL, FINISH_ROW);
            
            
            player.draw(window);

            for (auto& bot : bots) {
                bot->draw(window);
            }



            // classical.draw(window);
            // classical2.draw(window);
            // classical3.draw(window);
            // classical4.draw(window);


            quantum.draw(window);
        }
        if(pause){
            //insert a imagem of pause on all the screen
            sf::Texture pauseTexture;
            if (!pauseTexture.loadFromFile("imagen/boca_boca.jpg")) {
                std::cerr << "Failed to load pause image\n"; 
            }else{
                sf::Sprite pauseSprite(pauseTexture);
                // pauseSprite.setPosition(0.f, 0.f); // Set position to top-left corner
                window.draw(pauseSprite);
            }
        }

        
        
        window.display();

    }
    return 0;
}
