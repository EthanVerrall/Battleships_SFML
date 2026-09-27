#include "ui/include/text_area.hpp"
#include "SFML/Graphics.hpp"
#include "utils/include/logger.hpp"

#include <optional>
#include <iostream>

int main() {


    using namespace battleships;
    using namespace utils;
    LOG_LVLS_ENABLED(Log_lvl::WARN | Log_lvl::ERR | Log_lvl::INFO | Log_lvl::DEBUG);

    auto window = sf::RenderWindow(
            sf::VideoMode({1920u, 1080u}),
            "Battleships",
            sf::Style::Default,
            sf::State::Fullscreen,
            sf::ContextSettings{.antiAliasingLevel = 4u}
        );


    ui::Text_area text_area(window, "Text_area");
    text_area.set_size({200.0f, 500.0f});
    text_area.set_pos({50.0f,50.0f});
    text_area.set_rect_color(sf::Color::Blue);

    text_area.set_scroll_bar_color(sf::Color::Green);
    text_area.set_scroll_bar_width(10.0f);
    std::cout << text_area.is_scrollable();
    text_area.set_scrollable(true);
    std::cout << text_area.is_scrollable();

    while (window.isOpen()) {

        while (const auto current_event = window.pollEvent()) {

            if (auto key_release = current_event->getIf<sf::Event::KeyReleased>()) {
                if (key_release->code == sf::Keyboard::Key::Escape) {
                    window.close();
                }
            }
        }

        window.clear(sf::Color::Black);

        text_area.draw();

        window.display();
    }

    return 0;
}