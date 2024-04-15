#include "../include/Visualiser.h"

#include <Eigen/Dense>

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <string>

using namespace sf;


std::vector<Text> updateOutput(Eigen::VectorXcd outState, const Font& font, const int startY, const double stepY, const int charSize){

    std::vector<Text> outputTexts;

        std::ostringstream stream;

        for(int ii = 0; ii < outState.size(); ii++){
        stream.str(""); // Clear the stream
        stream.clear(); // Clear any flags

        // Apply fixed-point notation and set precision
        stream << std::fixed << std::setprecision(2);

        std::complex<double> num = outState[ii];

        stream << num.real();
        
        if (num.imag() >= 0)
            stream << " + " << std::abs(num.imag()) << "i";
        else
            stream << " - " << std::abs(num.imag()) << "i";
        
        Text text;
        text.setFont(font);
        text.setString(stream.str());
        text.setCharacterSize(charSize);
        text.setFillColor(Color::Black);
        text.setPosition(920, startY + ii * stepY);

        outputTexts.push_back(text);

    }

    return outputTexts;

    }

Visualiser::Visualiser(QComputer& qc, QAlgo alg): qComp(qc), algo(alg) {}

void Visualiser::show(){

    const int sizeX = 1200;
    const int sizeY = 900;
    const int charSize = 24;

    RenderWindow window(sf::VideoMode(sizeX, sizeY), "QComputer");

    Font font;
    if(!font.loadFromFile("../include/OpenSans-Regular.ttf")){
        throw std::invalid_argument("Error: Could not open font file");
    }

    Eigen::VectorXcd state = qComp.getState();

    algo(qComp, 7);
    Eigen::VectorXcd outState = qComp.getState();

    std::vector<Text> inputTexts, outputTexts;
    std::vector<RectangleShape> bars;

    const int startY = 50;
    const double stepY = (double) (sizeY - 2 * startY) / (double) (1 << qComp.getNQubits());

    std::ostringstream stream;

    for(int ii = 0 ; ii < state.size(); ii++){
        stream.str(""); // Clear the stream
        stream.clear(); // Clear any flags

        // Apply fixed-point notation and set precision
        stream << std::fixed << std::setprecision(2);

        std::complex<double> num = state[ii];

        std::bitset<32> binary(ii);

        std::string str = binary.to_string();
        str = str.substr(str.size() - qComp.getNQubits(), qComp.getNQubits());
        
        stream << "|" << str << ">  " << num.real();
        
        if (num.imag() >= 0)
            stream << " + " << std::abs(num.imag()) << "i";
        else
            stream << " - " << std::abs(num.imag()) << "i";
        
        Text text;
        text.setFont(font);
        text.setString(stream.str());
        text.setCharacterSize(charSize);
        text.setFillColor(Color::Black);
        text.setPosition(50, startY + ii * stepY);

        inputTexts.push_back(text);

        RectangleShape bar(Vector2f(600, 3));
        bar.setFillColor(Color::Black);
        bar.setPosition(300, startY + ii * stepY + 15);

        bars.push_back(bar);
    }

    outputTexts = updateOutput(outState, font, startY, stepY, charSize);
 
    Button applyButton(920, 10, 100, 40, font, "Apply", Color(245,245,220), Color::Black);

    while(window.isOpen()){
        Event event;

        while(window.pollEvent(event)){
            if(event.type == Event::Closed) window.close();
            if(event.type == Event::MouseButtonPressed){
                algo(qComp, 7);
                outState = qComp.getState();
                outputTexts = updateOutput(outState, font, startY, stepY, charSize);
            }
        }

        applyButton.update(window);
        window.clear(Color(245,245,220));

        for(Text& text : inputTexts){
            window.draw(text);
        }
        for(RectangleShape& bar : bars){
            window.draw(bar);
        }     
        for(Text& text : outputTexts){
            //segfault here
            window.draw(text);
        }

        window.display();
    }
}


Button::Button(float x, float y, float width, float height, const sf::Font& font, const std::string& text,
               const sf::Color& bgColor, const sf::Color& textColor)
    : backgroundColor(bgColor), hoverColor(sf::Color::Green), hovered(false) {
    shape.setPosition(sf::Vector2f(x, y));
    shape.setSize(sf::Vector2f(width, height));
    shape.setFillColor(backgroundColor);

    shape.setOutlineThickness(2.f);  // Set the thickness of the outline
    shape.setOutlineColor(sf::Color::Black);  // Set the color of the outline to black

    labelText.setFont(font);
    labelText.setString(text);
    labelText.setCharacterSize(24);
    labelText.setFillColor(textColor);
    labelText.setPosition(x + (width - labelText.getLocalBounds().width) / 2,
                          y + (height - labelText.getLocalBounds().height) / 2 - 10);
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(labelText);
}

bool Button::isMouseOver(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    return shape.getGlobalBounds().contains(mousePos.x, mousePos.y);
}

void Button::update(sf::RenderWindow& window) {
    hovered = isMouseOver(window);
    shape.setFillColor(hovered ? hoverColor : backgroundColor);
}