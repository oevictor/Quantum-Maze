// =============================================================================
// particle.cpp — Implementation of ClassicalParticle & QuantumParticle
// Part of the “Labyrinth: Classical vs Quantum” demo
//
// This implementation file contains the runtime behaviour for two agents that
// live inside a grid‑based maze rendered with SFML 3.
// ──────────────────────────────────────────────────────────────────────────────
//  • ClassicalParticle — deterministic Newtonian motion (green disc)
//  • QuantumParticle   — simple discrete quantum walk with collapse (Blue)
//
// The accompanying declarations and grid helpers live in:
//   • particle.hpp   — class/struct definitions
//   • mazeHelper.h   — grid constants (GRID_WIDTH, GRID_HEIGHT, NODE_SIZE),
//                      Node struct, and helper functions nextCol/nextRow/
//                      indexIsValid.
//
// © <2025> <Victor Emanuel> — MIT License
// =============================================================================

#include "../include/particle.hpp"
#include "../include/mazeHelper.h"       // GRID_* constants & Node helpers
#include <SFML/Graphics.hpp>
#include <algorithm>           // std::copy (used in QuantumParticle::evolve)
#include <iostream>
// ─────────────────────────────────────────────────────────────────────────────
// ClassicalParticle — methods
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Integrates velocity and position using explicit Euler.
 *
 * @param dt   Elapsed time in **seconds** since last frame.
 */
void ClassicalParticle::update(float dt)
{
    velocity += acceleration * dt;
    position += velocity * dt;
}

/**
 * @brief Renders the classic particle as a solid green circle.
 *
 * The radius is `0.3 × NODE_SIZE`, chosen to fit nicely inside a maze cell.
 * @param window  Destination SFML render target.
 */
void ClassicalParticle::draw(sf::RenderWindow& window) const
{
    sf::CircleShape shape(NODE_SIZE * 0.3f);
    shape.setFillColor(color);
    shape.setPosition(position);   // position already holds an sf::Vector2f
    window.draw(shape);
}

// ─────────────────────────────────────────────────────────────────────────────
// QuantumParticle — methods
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Resets the probability array to a uniform distribution.
 */
void QuantumParticle::initialize()
{
    float uniform = 1.0f / (GRID_WIDTH * GRID_HEIGHT);
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
        probability[i] = uniform;
}

/**
 * @brief Performs one step of the discrete quantum walk.
 *
 * Probability mass in each cell flows equally to all neighbouring cells that
 * are reachable (i.e., the corresponding wall is open).
 *
 * @param nodeList Flat array of Node elements representing the maze grid.
 */
void QuantumParticle::evolve(Node nodeList[])
{
    float next[GRID_WIDTH * GRID_HEIGHT] = {0.0f};

    for (int r = 0; r < GRID_HEIGHT; ++r)
    {
        for (int c = 0; c < GRID_WIDTH; ++c)
        {
            int idx = c + r * GRID_WIDTH;
            float p = probability[idx];
            if (p == 0) continue;        // skip zero‑probability cells

            Node &n = nodeList[idx];

            // count open exits (walls == false)
            int count = 0;
            for (int s = 0; s < 4; ++s)
                if (!n.walls[s]) ++count;

            // distribute probability equally among open exits
            for (int s = 0; s < 4; ++s)
            {
                if (!n.walls[s])
                {
                    int nc = nextCol(c, s);
                    int nr = nextRow(r, s);
                    if (indexIsValid(nc, nr))
                    {
                        int ni = nc + nr * GRID_WIDTH;
                        next[ni] += p / count;
                    }
                }
            }
        }
    }

    // copy back into member array
    std::copy(std::begin(next), std::end(next), std::begin(probability));
}

/**
 * @brief Simulates a measurement, collapsing the wavefunction.
 *
 * Draws a uniformly random number r ∈ [0,1) and finds the first index where the
 * cumulative probability exceeds r. The particle then acquires definite cell
 * coordinates `(col,row)`.
 */
void QuantumParticle::collapse()
{
    float r = static_cast<float>(rand()) / RAND_MAX;
    float sum = 0.0f;
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
    {
        sum += probability[i];
        if (r < sum)
        {
            col = i % GRID_WIDTH;
            row = i / GRID_WIDTH;
            collapsed = true;
            break;
        }
    }
}

/**
 * @brief Draws either the probability field or the collapsed point particle.
 *
 *  • **Not collapsed** → Draw semi‑transparent magenta blobs in every cell with
 *    probability > 0.01. Radius is proportional to probability.
 *  • **Collapsed**     → Draw a solid magenta circle at the selected cell.
 *
 * @param window SFML render target.
 */
void QuantumParticle::draw(sf::RenderWindow& window) const
{
    if (!collapsed)
    {
        for (int r = 0; r < GRID_HEIGHT; ++r)
        {
            for (int c = 0; c < GRID_WIDTH; ++c)
            {
                float p = probability[c + r * GRID_WIDTH];
                if (p > 0.01f)
                {
                    sf::CircleShape blob(NODE_SIZE * p * 1.5f);
                    blob.setFillColor(sf::Color(255, 0, 255, static_cast<int>(255 * p)));
                    blob.setPosition(sf::Vector2f(c * NODE_SIZE, r * NODE_SIZE));
                    window.draw(blob);
                }
            }
        }
    }
    else // collapsed state
    {   
        // std::cout << "Collapsed at: (" << col << ", " << row << ")\n";
        sf::CircleShape qblob(NODE_SIZE * 0.3f);
        qblob.setFillColor(color);
        qblob.setPosition(sf::Vector2f(col * NODE_SIZE + NODE_SIZE * 0.2f,
                                       row * NODE_SIZE + NODE_SIZE * 0.2f));
        window.draw(qblob);
    }
}
