#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>
#include <stdio.h>
#include <Windows.h>
#include <exception>
using namespace std;
#include "EventHandler.h"
using namespace sf;

int input_figures_count() {
    int n = -1;
    while (n <= 1) {
        cout << "Enter max random figures count (more than 1): ";
        cin >> n;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return n;
}

int main()
{
    int n = input_figures_count();
    srand((unsigned)time(NULL));

    ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::Image icon;
    icon.loadFromFile("icon.png");
    sf::Texture f1_texture;
    f1_texture.loadFromFile("f1.png");
    sf::Sprite f1;
    f1.setTexture(f1_texture, true);
    sf::RenderWindow window(VideoMode(1000, 800),
        "Agar.io",
        sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());


    sf::Text label;
    sf::Clock time;
    sf::Font font;
    sf::Text status;
    EventHandler event_handler(label, status, font, n);


    float starting = time.getElapsedTime().asSeconds();

    sf::Thread thread([&window, &event_handler, &time, &starting]() {
        while (window.isOpen())
        {
            float ending = time.getElapsedTime()
                .asSeconds();

            float dt = (ending - starting) / 2;
            starting = ending;

            // draw...
            __try {
                event_handler.poll_action_event(window, dt);
                event_handler.poll_draw_event(window);
            }
            __except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
                EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH) {
                cout << "Access violation ex\n";
            }
            
            // end the current frame
            window.display();
        };
        });

    // Вимикаємо вікно, щоб OpenGL не сварився
    window.setActive(false);

    // Запуск thread
    thread.launch();


    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

    }


    return 0;

}