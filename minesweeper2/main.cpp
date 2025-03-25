#include "welcome.h"
#include "gamey.h"
#include <SFML/Graphics.hpp>
#include "sprite.h"
#include <vector>
#include <chrono>
#pragma once


struct Timer {
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<double>;

    TimePoint start_time;
    Duration elapsed_time = Duration(0);
    bool running = false;

    void start() {
        if (!running) {
            start_time = Clock::now();
            running = true;
        }
    }


    void pause() {
        if (running) {
            TimePoint now = Clock::now();
            elapsed_time += std::chrono::duration_cast<Duration>(now - start_time);
            running = false;
        }
    }

    void resume() {
        if (!running) {
            start_time = Clock::now();
            running = true;
        }
    }

    // Reset the stopwatch
    void reset() {
        elapsed_time = Duration(0);
        running = false;
    }

    // Get the total elapsed time in seconds
    double get_elapsed_time() const {
        if (running) {
            TimePoint now = Clock::now();
            return elapsed_time.count() + std::chrono::duration_cast<Duration>(now - start_time).count();
        } else {
            return elapsed_time.count();
        }
    }
};
void setText(sf::Text &text, float x, float y){
    FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

int main() {
    std::srand(time(0));
    RenderWindow gamer;


    RenderWindow leaderboard;
    Welcome wel;
    wel.readFile();
    RenderWindow welcome(VideoMode(wel.w,wel.l), "welcome", Style::Close);
    Font font;
    font.loadFromFile("files/font.ttf");
    Text inputText("|", font);
    String input;
    inputText.setFillColor(Color::Yellow);
    inputText.setCharacterSize(18);

    int count=0;
    while(welcome.isOpen()) {
        Event event;
        while(welcome.pollEvent(event)) {
            if(event.type == Event::Closed) {
                welcome.close();
                exit(0);
            }
            if (event.type == Event::TextEntered) {
                if (count!=10) {
                    if (isalpha(char(event.text.unicode))) {
                        if (count == 0) {
                            char x = toupper(char(event.text.unicode));
                            input += x;
                        } else {
                            char x = tolower(char(event.text.unicode));
                            input += x;
                        }

                        count++;
                    }
                }
                if(event.text.unicode == '\b'){
                    if (count!=0) {
                        input.erase(count - 1, 1);
                        count--;
                    }
                }
                inputText.setString(input + "|");
            }
            else if(Keyboard::isKeyPressed(sf::Keyboard::Return)){
                if(count!=0){
                    welcome.close();

                }

            }

        }
        welcome.clear(Color::Blue);
        welcome.draw(wel.loadTitle(font));
        welcome.draw(wel.loadName(font));
        wel.setText(inputText, wel.w/2, wel.l/2-45);
        welcome.draw(inputText);
        welcome.display();

    }
    Game game(wel.w, wel.l, wel.cols, wel.rows, wel.mines);
    gamer.create(VideoMode(wel.w, wel.l), "Minesweeper", Style::Close);
    sf::Texture hidden, revealed, flag, mine, debugTexture, pause_tex, play_tex, leaderboard_tex, happy_tex, win_tex, lose_tex, counter_tex;
    hidden.loadFromFile("files/images/tile_hidden.png");
    revealed.loadFromFile("files/images/tile_revealed.png");
    flag.loadFromFile("files/images/flag.png");
    mine.loadFromFile("files/images/mine.png");
    debugTexture.loadFromFile("files/images/debug.png");
    pause_tex.loadFromFile("files/images/pause.png");
    play_tex.loadFromFile("files/images/play.png");
    leaderboard_tex.loadFromFile("files/images/leaderboard.png");
    happy_tex.loadFromFile("files/images/face_happy.png");
    win_tex.loadFromFile("files/images/face_win.png");
    lose_tex.loadFromFile("files/images/face_lose.png");
    counter_tex.loadFromFile("files/images/digits.png");
    SpriteCache sprites(hidden, revealed, flag, mine, play_tex, pause_tex, debugTexture, leaderboard_tex, happy_tex, win_tex, lose_tex, counter_tex);
    std::vector<Tile> tiles;
    for(int i=0;i<game.rows;i++){
        for(int j=0;j<game.cols;j++){
            tiles.push_back(Tile(game.cols, game.rows,j,i));
        }
    }
    game.loadMines(tiles);
    for(Tile& tile: tiles) {
        tile.tilesAround(tiles);
    }
    game.minesLeft=wel.mines;
    bool leadOpened=false;
    Timer time;
    time.start();
    game:
    while(gamer.isOpen()) {
        gamer.clear(Color::White);
        game.drawer(gamer, tiles, sprites);
        gamer.display();
        sf::Event event;
        sf::Vector2i pos=sf::Mouse::getPosition(gamer);
        auto translate = gamer.mapPixelToCoords(pos);
        while (gamer.pollEvent(event)) {


            if (event.type == sf::Event::Closed) {
                gamer.close();
                exit(0);
            }

            if(event.type==sf::Event::MouseButtonPressed){
                int mouse_tile_x = pos.x /32;
                int mouse_tile_y = pos.y/32;
                //std::cout<<mouse_tile_x<<", "<<mouse_tile_y<<std::endl;
                if(event.mouseButton.button==sf::Mouse::Left)
                {

                    if(game.game_end==0) {
                        if(!game.pause){
                            if (mouse_tile_x < game.cols && mouse_tile_y < game.rows) {
                                game.openTile(mouse_tile_x, mouse_tile_y, tiles);
                                break;
                            } else if (sprites.debugSprite.getGlobalBounds().contains(translate)) {
                                if (game.revealOn == 0) {
                                    game.revealOn = 1;
                                } else {
                                    game.revealOn = 0;
                                }

                            }else if(sprites.pause.getGlobalBounds().contains(translate)){
                                if (game.pause == 0) {
                                    game.pause = 1;
                                }
                            }
                            else if(sprites.happy.getGlobalBounds().contains(translate)){
                                for(Tile& t:tiles){
                                    t.reset();
                                }
                                game.loadMines(tiles);
                                game.game_end=0;
                                game.pause=false;
                                game.leadOn=false;
                                game.minesLeft=game.mines;
                            }
                            else if(sprites.leaderboard.getGlobalBounds().contains(translate)){
                                game.pause=1;
                                game.leadOn=1;
                                goto game;
                            }
                        }
                        else if(sprites.pause.getGlobalBounds().contains(translate)){
                            game.pause = 0;
                        }
                        else if(sprites.happy.getGlobalBounds().contains(translate)){
                            for(Tile& t:tiles){
                                t.reset();
                            }
                            game.loadMines(tiles);
                            game.game_end=0;
                            game.pause=false;
                            game.leadOn=false;
                            game.minesLeft=game.mines;
                        }
                        else if(sprites.leaderboard.getGlobalBounds().contains(translate)){
                            game.pause=1;
                            goto leader;
                        }

                    }
                    else if(sprites.happy.getGlobalBounds().contains(translate)){
                        for(Tile& t:tiles){
                            t.reset();
                        }
                        game.loadMines(tiles);
                        game.game_end=0;
                        game.pause=false;
                        game.leadOn=false;
                        game.minesLeft=game.mines;
                        leadOpened=0;
                    }
                    else if(sprites.leaderboard.getGlobalBounds().contains(translate)){
                        game.pause=1;
                        goto leader;
                    }
                }
                else if(event.mouseButton.button==sf::Mouse::Right){

                    game.flagTile(mouse_tile_x, mouse_tile_y, tiles, gamer);
                }

            }
            else if(game.leadOn){
                goto leader;
            }
            if(game.game_end==1){
                if(leadOpened==0){
                    leadOpened=1;
                    game.leadOn=1;
                }

            }



        }


    }


leader:
    leaderboard.create(VideoMode(wel.cols*16, wel.rows*16+50), "Leaderboard", Style::Close);
    while (leaderboard.isOpen()) {
        Event x;
        while (leaderboard.pollEvent(x)) {

            if (x.type == Event::Closed) {
                leaderboard.close();
                game.pause=0;
                game.leadOn=0;
                goto game;
            }
        }


        leaderboard.clear(Color::Blue);
        Text title("LEADERBOARD", font);
        title.setCharacterSize(20);
        title.setStyle(Text::Bold);
        title.setStyle(Text::Underlined);
        setText(title, leaderboard.getSize().x/2, leaderboard.getSize().y/2 - 120);
        Text line1("1.\t07:01\tAlex\n\n2.\t09:04\tBruce\n\n3.\t10:03\tCarl\n\n4.\t11:24\tDan\n\n5.\t12:15\tEve", font);
        line1.setCharacterSize(18);

        line1.setStyle(Text::Bold);
        setText(line1, leaderboard.getSize().x/2, leaderboard.getSize().y/2 +5);


        leaderboard.draw(title);
        leaderboard.draw(line1);

        leaderboard.display();
    }


    return 0;
};