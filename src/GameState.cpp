#include <iostream>
#include <fstream>
#include <sstream>
#include "GameState.h"

GameState::GameState(sf::Vector2i _dimensions, int _numberOfMines)// Constructs a new random game state with specified tile dimensions and number of mines.
{
    this->playStatus = GameState::PLAYING;
    int totalNumberOfTiles = _dimensions.x * _dimensions.y;
    std::array<Tile*, 8> neighbors;// a Tile array containing tile pointers to a given tile's neighbors
    std::array<bool, 8> neighborsMap;// a temporary boolean array that stores whether or not there is a neighbor at the specified index

    // fill the tile array with new tiles according to the given dimensions
    tile2DArray.resize(_dimensions.x);
    for(int i = 0; i < _dimensions.x; i++)
    {
        tile2DArray[i] = std::vector<Tile*>(_dimensions.y);
        for(int j = 0; j < _dimensions.y; j++)
        {
            tile2DArray[i][j] = new Tile(sf::Vector2f(i, j));
        }
    }

    // set the neighbors for all of the tiles

    // go through the entire 2D tile array
    for(int i = 0; i < _dimensions.x; i++)
    {
        for(int j = 0; j < _dimensions.y; j++)
        {
            // assume that the tile doesn't have any valid neighbors
            for(int i = 0; i < 8; i++)
            {
                neighborsMap[i] = true;
                neighbors[i] = nullptr;
            }

            // check to see where the tile is
            if(i - 1 < 0)// this means the tile is on the left edge of the board
            {
                neighborsMap[0] = false;
                neighborsMap[3] = false;
                neighborsMap[5] = false;
            }
            else if(i + 1 >= _dimensions.x)// this means the tile is on the right edge of the board
            {
                neighborsMap[2] = false;
                neighborsMap[4] = false;
                neighborsMap[7] = false;
            }

            if(j - 1 < 0)// this means the tile is on the top edge of the board
            {
                neighborsMap[0] = false;
                neighborsMap[1] = false;
                neighborsMap[2] = false;
            }
            else if(j + 1 >= _dimensions.y)// this means the tile is on the bottom edge of the board
            {
                neighborsMap[5] = false;
                neighborsMap[6] = false;
                neighborsMap[7] = false;
            }

            // generate neighbor pointers according to the specified values in the boolean neighbors map
            if(neighborsMap[0] != false)
            {
                neighbors[0] = tile2DArray[i - 1][j - 1];
            }
            if(neighborsMap[1] != false)
            {
                neighbors[1] = tile2DArray[i][j - 1];
            }
            if(neighborsMap[2] != false)
            {
                neighbors[2] = tile2DArray[i + 1][j - 1];
            }
            if(neighborsMap[3] != false)
            {
                neighbors[3] = tile2DArray[i - 1][j];
            }
            if(neighborsMap[4] != false)
            {
                neighbors[4] = tile2DArray[i + 1][j];
            }
            if(neighborsMap[5] != false)
            {
                neighbors[5] = tile2DArray[i - 1][j + 1];
            }
            if(neighborsMap[6] != false)
            {
                neighbors[6] = tile2DArray[i][j + 1];
            }
            if(neighborsMap[7] != false)
            {
                neighbors[7] = tile2DArray[i + 1][j + 1];
            }

            // store the neighbors array into the tile
            tile2DArray[i][j]->setNeighbors(neighbors);
        }
    }

    // set the mines in random locations

    // seed the random number generator
    srand(time(NULL));
    int x, y, i = 0, oldNumber;

    while(i < _numberOfMines)// keep on generating mines until the generated number of mines equals the specified amount
    {
        x = rand() % totalNumberOfTiles + 1;// generates a random number from 1 to the number of tiles
        y = 0;
        // convert the random number to a 2D array coordinate
        while(x - _dimensions.x > 0)
        {
            x -= _dimensions.x;
            y++;
        }
        x = x - 1;

        if(tile2DArray[x][y]->getIsAMine() == false)// this means that there is not a mine at this location
        {
            i++;
            tile2DArray[x][y]->setIsAMine(true);
            // check all the neighbors of the mined tile
            for(int k = 0; k < 8; k++)
            {
                if(tile2DArray[x][y]->getNeighbors()[k] != nullptr)//this means that there is an actual tile neighboring the mine
                {
                    tile2DArray[x][y]->getNeighbors()[k]->setNumber( tile2DArray[x][y]->getNeighbors()[k]->getNumber() + 1);//add 1 to the neighbor's number
                }
            }
        }
        // if there is already a mine, then don't do anything and continue the while loop to regenerate a random number
    }
}// end of Gamestate(sf::Vector2i, int)

GameState::~GameState()// Destructor
{
    for(int i = 0; i < tile2DArray.size(); i++)
    {
        for(int j = 0; j < tile2DArray[i].size(); j++)
        {
            if(tile2DArray[i][j] != nullptr)
            {
                delete tile2DArray[i][j];
            }
        }
    }
}// end of ~Gamestate()

GameState::GameState(const char *filepath)// Constructs a new game state based on the size, mine placement, and mine number specified at the filepath.
{
    this->playStatus = GameState::PLAYING;
    int _numberOfMines = 0;
    std::array<Tile*, 8> neighbors;
    std::array<bool, 8> neighborsMap;

    // open a file stream to read in the board data
    std::ifstream inputStream(filepath);
    inputStream.exceptions(std::ifstream::badbit);

    std::string tempBoard;

    // create a buffer and dump everything from the file to a string
    std::stringstream myBuffer;
    myBuffer << inputStream.rdbuf();
    tempBoard = myBuffer.str();

    // find the dimensions of the board
    int x = 0, y = 0;
    x = tempBoard.find("\n");
    y = tempBoard.size()/x;

    // fill the tile array with tiles according to the given dimensions
    tile2DArray.resize(x);
    for(int i = 0; i < x; i++)
    {
        tile2DArray[i] = std::vector<Tile*>(y);
        for(int j = 0; j < y; j++)
        {
            tile2DArray[i][j] = new Tile(sf::Vector2f(i, j));
            if(tempBoard[x*j + i + j] == '1')// this means that a mine has been found at the location
            {
                tile2DArray[i][j]->setIsAMine(true);
                _numberOfMines++;
            }
        }
    }

    // set the neighbors for all of the tiles
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            for(int i = 0; i < 8; i++)
            {
                neighborsMap[i] = true;
                neighbors[i] = nullptr;
            }
            if(i - 1 < 0)// this means the tile is on the left edge of the board
            {
                neighborsMap[0] = false;
                neighborsMap[3] = false;
                neighborsMap[5] = false;
            }
            else if(i + 1 >= x)// this means the tile is on the right edge of the board
            {
                neighborsMap[2] = false;
                neighborsMap[4] = false;
                neighborsMap[7] = false;
            }

            if(j - 1 < 0)// this means the tile is on the top edge of the board
            {
                neighborsMap[0] = false;
                neighborsMap[1] = false;
                neighborsMap[2] = false;
            }
            else if(j + 1 >= y)// this means the tile is on the bottom edge of the board
            {
                neighborsMap[5] = false;
                neighborsMap[6] = false;
                neighborsMap[7] = false;
            }

            // generate neighbor pointers according to the specified values in the boolean neighbors map
            if(neighborsMap[0] != false)
            {
                neighbors[0] = tile2DArray[i - 1][j - 1];
            }
            if(neighborsMap[1] != false)
            {
                neighbors[1] = tile2DArray[i][j - 1];
            }
            if(neighborsMap[2] != false)
            {
                neighbors[2] = tile2DArray[i + 1][j - 1];
            }
            if(neighborsMap[3] != false)
            {
                neighbors[3] = tile2DArray[i - 1][j];
            }
            if(neighborsMap[4] != false)
            {
                neighbors[4] = tile2DArray[i + 1][j];
            }
            if(neighborsMap[5] != false)
            {
                neighbors[5] = tile2DArray[i - 1][j + 1];
            }
            if(neighborsMap[6] != false)
            {
                neighbors[6] = tile2DArray[i][j + 1];
            }
            if(neighborsMap[7] != false)
            {
                neighbors[7] = tile2DArray[i + 1][j + 1];
            }
            tile2DArray[i][j]->setNeighbors(neighbors);
        }
    }

    // add numbers to the mines
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            if(tile2DArray[i][j]->getIsAMine() == true)// this means that there IS a mine at this location
            {
                // check all the neighbors of the mined tile
                for(int k = 0; k < 8; k++)
                {
                    if(tile2DArray[i][j]->getNeighbors()[k] != nullptr)// this means that there is an actual tile neighboring the mine
                    {
                        tile2DArray[i][j]->getNeighbors()[k]->setNumber( tile2DArray[i][j]->getNeighbors()[k]->getNumber() + 1);//add 1 to the neighbor's number
                    }
                }
            }
        }
    }
}// end of GameState(const char)

int GameState::getMineCount()// returns the current count of the number of mines actually on the board.
{
    int mineCounter = 0;
    for(int i = 0; i < tile2DArray.size(); i++)
    {
        for(int j = 0; j < tile2DArray[i].size(); j++)
        {
            if(tile2DArray[i][j]->getIsAMine() == true)
            {
                mineCounter++;
            }
        }
    }
    return mineCounter;
}// end of getMineCount()

int GameState::getFlagCount()// returns the current count of the number of flags placed on the screen.
{
    int flagCounter = 0;
    for(int i = 0; i < tile2DArray.size(); i++)
    {
        for(int j = 0; j < tile2DArray[i].size(); j++)
        {
            if(tile2DArray[i][j]->getState() == Tile::FLAGGED)
            {
                flagCounter++;
            }
        }
    }
    return flagCounter;
}// end of getFlagCount()

GameState::PlayStatus GameState::getPlayStatus()// Returns the play status of the game.
{
    return this->playStatus;
}// end of getPlayStatus()

void GameState::setPlayStatus(PlayStatus _status)// Sets the play status of the game
{
    this->playStatus = _status;
}// end of setPlayStatus()

Tile *GameState::getTile(int x, int y)// Returns a pointer to the Tile at the specified coordinates, or nullptr if out of bounds.
{
    if(x < tile2DArray.size() && x >= 0 && y < tile2DArray[0].size() && y >= 0)//this means that the requested coordinates do in fact exist
    {
        return tile2DArray[x][y];
    }
    else
    {
        return nullptr;
    }
}// end of getTile()


