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
#include "../include/mazeHelper.hpp"   // grid constants and helpers
#include <algorithm>                  // std::copy (used by QuantumParticle)

//palyer particle it just a copy of classical particle but with a different color and name

struct PlayerParticle{
    sf::Vector2f position;     //!< Center position in pixels.
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Color    color = sf::Color::Green;
    int col=0, row=0;

    // Radius used for drawing and collision (20% of cell size)
    inline float radius() const { return NODE_SIZE * 0.2f; }

    void setPosition(int newCol, int newRow, Node nodeList[]);
    void update(float dt, Node nodeList[]);
    void draw(sf::RenderWindow& window) const;
};





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
struct ClassicalParticle{
    sf::Vector2f position;     //!< Center position in pixels.
    sf::Vector2f velocity;
    sf::Vector2f acceleration;
    sf::Color    color = sf::Color::Green;
    int col=0, row=0;

    // Radius used for drawing and collision (20% of cell size)
    inline float radius() const { return NODE_SIZE * 0.2f; }

    void setPosition(int newCol, int newRow, Node nodeList[]);
    void update(float dt, Node nodeList[]);
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
struct QuantumParticle{

    float       probability[GRID_WIDTH * GRID_HEIGHT] = {0.f}; //!< Probability map.
    sf::Color   color      = sf::Color::Blue;                 //!< Rendering colour.
    bool        collapsed  = false;                           //!< True after collapse().
    int         col = 0, row = 0;                             //!< Cell coordinates once collapsed.

    /** @brief Initialise with a uniform distribution. */
    void initialize(Node* nodeList);

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
    static void addQuantumParticle(std::vector<QuantumParticle*>& out,
                                    int numParticles,
                                    Node*);

    // void addQuantumParticle(std::vector<QuantumParticle*>& particles, int numParticles, Node* nodeList) {}

};
