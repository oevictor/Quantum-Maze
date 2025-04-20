#include "../include/mazeHelper.hpp"
#include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle
#include "../include/gamesettings.hpp" // Game settings header
#include <SFML/Graphics.hpp> // For graphics rendering






//another function to imporve the bots the way they are generated
//genereted the bots in a random way and with a defined number of bots
void generateBots(std::vector<ClassicalParticle*>& bots, int numBots, Node* nodeList) {
    for (int i = 0; i < numBots; ++i) {
        ClassicalParticle* bot = new ClassicalParticle; //creating the bot 
        // bot->position = sf::Vector2f(0.f, 0.f); // Initial position
        bot->position= sf::Vector2f(std::rand() % GRID_HEIGHT,  std::rand() % GRID_WIDTH); // Initial position
        // bot->velocity = sf::Vector2f(10.f, 5.f); // Initial velocity
        bot->velocity = sf::Vector2f(std::rand() % 10 , std::rand() % 10); // Initial velocity
        bot->acceleration = sf::Vector2f(std::rand() % 100, std::rand() % 100); // Initial acceleration
        bot->col = std::rand() % GRID_WIDTH; // Random column
        bot->row = std::rand() % GRID_HEIGHT; // Random row
        bot->color = sf::Color(std::rand() , std::rand() , std::rand()); // Default color
        bot->setPosition(bot->col, bot->row, nodeList); // Set position in the maze
        bots.push_back(bot);
        
    }
    std::cout << "Bots generated "<<numBots << "!\n";
}





//just a function to reset the gaame 
void resetGame(Node* nodeList, std::vector<Wall>& wallVec, PlayerParticle& player,
                std::vector<ClassicalParticle*>& bots, bool& mazeReady, int& cur_col, int& cur_row) {
        // Reset maze
        std::fill(nodeList, nodeList + (GRID_WIDTH * GRID_HEIGHT), Node{}); // Clear all nodes
        wallVec.clear();          // Clear walls
        cur_col = std::rand() % GRID_WIDTH; // Random starting cell
        cur_row = std::rand() % GRID_HEIGHT;
        nodeList[cur_col + cur_row * GRID_WIDTH].visited = true;
        addWalls(wallVec, nodeList, cur_col, cur_row);
        mazeReady = false;

        // Reset player
        player.position = sf::Vector2f(0.f, 0.f);
        player.velocity = sf::Vector2f(0.f, 0.f);
        player.col = 0;
        player.row = 0;

        // Reset bots
        for (auto& bot : bots) {
        bot->position = sf::Vector2f(0.f, 0.f);
        bot->velocity = sf::Vector2f(10.f, 5.f);
        bot->col = 0;
        bot->row = 0;
        }

        // Reset finish line
        FINISH_COL = std::rand() % GRID_WIDTH;
        FINISH_ROW = std::rand() % GRID_HEIGHT;

        std::cout << "Game reset!\n";
}
