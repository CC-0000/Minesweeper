//
// Created by Lolz on 12/6/2021.
//

#ifndef MINESWEEPER_TILE_H
#define MINESWEEPER_TILE_H
#include <SFML/Graphics.hpp>

using namespace sf;
class Tile
{
public:
    enum State { REVEALED, HIDDEN, FLAGGED, EXPLODED };// Represents tile’s current UI state (visualization).

private:
    bool isAMine = false;// stores whether or not the tile is a mine
    int number = 0;// stores the number that the tile represents. 0 for blank
    Tile::State state;// stores the current UI state of the tile
    sf::Vector2f position;// stores the position of this tile on the board
    std::array<Tile*, 8> neighbors;// stores pointers pointing to the 8 adjacent tiles to this tile

public:
    Tile(sf::Vector2f position);// Constructs a new tile object at the designated _position.
    sf::Vector2f getLocation();// Returns the position of this tile.
    State getState();// Returns current state of this tile.
    void setState(State _state);// Sets the state of this tile. Should trigger other behaviors related to the state change (including visualization).
    void onClickLeft();// Defines the reveal behavior for a tile when the left mouse button is clicked inside its boundaries.
    void onClickRight();// Toggles this tile’s state between FLAGGED and HIDDEN.
    void draw();// draws the tile onto the window

    // setters and getters
    void setNeighbors(std::array<Tile*, 8> _neighbors);
    std::array<Tile*, 8>& getNeighbors();
    void setIsAMine(bool isAMine);
    bool getIsAMine();
    void setNumber(int number);
    int getNumber();

protected:
    void revealNeighbors();// recursive method that performs a cascade reveal on blank tiles
};


#endif //MINESWEEPER_TILE_H
