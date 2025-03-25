#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>
#include "sprite.h"
#include <chrono>
#pragma once

struct Tile{
    int x;
    int y;
    int c;
    int r;
    int mineCount;
    bool flag=false;
    bool open=false;
    bool mine=false;
    std::vector<Tile*> neighbors;
    Tile(int cols, int rows, int xx, int yy){
        x=xx;
        y=yy;
        c=cols;
        r=rows;
    }
    void reset(){
        flag = false;
        mine = false;
        open = 0;
        mineCount=0;
    }
    bool openT(std::vector<Tile>& tiles) {
        if (open) {
            return false;
        }

        if (mine) {
            open = true;
            return true;
        }

        open = true;

        if (minesAround() == 0) {
            for (Tile* neighbor : neighbors) {
                if (neighbor != nullptr && !neighbor->open && !neighbor->mine) {
                    neighbor->openT(tiles);
                }
            }
        }

        return false;
    }

    Tile* getTile(int xx, int yy, std::vector<Tile>& ttiles){

        return &ttiles[xx+c*yy];
    }
    void tilesAround(std::vector<Tile>& tiles) {
        if (x + 1 < c) {
            neighbors.push_back(getTile(x + 1, y, tiles));
            if (y + 1 < r) {
                neighbors.push_back(getTile(x + 1, y + 1, tiles));
            }
            if (y - 1 >= 0) {
                neighbors.push_back(getTile(x + 1, y - 1, tiles));
            }
        }
        if (x - 1 >= 0) {
            neighbors.push_back(getTile(x - 1, y, tiles));
            if (y + 1 < r) {
                neighbors.push_back(getTile(x - 1, y + 1, tiles));
            }
            if (y - 1 >= 0) {
                neighbors.push_back(getTile(x - 1, y - 1, tiles));
            }
        }
        if (y + 1 < r) {
            neighbors.push_back(getTile(x, y + 1, tiles));
        }

        if (y - 1 >= 0) {
            neighbors.push_back(getTile(x, y - 1, tiles));
        }
    }

    int minesAround(){
        int aMines=0;
        if(mine ==0){
            for(Tile* n: neighbors){
                if(n->mine==1){
                    aMines++;
                }
            }
            mineCount=aMines;
            return aMines;

        }
        return -1;
    }

};

struct Game{

    int l;
    int w;
    int cols;
    int rows;
    int mines=0;
    int game_end = 0;
    bool revealOn = false;
    bool pause=false;
    bool leadOn=false;
    int minesLeft;




    Game(int width, int length, int columns, int rrows, int mine){
        w=width;
        l=length;
        cols=columns;
        rows=rrows;
        mines=mine;
    }
    void reset(){
        game_end=0;
        bool revealOn = false;
        bool pause=false;
        bool leadOn=false;
    }
    Tile* getTile(int xx, int yy, std::vector<Tile>& ttiles){
        if(xx<0){
            xx=0;
        }
        else if(xx>cols-1){
            xx=cols-1;
        }
        if(yy<0){
            yy=0;
        }
        else if(yy>rows-1){
            yy=rows-1;
        }
        return &ttiles[xx+cols*yy];
    }

    void counterTexture(SpriteCache& sprites){
        int hundreds=minesLeft/100;
        int tens= minesLeft/10%10;
        int ones = minesLeft%10;
        std::vector<int> nums{0,1,2,3,4,5,6,7,8,9};
        if(minesLeft>=0){
            for(int& i: nums){
                if(i==hundreds){
                    sprites.counter1.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
                }
                if(i==tens){
                    sprites.counter2.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
                }
                if(i==ones){
                    sprites.counter3.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
                }
            }
        }
        else{
            for(int& i: nums) {
                if (-i == hundreds) {
                    sprites.counter1.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
                }
                if (-i == tens) {
                    sprites.counter2.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
                }
                if (-i == ones) {
                    sprites.counter3.setTextureRect(sf::IntRect(i * 21, 0, 21, 32));
                }
            }
        }
        if(game_end==1){
            minesLeft=0;
            sprites.counter1.setTextureRect(sf::IntRect(0 * 21, 0, 21, 32));
            sprites.counter2.setTextureRect(sf::IntRect(0 * 21, 0, 21, 32));
            sprites.counter3.setTextureRect(sf::IntRect(0 * 21, 0, 21, 32));
        }


    }

    void drawer(sf::RenderWindow& window, std::vector<Tile>& tiles, SpriteCache& sprites) {

        sprites.debugSprite.setPosition((32 * cols - 304), 32 * (rows + 0.5));
        sprites.play.setPosition(cols*32-240, 32*(rows+0.5));
        sprites.pause.setPosition(cols*32-240, 32*(rows+0.5));
        sprites.happy.setPosition((cols/2.0)*32-32,32*(rows+0.5));
        sprites.win.setPosition((cols/2.0)*32-32,32*(rows+0.5));
        sprites.lose.setPosition((cols/2.0)*32-32,32*(rows+0.5));
        sprites.leaderboard.setPosition(cols*32-176,32*(rows+0.5) );
        counterTexture(sprites);
        sprites.counter1.setPosition(33, 32*(rows+0.5)+16);
        sprites.counter2.setPosition(54, 32*(rows+0.5)+16);
        sprites.counter3.setPosition(75, 32*(rows+0.5)+16);
        sprites.negative.setPosition(12,32*(rows+0.5)+16);


        window.draw(sprites.debugSprite);
        window.draw(sprites.leaderboard);
        if(minesLeft<0){
            window.draw(sprites.negative);
        }
        window.draw(sprites.counter1);
        window.draw(sprites.counter2);
        window.draw(sprites.counter3);
        if(game_end==0){
            window.draw(sprites.happy);
        }
        else if(game_end==-1){
            window.draw(sprites.lose);
        }
        else if(game_end==1){
            window.draw(sprites.win);
        }

        if(!pause) {
            window.draw(sprites.pause);
            for (Tile &tile: tiles) {

                sprites.hidden.setPosition(tile.x * 32, tile.y * 32);
                sprites.revealed.setPosition(tile.x * 32, tile.y * 32);

                if (tile.open) {
                    if (tile.mine) {

                        window.draw(sprites.revealed);
                        if (tile.flag) {
                            sprites.flag.setPosition(tile.x * 32, tile.y * 32);
                            window.draw(sprites.flag);
                        }
                        sf::Sprite mineSprite(sprites.mine);
                        mineSprite.setPosition(tile.x * 32, tile.y * 32);
                        window.draw(mineSprite);
                    } else {

                        window.draw(sprites.revealed);
                        int minesAround = tile.mineCount;
                        if (minesAround > 0) {
                            sf::Texture numberTexture;
                            numberTexture.loadFromFile("files/images/number_" + std::to_string(minesAround) + ".png");
                            sf::Sprite numberSprite(numberTexture);
                            numberSprite.setPosition(tile.x * 32, tile.y * 32);
                            window.draw(numberSprite);
                        }
                    }
                } else {

                    window.draw(sprites.hidden);
                    if (revealOn == 1) {
                        if (tile.mine) {
                            sprites.mine.setPosition(tile.x * 32, tile.y * 32);
                            window.draw(sprites.mine);
                        }
                    }
                    if (tile.flag) {
                        sprites.flag.setPosition(tile.x * 32, tile.y * 32);
                        window.draw(sprites.flag);
                    }

                    if (game_end == -1 && tile.mine) {

                        sprites.revealed.setPosition(tile.x * 32, tile.y * 32);
                        window.draw(sprites.revealed);
                        if (tile.flag) {

                            sprites.flag.setPosition(tile.x * 32, tile.y * 32);
                            window.draw(sprites.flag);
                        }
                        sprites.mine.setPosition(tile.x * 32, tile.y * 32);
                        window.draw(sprites.mine);
                    } else if (game_end == 1 && tile.mine) {
                        sprites.flag.setPosition(tile.x * 32, tile.y * 32);
                        window.draw(sprites.flag);
                    }
                }

            }
        }
        else{
            for(int i=0;i<cols;i++){
                for(int j=0;j<rows;j++){
                    sprites.revealed.setPosition(i*32,j*32);
                    window.draw(sprites.revealed);
                }
            }
            window.draw(sprites.play);
        }
    }

    void loadMines(std::vector<Tile>& tiles){


        for(int i=0;i<mines;i++){

            int minex = 0+rand()%cols-1;
            int miney = 0+rand()%rows-1;
            if(getTile(minex, miney, tiles)->mine==1){
                i--;
            }
            else{
                getTile(minex, miney, tiles)->mine=1;
            }
        }
/*        int s=0;
        for(Tile& t:tiles){

            if(t.mine==1){
                s++;
                std::cout<<t.x<<", "<<t.y<<std::endl;

            }
        }
        std::cout<<s<<std::endl;
*/
    }

    void openTile(int xx, int yy, std::vector<Tile>& tiles){

        if(game_end==0 && getTile(xx, yy, tiles)->flag==0){
            if(getTile(xx, yy, tiles)->openT(tiles)){
                game_end=-1;
            }
            else{
                int tiles_left=0;
                for(Tile& tile: tiles){
                    if(tile.open==0){
                        tiles_left++;
                    }
                }
                if(mines==tiles_left){
                    game_end=1;
                }
            }

        }
    }
    void flagTile(float x, float y, std::vector<Tile>& tiles, sf::RenderWindow& w){
        if(game_end==0){
            if(getTile(x, y, tiles)->open==0){
                if(getTile(x, y, tiles)->flag==1){
                    getTile(x, y, tiles)->flag=0;
                    minesLeft++;
                }
                else {
                    getTile(x, y, tiles)->flag = 1;
                    minesLeft--;
                }
            }
        }

    }


};