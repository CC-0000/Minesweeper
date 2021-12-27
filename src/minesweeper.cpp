#include "minesweeper.h"
#include "Toolbox.h"
#include <iostream>

bool debugMode = false;

int launch()// this is the main game loop where the game events are captured and rendered
{
    Toolbox* global = Toolbox::getInstance();
    Toolbox::getInstance()->render();
    global->window.isOpen();
    while (global->window.isOpen())
    {
        // Process events
        sf::Event event;
        while (global->window.waitEvent(event))
        {
            // Close window: exit
            if (event.type == sf::Event::Closed)
                global->window.close();
            if(event.type == sf::Event::MouseButtonPressed)
            {
                // count how big the board is
                int x = 0, y = 0;// x and y represent the width and height of the board respectively
                while(global->gameState->getTile(x, 0) != nullptr)
                {
                    x++;
                }// while loop will exit when there exists no tile greater than the current x (width) value
                while(global->gameState->getTile(0, y) != nullptr)
                {
                    y++;
                }// while loop will exit when there exists no tile greater than the current y (height) value

                // Right Click
                if(event.mouseButton.button == sf::Mouse::Right && global->gameState->getPlayStatus() == GameState::PLAYING)
                {
                    if(event.mouseButton.x < x * 32 && event.mouseButton.y < y * 32)
                    {
                        global->gameState->getTile((int) event.mouseButton.x / 32, (int) event.mouseButton.y / 32)->onClickRight();
                    }
                }
                // Left Click
                else if(event.mouseButton.button == sf::Mouse::Left)
                {
                    // tile action
                    if(event.mouseButton.x < x * 32 && event.mouseButton.y < y * 32 && global->gameState->getPlayStatus() == GameState::PLAYING)
                    {
                        global->gameState->getTile((int) event.mouseButton.x / 32, (int) event.mouseButton.y / 32)->onClickLeft();
                    }
                    // button action
                    else if(event.mouseButton.y < (y + 2) * 32 && event.mouseButton.y > y * 32)
                    {
                        if(event.mouseButton.x > 11.5 * 32 && event.mouseButton.x < 13.5 * 32)
                        {
                            // new game button clicked
                            global->newGameButton->onClick();
                        }
                        else if(event.mouseButton.x > 15.5 * 32 && event.mouseButton.x < 17.5 * 32)
                        {
                            // debug button clicked
                            global->debugButton->onClick();
                        }
                        else if(event.mouseButton.x > 17.5 * 32 && event.mouseButton.x < 19.5 * 32)
                        {
                            // test 1 button clicked
                            global->testButton1->onClick();
                        }
                        else if(event.mouseButton.x > 19.5 * 32 && event.mouseButton.x < 21.5 * 32)
                        {
                            // test 2 button clicked
                            global->testButton2->onClick();
                        }

                    }
                }

                // check to see if the player has won the game
                if(global->gameState->getPlayStatus() == GameState::PLAYING)
                {
                    // assume the player has won the game
                    global->gameState->setPlayStatus(GameState::WIN);

                    // check to see if the player is still playing. If the player is no longer playing, that means that they have won
                    for(int i = 0; i < x; i++)
                    {
                        for(int j = 0; j < y; j++)
                        {
                            if(global->gameState->getTile(i, j)->getState() == Tile::HIDDEN)
                            {
                                if(global->gameState->getTile(i, j)->getIsAMine() == false)// this means that there exists a non mine tile that hasn't been revealed yet
                                {
                                    global->gameState->setPlayStatus(GameState::PLAYING);
                                }
                            }
                        }
                    }
                }
                else if(event.mouseButton.x < x * 32 && event.mouseButton.y < y * 32 && global->gameState->getPlayStatus() == GameState::LOSS && global->getDebugMode() == false)// this means that the player has lost
                {
                    // reveal the mines
                    global->toggleDebugMode();
                }

            }
        }

    }
    return 0;
}// end of launch()

void restart()// generates a new random board and restarts the game
{
    delete Toolbox::getInstance()->gameState;
    Toolbox::getInstance()->gameState = new GameState();
    if(Toolbox::getInstance()->getDebugMode() == true)
    {
        Toolbox::getInstance()->toggleDebugMode();
    }
    Toolbox::getInstance()->render();
}// end of restart()

void restartTest1()// generates a new board according to the specs of the first test board file
{
    delete Toolbox::getInstance()->gameState;
    Toolbox::getInstance()->gameState = new GameState("boards/testboard1.brd");
    if(Toolbox::getInstance()->getDebugMode() == true)
    {
        Toolbox::getInstance()->toggleDebugMode();
    }
    Toolbox::getInstance()->render();
}// end of restartTest1()

void restartTest2()// generates a new board according to the specs of the second test board file
{
    delete Toolbox::getInstance()->gameState;
    Toolbox::getInstance()->gameState = new GameState("boards/testboard2.brd");
    if(Toolbox::getInstance()->getDebugMode() == true)
    {
        Toolbox::getInstance()->toggleDebugMode();
    }
    Toolbox::getInstance()->render();
}// end of restartTest2()