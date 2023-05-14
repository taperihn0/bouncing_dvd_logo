#include <iostream>
#include <random>
#include <chrono>
#include <SFML\Graphics.hpp>
#include <iomanip>

void HandleKeyEvents(sf::RenderWindow &window, const sf::Keyboard::Key &key) {

    switch (key) {
    case sf::Keyboard::Q: {
        window.close();
        break;
    } 
    default: {
        break;
    }
    }
}

std::ostream& operator<<(std::ostream& out, sf::Vector2f& vec) {
    return out << "X: " << std::setw(10) << vec.x << ", Y: " << vec.y;
}

sf::Vector2f GenerateRandomDirection(const float& starting_velocity) {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    static std::uniform_real_distribution<float> distribute(-starting_velocity, starting_velocity);
    
    static auto time = std::chrono::high_resolution_clock::now();
    static auto seed = static_cast<unsigned int>(time.time_since_epoch().count());
    generator.seed(seed);
    
    // just Pythagoras theorem
    auto vec_x = distribute(generator),
         vec_y = hypot(vec_x, starting_velocity);

    return { vec_x, vec_y };
}

int main() {
    constexpr auto WIDTH = 800u, HEIGHT = 600u;
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Bouncing DVD Logo");
    sf::Image window_icon;
    sf::Texture dvd_texture;

    if (!window_icon.loadFromFile("program\\resource\\dvd_icon.png")
        or !dvd_texture.loadFromFile("program\\resource\\DVD_logo_t.png")) {
        exit(0);
    }

    window.setIcon(window_icon.getSize().x, window_icon.getSize().y, window_icon.getPixelsPtr());
    dvd_texture.setSmooth(true);
    
    constexpr auto starting_velocity = 0.04f;

    sf::Sprite dvd_logo_sprite(dvd_texture);
    dvd_logo_sprite.scale(0.07f, 0.07f);
    dvd_logo_sprite.setPosition(WIDTH / 2, HEIGHT / 2);

    sf::Vector2f move_v(GenerateRandomDirection(starting_velocity));
    std::cout << move_v << '\n';

    // main simulation loop
    sf::Event processed_event;
    while (window.isOpen()) {
        while (window.pollEvent(processed_event)) {
            switch (processed_event.type) {
            case sf::Event::Closed: {
                window.close();
                break;
            }
            case sf::Event::KeyPressed: {
                HandleKeyEvents(window, processed_event.key.code);
            }                      
            default: {
                break;
            }
            }
        }

        const auto bounds = dvd_logo_sprite.getGlobalBounds();
        if (bounds.left < 0 or bounds.left + bounds.width > WIDTH) {
            move_v.x = -move_v.x;
            std::cout << move_v << '\n';
        }
        else if (bounds.top < 0 or bounds.top + bounds.height > HEIGHT) {
            move_v.y = -move_v.y;
            std::cout << move_v << '\n';
        }

        dvd_logo_sprite.move(move_v);

        window.clear();
        window.draw(dvd_logo_sprite);
        window.display();
    }
}
