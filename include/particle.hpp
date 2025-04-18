#pragma once
// ============================================================================
// particle.hpp — Declarations for ClassicalParticle and QuantumParticle
// Part of the “Labyrinth: Classical vs Quantum” project
//
// • ClassicalParticle  — deterministic Newtonian motion
// • QuantumParticle    — discrete quantum‑walk probability field
//
// Dependencies:
//   * SFML 3 (Graphics module)
//   * mazeHelper.h  → Node definition, grid constants (GRID_WIDTH/HEIGHT,
//                     NODE_SIZE) and helper functions:
//                     nextCol(), nextRow(), indexIsValid()
//
// All comments use Doxygen style so they can be turned into HTML/PDF docs with
// a single `doxygen` run.
// ============================================================================

#include <SFML/Graphics.hpp>
#include "../include/mazeHelper.h"   // grid constants and helpers
#include <algorithm>                  // std::copy (used by QuantumParticle)

// ─────────────────────────────────────────────────────────────────────────────
// ClassicalParticle
// ─────────────────────────────────────────────────────────────────────────────
/**
 * @class ClassicalParticle
 * @brief A point‑mass that follows Newtonian kinematics.
 *
 * The state advance is a simple explicit‑Euler integration:
 *   - **velocity** ← **velocity** + **acceleration** · dt
 *   - **position** ← **position** + **velocity** · dt
 *
 * Rendering: a solid green disc with radius 0.3 × NODE_SIZE centred at
 * `position`.
 */
struct ClassicalParticle
{
    sf::Vector2f position;     //!< Current position in pixels.
    sf::Vector2f velocity;     //!< Velocity in pixels · s⁻¹.
    sf::Vector2f acceleration; //!< Acceleration in pixels · s⁻².
    sf::Color    color = sf::Color::Green; //!< Fill colour (default: green).


    //Putting somethig so it now where it can be or not 
    int col=0, row=0; //position of the particle in the grid

    /*void to determinaed if the particle is in the right position*/
    void setPosition(int newCol, int newRow,Node nodeList[]); //Cecking to see where the particle is

    /**
     * @brief Integrate one time‑step.
     * @param dt  Elapsed time in **seconds**.
     */
    // void update(float dt);
    void update (float dt, Node nodeList[]); //update the position of the particle

    /**
     * @brief Draw the particle on the provided SFML render target.
     * @param window  Render target.
     */
    void draw(sf::RenderWindow& window) const;
};

// ─────────────────────────────────────────────────────────────────────────────
// QuantumParticle
// ─────────────────────────────────────────────────────────────────────────────
/**
 * @class QuantumParticle
 * @brief Discrete quantum‑walk entity represented by a probability field.
 *
 * Internally stores |ψ|² for every cell in a flat array of size
 * `GRID_WIDTH * GRID_HEIGHT`.
 */
struct QuantumParticle
{
    float       probability[GRID_WIDTH * GRID_HEIGHT] = {0.f}; //!< Probability map.
    sf::Color   color      = sf::Color::Blue;                 //!< Rendering colour.
    bool        collapsed  = false;                           //!< True after collapse().
    int         col = 0, row = 0;                             //!< Cell coordinates once collapsed.

    /** @brief Initialise with a uniform distribution. */
    void initialize();

    /**
     * @brief Perform one evolution step of the quantum walk.
     *
     * Probability at each open cell is evenly distributed to its neighbours
     * according to the maze topology stored in @p nodeList.
     *
     * @param nodeList  Flat array of Node objects describing the maze layout.
     */
    void evolve(Node nodeList[]);

    /**
     * @brief Collapse the wavefunction, sampling a single cell position.
     *
     * Uses a random float in [0,1) to pick the first cell where the cumulative
     * probability exceeds that value.
     */
    void collapse();

    /**
     * @brief Render the probability blobs or the collapsed particle.
     * @param window  SFML render target.
     */
    void draw(sf::RenderWindow& window) const;
};
