#include "QComputer.h"
#include "QAlgorithms.h"

#ifndef VISUALISER_H
#define VISUALISER_H

#include <SFML/Graphics.hpp>

class Visualiser{
    public:
        typedef void (*QAlgo)(QComputer&, int);

        /// @brief Initilaises a Visualiser object to create an interactive representaton of the simulated quantum computer
        /// @param qc The computer to visualise
        Visualiser(QComputer& qc, QAlgo alg);

        /// @brief Displayes the interactive representation of the quantum computer
        void show();

    private:
        QComputer& qComp;
        QAlgo algo;

};

class Button {
public:
    Button(float x, float y, float width, float height, const sf::Font& font, const std::string& text,
           const sf::Color& bgColor, const sf::Color& textColor);

    void draw(sf::RenderWindow& window);
    bool isMouseOver(sf::RenderWindow& window);
    void update(sf::RenderWindow& window);

private:
    sf::RectangleShape shape;
    sf::Text labelText;
    sf::Color backgroundColor;
    sf::Color hoverColor;
    bool hovered;
};

#endif