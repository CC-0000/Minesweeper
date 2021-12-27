#include <iostream>
#include "Tile.h"
#include "Toolbox.h"

Tile::Tile(sf::Vector2f position)// Constructs a new tile object at the designated _position.
{
    // set the position of the tile Object and set the neighbor pointers to null pointers.
    this->number = 0;
    this->isAMine = false;
    this->state = HIDDEN;
    this->position = sf::Vector2f(position.x, position.y);
    for(int i = 0; i < 8; i++)
    {
        this->neighbors[i] = nullptr;
    }
}// end of Tile(sf::Vector2f)

sf::Vector2f Tile::getLocation()// Returns the position of this tile.
{
    return this->position;
}// end of getLocation()

Tile::State Tile::getState()// Returns current state of this tile.
{
    return this->state;
}// end of getState()

std::array<Tile*, 8>& Tile::getNeighbors()// Returns pointer to array of Tile pointers (see Figure 2 for ordering).
{
    return this->neighbors;
}// end of getNeighbors()

void Tile::setState(State _state)// Sets the state of this tile. Should trigger other behaviors related to the state change (including visualization).
{
    this->state = _state;
}// end of setState(State)

void Tile::setNeighbors(std::array<Tile*, 8> _neighbors)// Populates / replaces the neighboring tile container.
{
    this->neighbors[0] = _neighbors[0];
    this->neighbors[1] = _neighbors[1];
    this->neighbors[2] = _neighbors[2];
    this->neighbors[3] = _neighbors[3];
    this->neighbors[4] = _neighbors[4];
    this->neighbors[5] = _neighbors[5];
    this->neighbors[6] = _neighbors[6];
    this->neighbors[7] = _neighbors[7];
}// end of setNeighbors(std::array)

void Tile::setIsAMine(bool isAMine)
{
    this->isAMine = isAMine;
}// end of setIsAMine(bool)

bool Tile::getIsAMine()
{
    return this->isAMine;
}// end of getIsAMine()

void Tile::setNumber(int number)
{
    this->number = number;
}// end of setNumber(int)

int Tile::getNumber()
{
    return this->number;
}// end of getNumber()

void Tile::onClickLeft()// Defines the reveal behavior for a tile when the left mouse button is clicked inside its boundaries.
{
    if(this->state == HIDDEN)
    {
        if(getIsAMine() == true)
        {
            this->state = EXPLODED;// this should end the game
            Toolbox::getInstance()->gameState->setPlayStatus(GameState::LOSS);
        }
        else// this means that the tile is not a mine
        {
            if(number == 0)
            {
                revealNeighbors();
            }
            this->state = REVEALED;
            // this means that the tile is a blank one
        }
        Toolbox::getInstance()->render();
    }
}// end of onClickLeft()

void Tile::onClickRight()// Toggles this tile’s state between FLAGGED and HIDDEN.
{
    if(this->state == FLAGGED)
    {
        this->state = HIDDEN;
    }
    else if(this->state == HIDDEN)
    {
        this->state = FLAGGED;
    }
    draw();
    Toolbox::getInstance()->render();
}// onClickRight()

void Tile::draw()// Render this tile to the screen according to is state.
{
    RenderTexture base;
    base.create(32, 32);
    Texture texture;
    Sprite tempSprite;
    std::string filename;

    // based on the current state of the tile, create the corresponding sprite
    switch (state)
    {
        case REVEALED:
            // load the base image for a revealed tile and draw it onto the RenderTexture
            filename = "images/tile_revealed.png";
            texture.loadFromFile(filename);
            tempSprite.setTexture(texture);
            base.draw(tempSprite);

            // load the number image for the revealed tile and draw it onto the RenderTexture
            if(this->number != 0)
            {
                filename = "images/number_" + std::to_string(number) + ".png";
                texture.loadFromFile(filename);
                tempSprite.setTexture(texture);
                base.draw(tempSprite);
            }
            // convert the RenderTexture to a usable sprite
            base.display();
            tempSprite.setTexture(base.getTexture());
            break;

        case HIDDEN:
            // load the image for a hidden tile
            filename = "images/tile_hidden.png";
            texture.loadFromFile(filename);
            tempSprite.setTexture(texture);
            break;

        case FLAGGED:
            // load the base image for a hidden tile and draw it onto the RenderTexture
            filename = "images/tile_hidden.png";
            texture.loadFromFile(filename);
            tempSprite.setTexture(texture);
            base.draw(tempSprite);

            // load the flag image and draw it onto the RenderTexture
            filename = "images/flag.png";
            texture.loadFromFile(filename);
            tempSprite.setTexture(texture);
            base.draw(tempSprite);

            // convert the RenderTexture to a usable sprite
            base.display();
            tempSprite.setTexture(base.getTexture());
            break;

        case EXPLODED:
            // load the base image for a hidden tile and draw it onto the RenderTexture
            filename = "images/tile_hidden.png";
            texture.loadFromFile(filename);
            tempSprite.setTexture(texture);
            base.draw(tempSprite);

            // load the mine image and tile and draw it onto the RenderTexture
            filename = "images/mine.png";
            texture.loadFromFile(filename);
            tempSprite.setTexture(texture);
            base.draw(tempSprite);

            // convert the RenderTexture to a usable sprite
            base.display();
            tempSprite.setTexture(base.getTexture());
            break;
    }

    //based on the stored position of the tile, draw the sprite onto the window
    tempSprite.setPosition(this->position.x * 32, this->position.y * 32);
    Toolbox::getInstance()->window.draw(tempSprite);

}// end of draw()

void Tile::revealNeighbors()//Based on State and mine content of the tile neighbors, set their state to REVEALED.
{
    /*
     * things to note:
     *      - the caller of reveal neighbors assumes that the tile it is calling from is a blank tile
     *      - a number of 0 represents a blank tile
     */

    // cascade reveal all blank tiles
    for(int i = 0; i < 8; i++)
    {
        if(this->neighbors[i] != nullptr)// there is a non-mine tile next to it
        {
            if(this->neighbors[i]->getNumber() == 0 && this->neighbors[i]->getState() != REVEALED)//the non-mine tile is a blank tile
            {
                this->neighbors[i]->setState(REVEALED);
                this->neighbors[i]->revealNeighbors();
            }
            else// the non-mine tile is a number tile
            {
                this->neighbors[i]->setState(REVEALED);
            }
        }
    }
}// end of revealNeighbors()
