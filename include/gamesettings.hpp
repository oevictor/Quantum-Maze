#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H
//to puting some event that may be necessary to the game
#include <iostream>
#include "../include/mazeHelper.hpp"            // grid constants, Node, drawMaze()
#include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle
void resetGame(Node* nodeList, std::vector<Wall>& wallVec, PlayerParticle& player,
    std::vector<ClassicalParticle*>& bots, bool& mazeReady, int& cur_col, int& cur_row);

#endif