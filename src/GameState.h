#ifndef MINESWEEPER_GAMESTATE_H
#define MINESWEEPER_GAMESTATE_H
#include <SFML/Graphics.hpp>
#include "Tile.h"

using namespace sf;
class GameState
{
    std::vector<std::vector<Tile*>> tile2DArray;// stores all of the tile objects

public:
    enum PlayStatus { WIN, LOSS, PLAYING };// Represents the 3 possible play statuses

private:
    PlayStatus playStatus;// stores the current play status

public:
    GameState(sf::Vector2i _dimensions = Vector2i(25, 16), int _numberOfMines = 50);// Constructs a new random game state with specified tile dimensions and number of mines.
    ~GameState();// destructor
    GameState(const char* filepath);// Constructs a new game state based on the size, mine placement, and mine number specified at the filepath.
    int getFlagCount();// returns the current count of the number of flags placed on the screen.
    int getMineCount();// returns the current count of the number of mines actually on the board.
    Tile* getTile(int x, int y);// Returns a pointer to the Tile at the specified coordinates, or nullptr if out of bounds.
    PlayStatus getPlayStatus();// Returns the play status of the game.
    void setPlayStatus(PlayStatus _status);// Sets the play status of the game
};


#endif //MINESWEEPER_GAMESTATE_H
