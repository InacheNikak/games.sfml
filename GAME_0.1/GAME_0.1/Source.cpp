#include <SFML/Graphics.hpp>
#include <optional>
#include <random>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <string>

int main() {
    //слова для убийства врага
    std::vector<std::string> words = {
    "hello","dog","right","finish","cat", "suka","ramons","klava"
    };
    
    //подключение шрифта и проверка
    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/CHILLER.TTF"))
    {
        std::cout << "error";
        for (int i = 0; i < 1000; i++) {
            std::cout << "------";
        }
    }

    //слово для победы над врагом
    std::string enemyDam = "privet";
    std::string wordSpace = "";
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Дельта-время");
    window.setFramerateLimit(60);//ограничение фпс
    
    //player
    sf::RectangleShape player({50.f, 50.f});
    player.setFillColor(sf::Color::Blue);
    player.setPosition({400-25, 300-25});
    float speed = 400.f;  // 400 пикселей в секунду

    //enemy
    sf::CircleShape enemy(20.f);
    enemy.setFillColor(sf::Color(255, 59, 111));
    float enemySpeed = 75.f;

    //пули
    sf::CircleShape bullet(5.f);
    bullet.setFillColor(sf::Color(255, 69, 0));
    bullet.setPosition({ -100.f,-100.f });
    float bulletSpeed = 700.f;
    bool fire = false;

    //размеры игрока и врага
    sf::Vector2f playerSize = player.getSize();
    float enemySize = 2 * enemy.getRadius();

    //статистика
    int health = 100;
    int stat = 0;
    int kombo = 0;

    //случайное слово в начале
    int wordIndex = rand() % words.size();

    //текстовые объекты
                //счет
                sf::Text statText(font, "score: " + std::to_string(stat), 40);
                statText.setFillColor(sf::Color::Black);
                statText.setPosition({ 10.f,40.f });
                ///hp
                sf::Text healhtText(font, "health " + std::to_string(health), 40);
                healhtText.setFillColor(sf::Color::Black);
                healhtText.setPosition({ 10.f,80.f });
                //имя врага
                sf::Text enemyDamText(font, "vrag: " + enemyDam, 50);
                enemyDamText.setFillColor(sf::Color::Black);
                enemyDamText.setPosition({ 400.f, 40.f });
                //kombo
                sf::Text komboText(font, "X" + std::to_string(kombo), 80);
                komboText.setFillColor(sf::Color::Black);
                komboText.setPosition({ 600.f,40.f });

    sf::Clock clock;
    
    while (window.isOpen()) {
        //индекс случайного слова
        //int wordIndex = rand() % words.size();
        enemyDam = words[wordIndex];

        float dt = clock.restart().asSeconds();//дельта время
        
        //обновление такста
        statText.setString("score: " + std::to_string(stat));
        healhtText.setString("health " + std::to_string(health));
        enemyDamText.setString("vrag: " + enemyDam);
        komboText.setString("X" + std::to_string(kombo));

        //float xrand = std::rand() % (800 - static_cast<int>(enemySize)) + enemy.getRadius();
        //float yrand = std::rand() % (600 - static_cast<int>(enemySize)) + enemy.getRadius();
        //случайная позциция за экраном
        int side = rand() % 4;
        float  x, y;
        switch (side) {
        case 0: // левая граница
            x = -enemySize;
            y = rand() % 600;
            break;
        case 1: // правая граница
            x = 800 + enemySize;
            y = rand() % 600;
            break;
        case 2: // верхняя
            x = rand() % 800;
            y = -enemySize;
            break;
        case 3: // нижняя
            x = rand() % 800;
            y = 600 + enemySize;
            break;
        }

        // События
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            /*
            //пробел
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Space) {
                    enemy.setPosition({ xrand, yrand });
                    stat++;
                    system("CLS");
                    std::cout << "счетчик^" << stat;
                    std::cout << "\nхп:" << health;
                }
            }
            */
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>()) {
                char c = static_cast<char>(textEntered->unicode);

                // Строчные буквы
                if (std::isalpha(static_cast<unsigned char>(c))) {
                    wordSpace += c;
                    std::cout << "Ввод: " << wordSpace << std::endl;
                }

                // Backspace
                if (textEntered->unicode == 8 && !wordSpace.empty()) {
                    wordSpace.pop_back();
                    std::cout << "Ввод: " << wordSpace << std::endl;
                }

                // Enter - проверка слова
                if (textEntered->unicode == 13) {
                    if (wordSpace == enemyDam) {
                        system("CLS");
                        //stat++;
                        //enemy.setPosition({ xrand, yrand });
                        std::cout << "Счет: " << stat << std::endl<<"HP"<<health<<std::endl<<"правильно!"<<std::endl;
                        fire = true;
                        bullet.setPosition(player.getPosition());
                    }
                    else {
                        std::cout << "Неправильно!"<< std::endl;
                        fire = false;
                    }
                    wordSpace = "";
                    std::cout << "Ввод: " << std::endl;
                }
            }
        }

        
        //возвращение позиции врага и игрока
        sf::Vector2f playerPos = player.getPosition();
        sf::Vector2f enemyPos = enemy.getPosition();

        //вектор к игроку
        float dx = playerPos.x - enemyPos.x;
        float dy = playerPos.y - enemyPos.y;
        
        //дистанция
        float dist = std::sqrt(dx*dx + dy*dy);

        //логика врага
        if (dist > 0.1f) {
            enemy.move({ (dx/dist)*enemySpeed*dt, (dy/dist) * enemySpeed * dt});
        }
        
        /*
        //передвичжение игрка
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
            player.move({ -speed * dt, 0 });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
            player.move({ speed * dt, 0 });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            player.move({ 0 , -speed * dt });
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            player.move({ 0, speed * dt });
        }
        */
       
        //столкновение
        if (enemy.getGlobalBounds().findIntersection(player.getGlobalBounds())) {
            //health = health - delta;
            int newSide = rand() % 4;
            wordIndex = rand() % words.size();
            enemy.setPosition({ x,y });
            health--;
            system("CLS");
            std::cout << "счетчик:" << stat;
            std::cout <<"\nхп:"<<health;
            wordSpace = "";
            std::cout << wordSpace;
            kombo = 0;
        }
        //выстрел попал во врага
        if (enemy.getGlobalBounds().findIntersection(bullet.getGlobalBounds())) {
            //health = health - delta;
            enemy.setPosition({ x,y });
            wordIndex = rand() % words.size();
            stat++;
            system("CLS");
            std::cout << "счетчик:" << stat;
            std::cout << "\nхп:" << health;
            wordSpace = "";
            std::cout << wordSpace;
            fire = false;
            bullet.setPosition({ -100.f ,-100.f });
            kombo++;
        }

        if (fire) {
            bullet.move({ -(dx / dist) * bulletSpeed * dt, -(dy / dist) * bulletSpeed * dt });
        }
        //за границу незя
        if (playerPos.x < 0) {
            player.setPosition({0,playerPos.y});
        }
        if (playerPos.x > 800-playerSize.x) {
            player.setPosition({ 800 - playerSize.x,playerPos.y });
        }

        window.clear(sf::Color(0, 96, 0));
        window.draw(player);
        window.draw(enemy);
        window.draw(statText);
        window.draw(enemyDamText);
        window.draw(healhtText);
        if (fire)window.draw(bullet);
        if (kombo > 1)window.draw(komboText);
        //window.draw(bullet);
        window.display();
        
    }
    
    return 0;
}