#include "Toolbox.h"
#include "minesweeper.h"

Toolbox* Toolbox::instance = nullptr;

Toolbox::Toolbox()// private constructor
{
    // create a new window
    window.create(sf::VideoMode(800, 600), "Minesweeper");
    window.setFramerateLimit(0);
    window.setVerticalSyncEnabled(true);
    // create a new GameState
    gameState = new GameState(Vector2i(25, 16), 50);

    // create new buttons to add to the window
    newGameButton = new Button(sf::Vector2f(12.5, 16), restart);
    debugButton = new Button(sf::Vector2f(16.5, 16), [](){Toolbox::getInstance()->toggleDebugMode();Toolbox::getInstance()->render();});
    testButton1 = new Button(sf::Vector2f(18.5, 16), restartTest1);
    testButton2 = new Button(sf::Vector2f(20.5, 16), restartTest2);
}// end of Toolbox()

Toolbox *Toolbox::getInstance()// Returns a pointer to the singular Toolbox instance and creates a new instance if the instance doesn't exist yet
{
    if(instance == nullptr)
    {
        instance = new Toolbox();
    }
    return instance;
}// end of getInstance()

Toolbox::~Toolbox()// public destructor
{
    releaseInstance();
}// end of ~Toolbox()

void Toolbox::releaseInstance()// private destructor method
{
    if(instance != nullptr)
    {
        delete instance;
    }
    if(gameState != nullptr)
    {
        delete gameState;
    }
    if(newGameButton != nullptr)
    {
        delete newGameButton;
    }
    if(testButton1 != nullptr)
    {
        delete testButton1;
    }
    if(testButton2 != nullptr)
    {
        delete testButton2;
    }
}// end of releaseInstance()

void Toolbox::render()
{
    // Clear screen
    this->window.clear(Color(255, 255, 255, 255));

    sf::Texture mineTexture;
    sf::Texture hiddenTexture;
    sf::Sprite mineSprite;
    sf::Sprite hiddenSprite;

    // load the base image for a hidden tile and draw it onto the RenderTexture
    hiddenTexture.loadFromFile("images/tile_hidden.png");
    hiddenSprite.setTexture(hiddenTexture);

    // load the mine image and tile and draw it onto the RenderTexture
    mineTexture.loadFromFile("images/mine.png");
    mineSprite.setTexture(mineTexture);

    // count how big the board is
    int x = 0, y = 0;// x and y represent the width and height of the board respectively
    while(this->gameState->getTile(x, 0) != nullptr)
    {
        x++;
    }// while loop will exit when there exists no tile greater than the current x (width) value
    while(this->gameState->getTile(0, y) != nullptr)
    {
        y++;
    }// while loop will exit when there exists no tile greater than the current y (height) value

    // draw all of the tiles
    for(int i = 0; i < x; i++)
    {
        for(int j = 0; j < y; j++)
        {
            if(this->debugMode == true && this->gameState->getTile(i, j)->getIsAMine() == true)// this means that debug mode is on and the tile is a mine
            {
                // draw a mine sprite
                hiddenSprite.setPosition(this->gameState->getTile(i, j)->getLocation().x * 32, this->gameState->getTile(i, j)->getLocation().y * 32);
                mineSprite.setPosition(this->gameState->getTile(i, j)->getLocation().x * 32, this->gameState->getTile(i, j)->getLocation().y * 32);

                this->window.draw(hiddenSprite);
                this->window.draw(mineSprite);
            }
            else// this means that debug mode is off
            {
                // draw a sprite according to the current state of the tile
                this->gameState->getTile(i, j)->draw();
            }
        }
    }

    // draw all of the buttons

    // set up the newGameButton
    sf::Texture tempTexture;
    sf::Sprite tempSprite;
    switch(this->gameState->getPlayStatus())
    {
        case GameState::PLAYING:
            tempTexture.loadFromFile("images/face_happy.png");
            tempSprite.setTexture(tempTexture);
            break;
        case GameState::WIN:
            tempTexture.loadFromFile("images/face_win.png");
            tempSprite.setTexture(tempTexture);
            break;
        case GameState::LOSS:
            tempTexture.loadFromFile("images/face_lose.png");
            tempSprite.setTexture(tempTexture);
            break;
    }
    tempSprite.setOrigin(32, 0);
    tempSprite.setPosition(32*this->newGameButton->getPosition().x, 32*this->newGameButton->getPosition().y);
    this->newGameButton->setSprite(&tempSprite);

    // set up the testButtons
    sf::Texture tempTexture1;
    sf::Sprite tempSprite1;
    tempTexture1.loadFromFile("images/test_1.png");
    tempSprite1.setTexture(tempTexture1);
    tempSprite1.setOrigin(32, 0);
    tempSprite1.setPosition(32*this->testButton1->getPosition().x, 32*this->testButton1->getPosition().y);
    this->testButton1->setSprite(&tempSprite1);

    sf::Texture tempTexture2;
    sf::Sprite tempSprite2;
    tempTexture2.loadFromFile("images/test_2.png");
    tempSprite2.setTexture(tempTexture2);
    tempSprite2.setOrigin(32, 0);
    tempSprite2.setPosition(32*this->testButton2->getPosition().x, 32*this->testButton2->getPosition().y);
    this->testButton2->setSprite(&tempSprite2);

    //s et up the debug button
    sf::Texture tempTexture3;
    sf::Sprite tempSprite3;
    tempTexture3.loadFromFile("images/debug.png");
    tempSprite3.setTexture(tempTexture3);
    tempSprite3.setOrigin(32, 0);
    tempSprite3.setPosition(32*this->debugButton->getPosition().x, 32*this->debugButton->getPosition().y);
    this->debugButton->setSprite(&tempSprite3);

    // draw the buttons onto the screen
    this->window.draw(*this->newGameButton->getSprite());
    this->window.draw(*this->testButton1->getSprite());
    this->window.draw(*this->testButton2->getSprite());
    this->window.draw(*this->debugButton->getSprite());

    // draw the flag counter
    int h = this->gameState->getMineCount() - this->gameState->getFlagCount();
    std::string flagsLeft;
    char tempString[3];

    if(h < 0)// this means that there is a negative flag count
    {
        h = -1 * h;
        if(h > 99)
        {
            h = 99;
        }
        flagsLeft = std::to_string(h);

        if(flagsLeft.size() == 1)
        {
            flagsLeft = "-0" + flagsLeft;
        }
        h = -1 * h;
    }
    else// this means that there is a positive flag count
    {
        flagsLeft = std::to_string(h);
    }

    // add in zeros to the left of the number accordingly
    while(flagsLeft.size() < 3)
    {
        flagsLeft = "0" + flagsLeft;
    }

    // import the image with all the digits in
    sf::Texture flagTexture;
    flagTexture.loadFromFile("images/digits.png");

    // create 3 sprites for the 3 digits and set their sprites according to what number they are
    sf::Sprite firstDigit;
    firstDigit.setTexture(flagTexture);
    if(h < 0)
    {
        firstDigit.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
    }
    else
    {
        firstDigit.setTextureRect(sf::IntRect((flagsLeft[0] - '0') * 21, 0, 21, 32));
    }
    sf::Sprite secondDigit;
    secondDigit.setTexture(flagTexture);
    secondDigit.setTextureRect(sf::IntRect((flagsLeft[flagsLeft.size() - 2] - '0') * 21, 0, 21, 32));
    sf::Sprite thirdDigit;
    thirdDigit.setTexture(flagTexture);
    thirdDigit.setTextureRect(sf::IntRect((flagsLeft[flagsLeft.size() - 1] - '0') * 21, 0, 21, 32));

    // order the digits horizontally
    firstDigit.setPosition(0, 32*y);
    secondDigit.setPosition(21, 32*y);
    thirdDigit.setPosition(42, 32*y);

    // draw the digits
    this->window.draw(firstDigit);
    this->window.draw(secondDigit);
    this->window.draw(thirdDigit);

    //display all the drawn elements onto the screen
    this->window.display();
}// end of render()

void Toolbox::toggleDebugMode()// turns the debug mode on or off
{
    if(getDebugMode() == true)
    {
        debugMode = false;
    }
    else
    {
        debugMode = true;
    }
}// end of toggleDebugMode()

bool Toolbox::getDebugMode()
{
    return debugMode;
}// end of getDebugMode()
