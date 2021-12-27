#ifndef MINESWEEPER_TOOLBOX_H
#define MINESWEEPER_TOOLBOX_H
#include <SFML/Graphics.hpp>
#include "GameState.h"
#include "Button.h"

using namespace sf;
class Toolbox
{
    static Toolbox* instance;
    bool debugMode;
public:
    sf::RenderWindow window; // SFML application window
    GameState* gameState;    // Primary game state representation
    Button* debugButton;     // Turns debug on or off. Debug mode reveals all mines on the field
    Button* newGameButton;   // Resets / starts new game
    Button* testButton1;     // Loads test board #1
    Button* testButton2;     // Loads test board #2

    static Toolbox* getInstance();// Returns a pointer to the singular Toolbox instance and creates a new instance if the instance doesn't exist yet
    ~Toolbox();// Destructor
    void render();// Draws all the SFML elements onto the window display
    void toggleDebugMode();// turns the debug mode on or off
    bool getDebugMode();

private:
    void releaseInstance();// private destructor method
    Toolbox();// private constructor
};



#endif //MINESWEEPER_TOOLBOX_H
