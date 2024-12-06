#include <SFML/Graphics.hpp>
#include <time.h>
#include <vector>

using namespace sf;

bool svat[9] = { false }; 
int winner = 0;           
int winCombo[3] = { -1, -1, -1 }; 

class Stav {
public:
    Sprite sprite[9];
    bool tik[9];

    Stav(Texture& image) {
        for (int i = 0; i < 9; i++) {
            sprite[i].setTexture(image);
            tik[i] = false;
        }
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                sprite[i * 3 + j].setPosition(200 * j, 200 * i);
    }
    void update(int vid) {
        for (int i = 0; i < 9; i++)
            sprite[i].setTextureRect(IntRect(200 * (vid - 1), 0, 200, 200));
    }
};

bool checkWin(const bool tik[9], int& winner) {
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, 
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, 
        {0, 4, 8}, {2, 4, 6}             
    };
    for (int i = 0; i < 8; i++) {
        int a = winPatterns[i][0], b = winPatterns[i][1], c = winPatterns[i][2];
        if (tik[a] && tik[b] && tik[c]) {
            winCombo[0] = a; winCombo[1] = b; winCombo[2] = c;
            winner = 1;
            return true;
        }
    }
    return false;
}


int Botstav(const bool player[9], const bool bot[9]) {
    const int winPatterns[8][3] = {
        {0, 1, 2}, {3, 4, 5}, {6, 7, 8},
        {0, 3, 6}, {1, 4, 7}, {2, 5, 8},
        {0, 4, 8}, {2, 4, 6}
    };

    for (int i = 0; i < 8; i++) {
        int a = winPatterns[i][0], b = winPatterns[i][1], c = winPatterns[i][2];
        if (bot[a] + bot[b] + bot[c] == 2) {
            if (!svat[a]) return a;
            if (!svat[b]) return b;
            if (!svat[c]) return c;
        }
    }
    for (int i = 0; i < 8; i++) {
        int a = winPatterns[i][0], b = winPatterns[i][1], c = winPatterns[i][2];
        if (player[a] + player[b] + player[c] == 2) {
            if (!svat[a]) return a;
            if (!svat[b]) return b;
            if (!svat[c]) return c;
        }
    }
    int res = 0;
    while (true) {
        res = rand() % 9;
        if (!svat[res]) return res;
    }
}

int main() {
    srand(time(0));
    RenderWindow window(VideoMode(600, 600), "Tic Tac Toe");

    Texture s;
    s.loadFromFile("your path"); 
    Sprite fon(s);

    Texture c;
    c.loadFromFile("your path"); 
    Texture lineTexture;
    lineTexture.loadFromFile("your path"); 
    Sprite line(lineTexture);

    Sprite vibor[2];
    for (int i = 0; i < 2; i++) {
        vibor[i].setTexture(c);
        vibor[i].setPosition(50 + 300 * i, 180);
    }

    int vib = 0;
    Stav player(c), bot(c);

    while (window.isOpen()) {
        Vector2i pos = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
                if (event.key.code == Mouse::Left) {
                    if (vib == 0) {
                        for (int i = 0; i < 2; i++)
                            if (vibor[i].getGlobalBounds().contains(pos.x, pos.y))
                                vib = i + 1;
                    }
                    else if (winner == 0) {
                        for (int i = 0; i < 9; i++)
                            if (player.sprite[i].getGlobalBounds().contains(pos.x, pos.y) && !svat[i]) {
                                player.tik[i] = true;
                                svat[i] = true;
                                if (checkWin(player.tik, winner)) break;

                                int botstav = Botstav(player.tik, bot.tik);
                                bot.tik[botstav] = true;
                                svat[botstav] = true;
                                if (checkWin(bot.tik, winner)) break;
                            }
                    }
                }
        }

       
        for (int i = 0; i < 2; i++) {
            if (vibor[i].getGlobalBounds().contains(pos.x, pos.y))
                vibor[i].setTextureRect(IntRect(200 * i, 200, 200, 200)); 
            else
                vibor[i].setTextureRect(IntRect(200 * i, 0, 200, 200));   
        }

        player.update(vib);
        bot.update(vib + 1);

        window.clear(Color::White);

        if (vib == 0) {
            for (int i = 0; i < 2; i++)
                window.draw(vibor[i]);
        } else {
            window.draw(fon);
            for (int i = 0; i < 9; i++) {
                if (player.tik[i]) window.draw(player.sprite[i]);
                if (bot.tik[i]) window.draw(bot.sprite[i]);
            }
            if (winner != 0) {
                line.setPosition(200 * (winCombo[0] % 3), 200 * (winCombo[0] / 3));
                window.draw(line);
            }
        }

        window.display();
    }

    return 0;
}
