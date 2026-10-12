#include <iostream>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <vector>
 

using namespace sf;
using namespace std;

struct Ball {
    Vector2f pos;
    Vector2f vel;
    float radius;
    float mass;
    CircleShape shape;

    Ball(float x, float y, float r, Color color) {
        pos = Vector2f(x, y);
        vel = Vector2f(static_cast<float>((rand() % 200) - 100), static_cast<float>((rand() % 200) - 100));
        radius = r;
        mass = r*r;
        shape.setRadius(radius);
        shape.setOrigin(radius, radius); // Центруємо точку прив'язки
        shape.setPosition(pos);
        shape.setFillColor(color);
    }
    void update(float dt, float windowWidth, float windowHeight) {

        pos += vel * dt;

        if (pos.x - radius < 0) {
            pos.x = radius;
            vel.x = -vel.x;
        }
        if (pos.x + radius > windowWidth) {
            pos.x = windowWidth - radius;
            vel.x = -vel.x;
        }
        if (pos.y - radius < 0) {
            pos.y = radius;
            vel.y = -vel.y;
        }
        if (pos.y + radius > windowHeight) {
            pos.y = windowHeight - radius;
            vel.y = -vel.y;
        }

        shape.setPosition(pos);
    }
};
void checkCollision(Ball& b1, Ball& b2) {
    Vector2f delta = b2.pos - b1.pos;
    float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y);
    float minDist = b1.radius + b2.radius;

    // Якщо відстань менша за суму радіусів — вони зіткнулися!
    if (distance < minDist) {
        // 1. Виправляємо «злипання» (розсуваємо кульки назовні)
        float overlap = minDist - distance;
        Vector2f normal = delta / distance;

        // Розсовуємо їх пропорційно масам (або навпіл)
        b1.pos -= normal * (overlap * 0.5f);
        b2.pos += normal * (overlap * 0.5f);

        // 2. Зміна швидкостей (пружний удар)
        Vector2f relVel = b2.vel - b1.vel;
        float velAlongNormal = relVel.x * normal.x + relVel.y * normal.y;

        // Якщо вони рухаються назустріч одна одній
        if (velAlongNormal < 0) {
            float restitution = 1.0f; // Коефіцієнт пружності (1 = абсолютно пружний удар)
            float impulseScalar = -(1 + restitution) * velAlongNormal;
            impulseScalar /= (1 / b1.mass + 1 / b2.mass);

            Vector2f impulse = normal * impulseScalar;
            b1.vel -= impulse / b1.mass;
            b2.vel += impulse / b2.mass;
        }
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    RenderWindow window(VideoMode(800, 600), "Brownian Motion");
    window.setFramerateLimit(60);
    vector<Ball> balls;
    cout << "Введіть кількість кульок";
    int i;
    cin >> i;
    for (int j = 0; j < i; ++j) {
        float startX = static_cast<float>(rand() % 700);
        float startY = static_cast<float>(rand() % 500);
        float radius = static_cast<float>(rand() % 16 + 10);
        balls.emplace_back(startX, startY, radius, Color::Green);
    }
    Clock clock;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        float dt = clock.restart().asSeconds();

        for (auto& ball : balls) {
            ball.update(dt, 800.f, 600.f);
        }
        for (size_t i = 0; i < balls.size(); ++i) {
            for (size_t j = i + 1; j < balls.size(); ++j) {
                checkCollision(balls[i], balls[j]);
            }
        }

        window.clear(Color::Black);

        for (const auto& ball : balls) {
            window.draw(ball.shape);
        }

        window.display();
    }

    return 0;

}
