#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H
#include <iostream>
#include "../include/mazeHelper.hpp"            // grid constants, Node, drawMaze()
#include "../include/particle.hpp"   // ClassicalParticle, QuantumParticle
//to puting some event that may be necessary to the game

void resetGame(Node* nodeList, std::vector<Wall>& wallVec, PlayerParticle& player,
    std::vector<ClassicalParticle*>& bots, bool& mazeReady, int& cur_col, int& cur_row);

#endif