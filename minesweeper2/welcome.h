#include <SFML/Graphics.hpp>
#include <fstream>
#pragma once
using namespace sf;
#include "gamey.h"

class Welcome{
public:
    int cols;
    int rows;
    int w;
    int l;
    int mines=0;
    Welcome(){

    }

    void readFile(){
        std::string col;
        std::string row;
        std::string mine;
        std::ifstream file("files/config.cfg");
        getline(file, col);
        cols = stoi(col);
        getline(file, row);
        rows = stoi(row);
        getline(file, mine);
        mines = stoi(mine);
        w = cols*32;
        l= rows*32+100;

    }
    Text loadTitle(Font &font){

        Text title("WELCOME TO MINESWEEPER!", font);
        title.setCharacterSize(24);
        title.setStyle(Text::Bold);
        title.setStyle(Text::Underlined);
        setText(title, w/2, l/2 - 150);
        return title;
    }

    Text loadName(Font &font){
        Text name("Enter your name:", font);
        name.setCharacterSize(20);
        name.setStyle(Text::Bold);
        setText(name, w/2, l/2 - 75);
        return name;
    }

    void setText(sf::Text &text, float x, float y){
        FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width/2.0f,
                       textRect.top + textRect.height/2.0f);
        text.setPosition(sf::Vector2f(x, y));
    }

};