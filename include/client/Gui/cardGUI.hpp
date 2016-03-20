#ifndef CARDGUI_HPP
#define CARDGUI_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class CardGUI
{
private:

    // Back view texture and sprite
    sf::Sprite _backView;
    sf::Texture _backTexture;

    // Front view texture and sprite
    sf::Sprite _frontView;
    sf::Texture _frontTexture;
    
    // Needed attributs
    std::vector<EffectParamsCollection> _effects;  // Effects
    int _cost;

public:
    CardGUI();

    /// Getters
    sf::Sprite getBack() {return _backView;}
    sf::Sprite getFront() {return _frontView;}

};



#endif //CARDGUI_HPP
