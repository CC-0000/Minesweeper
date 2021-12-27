#ifndef MINESWEEPER_BUTTON_H
#define MINESWEEPER_BUTTON_H
#include <SFML/Graphics.hpp>

using namespace sf;
class Button
{
    sf::Vector2f position;// stores the current position of the Button
    sf::Sprite sprite;// stores the current sprite of the Button
    std::function<void(void)> _onClick;// stores the method that the button should call when clicked

public:
    Button(sf::Vector2f _position, std::function<void(void)> _onClick);// Constructs a new object at the specified _position which invokes the _onClick callback when clicked.
    sf::Vector2f getPosition();// Returns the position of the button.
    sf::Sprite* getSprite();// Returns the current sprite of the button.
    void setSprite(sf::Sprite* _sprite);// Sets this button’s visualization to the specified _sprite.
    void onClick();// Invokes the button’s callback method (usually called when clicked).
};


#endif //MINESWEEPER_BUTTON_H
