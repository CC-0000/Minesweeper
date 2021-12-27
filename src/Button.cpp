#include "Button.h"
Button::Button(sf::Vector2f _position, std::function<void(void)> _onClick)// Constructs a new object at the specified _position which invokes the _onClick callback when clicked.
{
    this->position = _position;
    this->_onClick = _onClick;
}// end of Button(sf::Vector2f, std::function)

sf::Vector2f Button::getPosition()// Returns the position of the button.
{
    return this->position;
}// end of getPosition()

sf::Sprite* Button::getSprite()// Returns the current sprite of the button.
{
    return &this->sprite;
}// end of getSprite()

void Button::setSprite(sf::Sprite* _sprite)// Sets this button’s visualization to the specified _sprite.
{
    this->sprite = *_sprite;
}// end of setSprite()

void Button::onClick()// Invokes the button’s callback method (usually called when clicked).
{
    this->_onClick();
}// end of onClick()