
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;


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
    void update(int& vid) {
        for (int i = 0; i < 9; i++)
            sprite[i].setTextureRect(IntRect(200 * (vid - 1), 0, 200, 200));


    }
};


int main()
{

    RenderWindow window(VideoMode(600, 600), "krestikinoliki");

    Texture s;
    s.loadFromFile("F:/tiktaktok/setkav2.png");
    Sprite fon(s);


    Texture c;
    c.loadFromFile("F:/tiktaktok/krestnol.png");
    Sprite vibor[2];
    for (int i = 0; i < 2; i++) {
        vibor[i].setTexture(c);
        vibor[i].setPosition(50 + 300 * i, 180);
    }
    int vib = 0;
    Stav player(c), bot(c);


    while (window.isOpen())
    {
        Vector2i pos = Mouse::getPosition(window);

        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

            if (event.type == Event::MouseButtonPressed)
                if (event.key.code == Mouse::Left) {
                    if (vib == 0) {
                        for (int i = 0; i < 2; i++)
                            if (vibor[i].getGlobalBounds().contains(pos.x, pos.y))
                                vib = i + 1;
                    }
                    else
                        for (int i = 0; i < 9; i++)
                            if (player.sprite[i].getGlobalBounds().contains(pos.x, pos.y))
                                player.tik[i] = true;
                            
                    int botstav = rand() % 9;
                    bot.tik[botstav] = true;

                }
        }

        for (int i = 0; i < 2; i++)
            if (vibor[i].getGlobalBounds().contains(pos.x, pos.y))
                vibor[i].setTextureRect(IntRect(200 * i, 200, 200, 200));
            else
                vibor[i].setTextureRect(IntRect(200 * i, 0, 200, 200));

        player.update(vib);

        int biv = vib + 1;
        if (biv == 3)
            biv = 1;

        bot.update(biv);


        window.clear(Color::White);
        if (vib == 0)
            for (int i = 0; i < 2; i++)
                window.draw(vibor[i]);
        else {
            window.draw(fon);
            for (int i = 0; i < 9; i++) {
                if (player.tik[i])
                    window.draw(player.sprite[i]);

                if (bot.tik[i])
                    window.draw(bot.sprite[i]);
            }
        }
            window.display();
        
    }
    return 0;
}