#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

const int windowWidth = 800;
const int windowHeight = 600;
const int boxSize = 20;

class Snake {
public:
    std::vector<sf::RectangleShape> snakeParts;
    sf::Vector2f moveDirection;

    Snake() {
        sf::RectangleShape head(sf::Vector2f(boxSize, boxSize));
        head.setFillColor(sf::Color::Green);
        head.setPosition(windowWidth / 2, windowHeight / 2);
        snakeParts.push_back(head);
        moveDirection = {boxSize, 0}; 
    }

    void move() {
        for (int i = snakeParts.size() - 1; i > 0; --i) {
            snakeParts[i].setPosition(snakeParts[i - 1].getPosition());
        }
        snakeParts[0].move(moveDirection);
    }

    void grow() {
        sf::RectangleShape newPart(sf::Vector2f(boxSize, boxSize));
        newPart.setFillColor(sf::Color::Green);
        newPart.setPosition(snakeParts.back().getPosition());
        snakeParts.push_back(newPart);
    }

    bool hitItself() {
        sf::Vector2f headPos = snakeParts[0].getPosition();
        for (int i = 1; i < snakeParts.size(); ++i) {
            if (snakeParts[i].getPosition() == headPos)
                return true;
        }
        return false;
    }
};

sf::Vector2f createFoodPosition() {
    int x = (rand() % (windowWidth / boxSize)) * boxSize;
    int y = (rand() % (windowHeight / boxSize)) * boxSize;
    return sf::Vector2f(x, y);
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Simple Snake Game");
    Snake snake;

    sf::RectangleShape food(sf::Vector2f(boxSize, boxSize));
    food.setFillColor(sf::Color::Red);
    food.setPosition(createFoodPosition());

    sf::Clock timer;
    float moveDelay = 0.2f;
    float timePassed = 0;

    
    sf::SoundBuffer eatBuffer;
    eatBuffer.loadFromFile("eat.wav"); 
    sf::Sound eatSound;
    eatSound.setBuffer(eatBuffer);

    while (window.isOpen()) {
        float deltaTime = timer.restart().asSeconds();
        timePassed += deltaTime;

        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && snake.moveDirection.y == 0)
            snake.moveDirection = {0, -boxSize};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && snake.moveDirection.y == 0)
            snake.moveDirection = {0, boxSize};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && snake.moveDirection.x == 0)
            snake.moveDirection = {-boxSize, 0};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && snake.moveDirection.x == 0)
            snake.moveDirection = {boxSize, 0};

        if (timePassed >= moveDelay) {
            snake.move();
            timePassed = 0;

            
            if (snake.snakeParts[0].getGlobalBounds().intersects(food.getGlobalBounds())) {
                snake.grow();
                food.setPosition(createFoodPosition());
                eatSound.play();
                if (moveDelay > 0.05f) moveDelay -= 0.01f; 
            }

            
            sf::Vector2f headPos = snake.snakeParts[0].getPosition();
            if (headPos.x < 0 || headPos.x >= windowWidth || headPos.y < 0 || headPos.y >= windowHeight || snake.hitItself()) {
                window.close(); 
            }
        }
        window.clear();
        window.draw(food);
        for (auto& part : snake.snakeParts)
            window.draw(part);
        window.display();
    }

    return 0;
}