#include <SFML/Graphics.hpp>
#pragma once
struct SpriteCache {
    sf::Sprite hidden;
    sf::Sprite revealed;
    sf::Sprite flag;
    sf::Sprite mine;
    sf::Sprite play;
    sf::Sprite pause;
    sf::Sprite debugSprite;
    sf::Sprite leaderboard;
    sf::Sprite happy;
    sf::Sprite win;
    sf::Sprite lose;
    sf::Sprite counter1;
    sf::Sprite counter2;
    sf::Sprite counter3;
    sf::Sprite negative;

    SpriteCache(const sf::Texture& hiddenTex, const sf::Texture& revealedTex, const sf::Texture& flagTex, const sf::Texture& mineTex,
                const sf::Texture& playTex, const sf::Texture& pauseTex, const sf::Texture& debugSpriteTex, const sf::Texture& leaderboardTex,
                const sf::Texture& happyTex, const sf::Texture& winTex,const sf::Texture& loseTex, const sf::Texture& digitsTex) {
        hidden.setTexture(hiddenTex);
        revealed.setTexture(revealedTex);
        flag.setTexture(flagTex);
        mine.setTexture(mineTex);
        play.setTexture(playTex);
        pause.setTexture(pauseTex);
        debugSprite.setTexture(debugSpriteTex);
        leaderboard.setTexture(leaderboardTex);
        happy.setTexture(happyTex);
        win.setTexture(winTex);
        lose.setTexture(loseTex);
        counter1.setTexture(digitsTex);
        counter2.setTexture(digitsTex);
        counter3.setTexture(digitsTex);
        negative.setTexture(digitsTex);
        negative.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
    }
};