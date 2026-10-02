#include "ui/include/text_area.hpp"
#include <ui/include/label.hpp>
#include "SFML/Graphics.hpp"
#include "utils/include/logger.hpp"
#include <events/include/sfml_event_manager.hpp>

#include <optional>
#include <iostream>

int main() {


    using namespace battleships;
    using namespace utils;
    using namespace ui;

    LOG_LVLS_ENABLED(Log_lvl::WARN | Log_lvl::ERR | Log_lvl::INFO | Log_lvl::DEBUG);

    auto window = sf::RenderWindow(
            sf::VideoMode({800u, 800u}),
            "Battleships",
            sf::Style::Default,
            sf::State::Windowed,
            sf::ContextSettings{.antiAliasingLevel = 4u}
        );

    // ------------------------------------------------------------------------
    // TEST AREA 1: Standard Order (Pos -> Size -> Scrollbar Width -> Colors)
    // ------------------------------------------------------------------------
    Text_area area1(window, "Area_Standard_Order");
    area1.set_pos({200.0f, 100.0f});
    area1.set_size({500.0f, 125.0f});
    area1.set_scroll_bar_width(20.0f);
    area1.set_scrollable(true);
    area1.set_rect_color(sf::Color(180, 50, 50));        // Red Box
    area1.set_scroll_bar_bg_color(sf::Color(50, 50, 50)); // Dark Gray BG
    area1.set_scroll_bar_color(sf::Color(50, 200, 50));   // Green Bar

    auto& events_manager = events::SFML_event_manager::instance();

    events_manager.register_callback(events::SFML_event_type::KEYPRESS_ESCAPE,
        [&window](events::SFML_event_data const&) {
            window.close();
        },0
    );

    while (window.isOpen()) {


        events_manager.process_events(window);

        window.clear(sf::Color::Black);

        area1.draw();

        window.display();
    }

    return 0;
}
