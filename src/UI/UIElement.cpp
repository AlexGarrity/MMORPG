/**     Alex Garrity, 2018      **/

#include "UIElement.h"

void UIElement::Update(sf::Vector2f mousePosition)
{
    if (mousePosition.x > position.x && mousePosition.x < (position.x + sprite.getLocalBounds().width)) {
        if (mousePosition.y > position.y && mousePosition.y < (position.y + sprite.getLocalBounds().height)) {
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                // If the user has pressed something, call OnPress, and set heldDown to true
                OnPress();
                heldDown = true;
            }
            else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left) && heldDown == true) {
                // If the user is not pressing the object, and it's tagged as held down
                // Call OnRelease and set heldDown to false
                OnRelease();
                heldDown = false;
            }
            else {
                // If neither of those conditions are true, call OnHover
                OnHover();
            }
        }
    }
}

sf::Sprite& UIElement::GetSprite()
{
    //Return the sprite.  Used when drawing.
    return sprite;
}
