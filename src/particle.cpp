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
#include "../include/mazeHelper.hpp"       // GRID_* constants & Node helpers
#include <SFML/Graphics.hpp>
#include <algorithm>           // std::copy (used in QuantumParticle::evolve)
#include <iostream>


// ─────────────────────────────────────────────────────────────────────────────
// PlayerParticle — methods aging it just a copy of classical particle 
// ─────────────────────────────────────────────────────────────────────────────

void PlayerParticle::draw(sf::RenderWindow& window) const
{
    sf::CircleShape shape(NODE_SIZE * 0.3f);
    shape.setFillColor(color);
    shape.setPosition(position);   // position already holds an sf::Vector2f
    window.draw(shape);
}

/*Function to determinates if is possible to the classical is in the ritgh place*/


void PlayerParticle::update(float dt, Node nodeList[]){

    
    // 1) integrate acceleration → velocity
    velocity += acceleration * dt;
    

    // 2) propose new continuous position
    sf::Vector2f nextPos = position + velocity * dt;

    // 3) figure out which cell we’re in, before and after
    int oldCol = static_cast<int>(position.x / NODE_SIZE);
    int oldRow = static_cast<int>(position.y / NODE_SIZE);
    int newCol = static_cast<int>(nextPos.x  / NODE_SIZE);
    int newRow = static_cast<int>(nextPos.y  / NODE_SIZE);

    // 4) handle X­axis crossing
    if (newCol != oldCol) {
        // which wall would we cross?
        int side = (newCol > oldCol) ? SIDE_RIGHT : SIDE_LEFT;
        // if there’s a wall there, bounce and cancel the X move
        if (nodeList[oldCol + oldRow*GRID_WIDTH].walls[side]) {
            velocity.x = -velocity.x;
            nextPos.x = position.x;
        }
    }

    // 5) handle Y­axis crossing
    if (newRow != oldRow) {
        int side = (newRow > oldRow) ? SIDE_DOWN : SIDE_TOP;
        if (nodeList[oldCol + oldRow*GRID_WIDTH].walls[side]) {
            velocity.y = -velocity.y;
            nextPos.y = position.y;
        }
    }

    // 6) commit the (possibly corrected) position
    position = nextPos;
}

/**
 * @brief Sets the particle's position to a new cell, checking for walls.
 *
 * @param newCol  New column index in the grid.
 * @param newRow  New row index in the grid.
 * @param nodeList  Flat array of Node objects representing the maze layout.
 */
void PlayerParticle::setPosition(int newCol,
    int newRow,
    Node nodeList[])
{
    // 1) Bounds check
    if (!indexIsValid(newCol, newRow)) {
    std::cout << "Invalid cell (" 
    << newCol << "," << newRow << ")\n";
    return;
    }

    // 2) Which side?
    int oldIdx = col + row * GRID_WIDTH;
    int side = -1;
    if      (newCol == col + 1 && newRow == row)      side = SIDE_RIGHT;
    else if (newCol == col - 1 && newRow == row)      side = SIDE_LEFT;
    else if (newRow == row + 1 && newCol == col)      side = SIDE_DOWN;
    else if (newRow == row - 1 && newCol == col)      side = SIDE_TOP;

    // 3) Blocked by wall?  Bounce
    if (side >= 0 && nodeList[oldIdx].walls[side]) {
    if (side == SIDE_LEFT || side == SIDE_RIGHT)
    velocity.x = -velocity.x;
    else
    velocity.y = -velocity.y;
    return;
    }

    // 4) Not a neighbor?  Ignore
    if (side < 0) {
    return;
    }

    // 5) Commit the move
    col = newCol;
    row = newRow;
    position.x = col * NODE_SIZE;
    position.y = row * NODE_SIZE;
}


// ─────────────────────────────────────────────────────────────────────────────
// ClassicalParticle — methods
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Integrates velocity and position 
 *
 * @param dt   Elapsed time in **seconds** since last frame.
 */
// void ClassicalParticle::update(float dt, Node nodeList[])
// {
//     velocity += acceleration * dt;
//     position += velocity * dt;
// }

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


/*Function to determinates if is possible to the classical is in the ritgh place*/

void ClassicalParticle::update(float dt, Node nodeList[])
{
    // 1) integrate acceleration → velocity
    velocity += acceleration * dt;
    

    // 2) propose new continuous position
    sf::Vector2f nextPos = position + velocity * dt;

    // 3) figure out which cell we’re in, before and after
    int oldCol = static_cast<int>(position.x / NODE_SIZE);
    int oldRow = static_cast<int>(position.y / NODE_SIZE);
    int newCol = static_cast<int>(nextPos.x  / NODE_SIZE);
    int newRow = static_cast<int>(nextPos.y  / NODE_SIZE);

    // 4) handle X­axis crossing
    if (newCol != oldCol) {
        // which wall would we cross?
        int side = (newCol > oldCol) ? SIDE_RIGHT : SIDE_LEFT;
        // if there’s a wall there, bounce and cancel the X move
        if (nodeList[oldCol + oldRow*GRID_WIDTH].walls[side]) {
            velocity.x = -velocity.x;
            nextPos.x = position.x;
        }
    }

    // 5) handle Y­axis crossing
    if (newRow != oldRow) {
        int side = (newRow > oldRow) ? SIDE_DOWN : SIDE_TOP;
        if (nodeList[oldCol + oldRow*GRID_WIDTH].walls[side]) {
            velocity.y = -velocity.y;
            nextPos.y = position.y;
        }
    }

    // 6) commit the (possibly corrected) position
    position = nextPos;
}
void ClassicalParticle::setPosition(int newCol,
    int newRow,
    Node nodeList[])
{
    // 1) Bounds check
    if (!indexIsValid(newCol, newRow)) {
    std::cout << "Invalid cell (" 
    << newCol << "," << newRow << ")\n";
    return;
    }

    // 2) Which side?
    int oldIdx = col + row * GRID_WIDTH;
    int side = -1;
    if      (newCol == col + 1 && newRow == row)      side = SIDE_RIGHT;
    else if (newCol == col - 1 && newRow == row)      side = SIDE_LEFT;
    else if (newRow == row + 1 && newCol == col)      side = SIDE_DOWN;
    else if (newRow == row - 1 && newCol == col)      side = SIDE_TOP;

    // 3) Blocked by wall?  Bounce
    if (side >= 0 && nodeList[oldIdx].walls[side]) {
    if (side == SIDE_LEFT || side == SIDE_RIGHT)
    velocity.x = -velocity.x;
    else
    velocity.y = -velocity.y;
    return;
    }

    // 4) Not a neighbor?  Ignore
    if (side < 0) {
    return;
    }

    // 5) Commit the move
    col = newCol;
    row = newRow;
    position.x = col * NODE_SIZE;
    position.y = row * NODE_SIZE;
}


// ─────────────────────────────────────────────────────────────────────────────
// QuantumParticle — methods
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Resets the probability array to a uniform distribution.
 */

//define the number of particles in the maze
// std::vector<QuantumParticle*> particles; //vector to store the particles


void QuantumParticle::addQuantumParticle(std::vector<QuantumParticle*>& out, int numParticles, Node* nodeList) {
    for (int i = 0; i < numParticles; ++i) {
        // QuantumParticle* particle = new QuantumParticle; //creating the bot 
        auto*p =new QuantumParticle; //creating the bot
        // particle->position = sf::Vector2f(0.f, 0.f); // Initial position
        // particle->col = std::rand() % GRID_WIDTH; // Random column
        // particle->row = std::rand() % GRID_HEIGHT; // Random row
        // particle->color = sf::Color(std::rand() , std::rand() , std::rand()); // Default color
        // particle->initialize(nodeList); // Initialize the probability array
        // particles.push_back(particle);

        p->col = std::rand() % GRID_WIDTH; // Random column
        p->row = std::rand() % GRID_HEIGHT; // Random row
        p->color = sf::Color(std::rand() , std::rand() , std::rand()); // Default color
        p->initialize(nodeList); // Initialize the probability array
        out.push_back(p); // Add the particle to the vector


    }
    std::cout << "Quantum particles generated "<<numParticles << "!\n";
}
void QuantumParticle::initialize(Node* nodeList) //the probability array is initialized to a uniform distribution
{
    // addQuantumParticle(particles, 100, nodeList);
    // std::cout << "Quantum particles generated "<<numParticles << "!\n";
    float uniform = 1.0f / (GRID_WIDTH * GRID_HEIGHT);
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i){
        probability[i] = uniform;
    }
        // std::cout << "Quantum particle initialized with uniform distribution.\n";
}


/**
 * @brief Advances the quantum particle's wavefunction using a discrete quantum walk.
 *
 * This method simulates one step of evolution for the particle's probability distribution
 * in a maze represented by a grid of `Node` objects. Each cell with non-zero probability
 * distributes its probability mass equally to all reachable neighboring cells (i.e., those
 * without a wall in between).
 *
 * The result is a new probability field that replaces the current one.
 *
 * @param nodeList A flat array of `Node` objects representing the maze grid.
 *                 Each node contains wall information that determines valid paths
 *                 for the quantum walk.
 */

void QuantumParticle::evolve(Node nodeList[]) 
/*the probability mass in each cell flows equally to all
neighbouring cells that are reachable (i.e., the corresponding wall is open)*/
{
    float next[GRID_WIDTH * GRID_HEIGHT] = {0.0f}; // next probability field

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
    // Step 1: Generate a random number in the range [0, 1)
    float r = static_cast<float>(rand()) / RAND_MAX;

    // Step 2: Iterate through the probability field, accumulating probability
    float sum = 0.0f;
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; ++i)
    {
        sum += probability[i];

        // Step 3: Collapse the wavefunction at the first index where the cumulative
        // probability exceeds the random number r
        if (r < sum)
        {
            // Compute the corresponding cell coordinates (col, row)
            col = i % GRID_WIDTH;
            row = i / GRID_WIDTH;

            // Mark the particle as collapsed
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
    // Case 1: Particle has not yet collapsed
    if (!collapsed)
    {
        // Iterate over every cell in the grid
        for (int r = 0; r < GRID_HEIGHT; ++r)
        {
            for (int c = 0; c < GRID_WIDTH; ++c)
            {
                // Get the probability at this cell
                float p = probability[c + r * GRID_WIDTH];

                // Only draw if probability is noticeable
                if (p > 0.01f)
                {
                    // Create a translucent magenta circle, size proportional to p
                    sf::CircleShape blob(NODE_SIZE * p * 1.5f);
                    // blob.setFillColor(sf::Color(255, 0, 255, static_cast<int>(255 * p)));
                    blob.setFillColor(sf::Color::Blue);
                    // Position it centered in the cell
                    blob.setPosition(sf::Vector2f(c * NODE_SIZE, r * NODE_SIZE));
                    
                    // Render the blob to the window
                    window.draw(blob);
                }
            }
        }
    }
    else // Case 2: Particle has collapsed to a definite position
    {
        // Create a solid magenta circle representing the particle
        sf::CircleShape qblob(NODE_SIZE * 0.3f);
        qblob.setFillColor(color);

        // Offset position slightly to center it in the cell
        qblob.setPosition(sf::Vector2f(col * NODE_SIZE + NODE_SIZE * 0.2f,
                                       row * NODE_SIZE + NODE_SIZE * 0.2f));
        
        // Render the particle to the window
        window.draw(qblob);
    }
}
