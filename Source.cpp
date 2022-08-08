#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

using namespace std;

struct Circle{
    sf::CircleShape circle;
    sf::Vector2f velocity;
    sf::Vector2f position;
};

void updatePosition(struct Circle& c, float dt);
void wallCollision(struct Circle& c, sf::Vector2u window_size);
void circleCollision(struct Circle& c1, struct Circle& c2, float dt);
void createCircles(struct Circle* circles, int n);

sf::RenderWindow window(sf::VideoMode(700, 700), "Circle Collision" , sf::Style::Titlebar | sf::Style::Close);

int main()
{
    window.setFramerateLimit(60);
    float dt = (1.0f / 60.0f);

    struct Circle circles[15];
    createCircles(circles, 15);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Black);

        int size = sizeof(circles) / sizeof(circles[0]);

        for (int i = 0; i < size; i++) {
            updatePosition(circles[i], dt);
        }

        for (int i = 0; i < size; i++) {
            wallCollision(circles[i], window.getSize());
        }

        for (int i = 0; i < size-1; i++) {
            for (int j = i+1; j < size; j++) {
                circleCollision(circles[i], circles[j], dt);
            }
        }

        for (int i = 0; i < size; i++) {
            window.draw(circles[i].circle);
        }
            
        window.display();
    }

    return 0;
}

void createCircles(struct Circle *circles, int n) {

    srand(time(0));

    for (int i = 0; i < n; i++) { // set radius
        float radius = rand() % (50 - 15) + 15;
        circles[i].circle.setRadius(radius);
    }

    for (int i = 0; i < n; i++) { // set colour
        float r = rand() % 256;
        float g = rand() % 256;
        float b = rand() % 256;
        circles[i].circle.setFillColor(sf::Color(r, g, b));
    }

    int xx = window.getSize().x;
    int yy = window.getSize().y;

    for (int i = 0; i < n; i++) { // set position

        start:

        float x = rand() % int(xx - 2 * circles[i].circle.getRadius());
        float y = rand() % int(yy - 2 * circles[i].circle.getRadius());

        float c1_x = x + circles[i].circle.getRadius();
        float c1_y = y + circles[i].circle.getRadius();

        for (int c = 0; c < n; c++) {

            if (c == i)
                continue;

            float c2_x = circles[c].position.x + circles[c].circle.getRadius();
            float c2_y = circles[c].position.y + circles[c].circle.getRadius();

            if (sqrt(pow(c2_x - c1_x, 2) + pow(c2_y - c1_y, 2)) <= (circles[c].circle.getRadius() + circles[i].circle.getRadius()))
                goto start;

        }

        circles[i].position.x = x;
        circles[i].position.y = y;
    }

    for (int i = 0; i < n; i++) { // set velocity
        float vx = rand() % 200;
        float vy = rand() % 200;
 
        circles[i].velocity.x = vx;
        circles[i].velocity.y = vy;
    }

}

void updatePosition(struct Circle &c, float dt) {

    c.position.x = c.position.x + c.velocity.x * dt;
    c.position.y = c.position.y + c.velocity.y * dt;
    c.circle.setPosition(c.position);

}

void circleCollision(struct Circle& c1, struct Circle& c2, float dt) {

    float c1_x = c1.position.x + c1.circle.getRadius();
    float c1_y = c1.position.y + c1.circle.getRadius();
    float c2_x = c2.position.x + c2.circle.getRadius();
    float c2_y = c2.position.y + c2.circle.getRadius();

    if ( sqrt(pow(c2_x - c1_x, 2) + pow(c2_y - c1_y, 2)) <= (c1.circle.getRadius() + c2.circle.getRadius()) ) {
        // cout << "Circle Collision" << endl;

        float m1 = 3.141592654 * pow(c1.circle.getRadius(), 2);
        float m2 = 3.141592654 * pow(c2.circle.getRadius(), 2);

        float c1_x = c1.position.x + c1.circle.getRadius();
        float c1_y = c1.position.y + c1.circle.getRadius();
        float c2_x = c2.position.x + c2.circle.getRadius();
        float c2_y = c2.position.y + c2.circle.getRadius();

        float c1_min_c2_x = c1_x - c2_x;
        float c1_min_c2_y = c1_y - c2_y;
        float v1_min_v2_x = c1.velocity.x - c2.velocity.x;
        float v1_min_v2_y = c1.velocity.y - c2.velocity.y;

        float c1_velocity_x = c1.velocity.x - ((2 * m2) / (m1 + m2)) * ((v1_min_v2_x * c1_min_c2_x + v1_min_v2_y * c1_min_c2_y) / (pow(c1_min_c2_x, 2) + pow(c1_min_c2_y, 2))) * c1_min_c2_x;
        float c1_velocity_y = c1.velocity.y - ((2 * m2) / (m1 + m2)) * ((v1_min_v2_x * c1_min_c2_x + v1_min_v2_y * c1_min_c2_y) / (pow(c1_min_c2_x, 2) + pow(c1_min_c2_y, 2))) * c1_min_c2_y;

        float c2_min_c1_x = c2_x - c1_x;
        float c2_min_c1_y = c2_y - c1_y;
        float v2_min_v1_x = c2.velocity.x - c1.velocity.x;
        float v2_min_v1_y = c2.velocity.y - c1.velocity.y;

        float c2_velocity_x = c2.velocity.x - ((2 * m1) / (m1 + m2)) * ((v2_min_v1_x * c2_min_c1_x + v2_min_v1_y * c2_min_c1_y) / (pow(c2_min_c1_x, 2) + pow(c2_min_c1_y, 2))) * c2_min_c1_x;
        float c2_velocity_y = c2.velocity.y - ((2 * m1) / (m1 + m2)) * ((v2_min_v1_x * c2_min_c1_x + v2_min_v1_y * c2_min_c1_y) / (pow(c2_min_c1_x, 2) + pow(c2_min_c1_y, 2))) * c2_min_c1_y;
 
        c1.velocity.x = c1_velocity_x;
        c1.velocity.y = c1_velocity_y;
        c2.velocity.x = c2_velocity_x;
        c2.velocity.y = c2_velocity_y;

    }

}

void wallCollision(struct Circle& c, sf::Vector2u window_size) {

    if (c.position.x <= 0) { // left wall
        //std::cout << "Left" << std::endl;

        if (c.position.x < 0)
            c.position.x = 0;

        c.velocity.x = c.velocity.x * (-1);
    }
    else if (c.position.x + 2 * c.circle.getRadius() >= window_size.x) { // right wall
        //std::cout << "Right" << std::endl;

        if (c.position.x + 2 * c.circle.getRadius() > window_size.x)
            c.position.x = window_size.x - 2 * c.circle.getRadius();

        c.velocity.x = c.velocity.x * (-1);
    }
    else if (c.position.y <= 0) { // top wall
        //std::cout << "Top" << std::endl;

        if (c.position.y < 0)
            c.position.y = 0;

        c.velocity.y = c.velocity.y * (-1);
    }
    else if (c.position.y + 2 * c.circle.getRadius() >= window_size.y) {  // bottom wall
        //std::cout << "Bottom" << std::endl;

        if (c.position.y + 2 * c.circle.getRadius() > window_size.y)
            c.position.y = window_size.y - 2 * c.circle.getRadius();

        c.velocity.y = c.velocity.y * (-1);
    }

}