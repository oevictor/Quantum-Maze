#include "../include/mazeHelper.hpp"
// #include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle
#include "../include/gamesettings.hpp" // Game settings header
#include <SFML/Graphics.hpp> // For graphics rendering


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
        bot->velocity = sf::Vector2f(0.f, 0.f);
        bot->col = 0;
        bot->row = 0;
        }

        // Reset finish line
        FINISH_COL = std::rand() % GRID_WIDTH;
        FINISH_ROW = std::rand() % GRID_HEIGHT;

        std::cout << "Game reset!\n";
}
