#include "../include/Visualiser.h"

#include <SFML/Graphics.hpp>
#include <Eigen/Dense>

#include <iostream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <bitset>
#include <string>

using namespace sf;

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

    while(window.isOpen()){
        Event event;

        while(window.pollEvent(event)){
            if(event.type == Event::Closed) window.close();
        }

        window.clear(Color(245,245,220));

        for(Text& text : inputTexts){
            window.draw(text);
        }
        for(RectangleShape& bar : bars){
            window.draw(bar);
        }
        for(Text& text : outputTexts){
            window.draw(text);
        }

        window.display();
    }
}