#include <string>
#include <time.h>
#include <SFML/Graphics.hpp>

struct Coords {
    int x;
    int y;
};

const int SIZE = 20;
const int NUM_ROWS = 20;
const int NUM_COLUMNS = 20;

void handleMovement(std::string& direction) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && direction != "backward") {
        direction = "forward";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && direction != "forward") {
        direction = "backward";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && direction != "right") {
        direction = "left";
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && direction != "left") {
        direction = "right";
    }
}

void spawnFruit(Coords& fruit) {
    fruit.x = rand() % 20;
    fruit.y = rand() % 20;
}

void addBlockToSnake(std::string direction, std::vector<Coords>& positions) {
    Coords newPos = positions.back();
    if (direction == "backward") {
        newPos.y = newPos.y + 1;
        if (newPos.y > 19) {
            newPos.y = 0;
        }
    }
    else if (direction == "forward") {
        newPos.y = newPos.y - 1;
        if (newPos.y < 0) {
            newPos.y = 19;
        }
    }
    else if (direction == "left") {
        newPos.x = newPos.x - 1;
        if (newPos.x < 0) {
            newPos.x = 19;
        }
    }
    else {
        newPos.x = newPos.x + 1;
        if (newPos.x > 19) {
            newPos.x = 0;
        }
    }
    positions.push_back(newPos);
}

void moveSnake(std::string direction, std::vector<Coords>& positions) {
    addBlockToSnake(direction, positions);
    positions.erase(positions.begin());
}

bool isSnakeOnFruit(Coords fruit, std::vector<Coords> positions) {
    bool status = false;
    for_each(positions.begin(), positions.end(), [fruit, &status](Coords i) {
        if (i.x == fruit.x && i.y == fruit.y) {
            status = true;
        }
    });
    return status;
}

bool checkCollision(std::vector<Coords> positions) {
    bool status = false;
    const Coords lastPosition = positions.back();
    for_each(positions.begin(), positions.end() - 1, [lastPosition, &status](Coords i) {
        if (i.x == lastPosition.x && i.y == lastPosition.y) {
            status = true;
        }
    });
    return status;
}

void createAlert(int score) {
    sf::RenderWindow alert(sf::VideoMode(200, 100), "End");
    sf::Event event;
    sf::Text text;
    sf::Font font;
    alert.clear(sf::Color::White);
    font.loadFromFile("JetBrainsMono.ttf");
    text.setFont(font);
    text.setString("You lose! \nYour score: " + std::to_string(score));
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    alert.draw(text);
    alert.display();
    while (alert.isOpen()) {
        sf::Event event;
        while (alert.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                alert.close();
        }
    }
}

void drawBlock(sf::RenderWindow &window, Coords position, sf::Color color) {
    sf::RectangleShape shape(sf::Vector2f(SIZE, SIZE));
    shape.setPosition(position.x, position.y);
    shape.setOutlineThickness(1);
    shape.setOutlineColor(sf::Color::Black);
    shape.setFillColor(color);
    window.draw(shape);
}

void drawMap(sf::RenderWindow &window) {
    for (int i = 0; i < NUM_ROWS; i++) {
        for (int j = 0; j < NUM_COLUMNS; j++) {
            drawBlock(window, Coords{ i * SIZE, j * SIZE }, sf::Color::Green);
        }
    }
}

void drawSnake(sf::RenderWindow& window, std::vector<Coords> snakePos) {
    for (int i = 0; i < snakePos.size(); i++) {
        drawBlock(window, Coords{ snakePos[i].x * SIZE, snakePos[i].y * SIZE }, sf::Color::Blue);
    }
}

void drawFruit(sf::RenderWindow &window, Coords fruitPos) {
    drawBlock(window, Coords{ fruitPos.x * SIZE, fruitPos.y * SIZE }, sf::Color::Red);
}

int main()
{
    srand(time(NULL));
    sf::RenderWindow window(sf::VideoMode(SIZE * NUM_COLUMNS, SIZE * NUM_ROWS), "Snake");
    sf::Clock clock;

    std::vector<Coords> snakePos = { { 10, 9 }, { 10, 10 }, { 10, 11 } };
    std::string direction = "backward";
    int score = 0;
    Coords fruitPos;

    spawnFruit(fruitPos);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        handleMovement(direction);
        
        if (clock.getElapsedTime().asSeconds() >= 0.1) {
            clock.restart();
            
            if (isSnakeOnFruit(fruitPos, snakePos)) {
                score++;
                spawnFruit(fruitPos);
                addBlockToSnake(direction, snakePos);
            }
            
            moveSnake(direction, snakePos);
            
            if (checkCollision(snakePos)) {
                createAlert(score);
                snakePos = { { 10, 9 }, { 10, 10 }, { 10, 11 } };
                direction = "backward";
                score = 0;
                spawnFruit(fruitPos);
            }
            
            window.clear(sf::Color::White);
            drawMap(window);
            drawSnake(window, snakePos);
            drawFruit(window, fruitPos);
            window.display();
        }
    }

    return 0;
}