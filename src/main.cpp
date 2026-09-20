#include "ui/include/button.hpp"
#include "ui/include/label.hpp"
#include "ui/include/widget.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"
#include "resources/include/resource_manager.hpp"

#include <SFML/Graphics.hpp>

using namespace battleships;

int main() {

    // create the window
    sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");

    // create a label
    ui::Label label(window, "test_label");
    label.set_text("Test label");
    label.set_char_size(32u);
    label.set_pos({300.0f, 380.0f});
    label.set_border(ui::Widget_border{ .color = sf::Color::Green, .size = 2.0f });
    label.set_color(sf::Color::Red);

    // create a button
    ui::Button button(window, "test_button");
    button.set_text("Click me");
    button.set_text_char_size(32u);
    button.set_pos({300.0f, 280.0f});
    button.set_text_border(ui::Widget_border{ .color = utils::colors::DEFAULT_BORDER, .size = 2.0f });

    button.set_on_left_click([&label]() {
        label.set_text("IM CLICKED!");
        });

    button.set_on_hover([&button]() {
        button.set_text_color(sf::Color::Yellow);
        });

    button.set_on_exit_hover([&button]() {
        button.set_text_color(utils::colors::DEFAULT_TEXT);
        });

    auto& event_manager = event::SFML_event_manager::instance();
    auto& resource_manager = resources::Resource_manager::instance();

    event_manager.register_callback(
        event::SFML_event_type::WINDOW_CLOSED,
        [&window](event::SFML_event_data const&) { window.close(); },
        0u
        );

    event_manager.register_callback(
        event::SFML_event_type::KEYPRESS_ESCAPE,
        [&window](event::SFML_event_data const&) { window.close(); },
        0u
        );

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // dispatch SFML events
        event_manager.process_events(window);

        // clear the window with black color
        window.clear(sf::Color::Black);

        button.draw();

        label.draw();

        // end the current frame
        window.display();

        // unload resources
        resource_manager.flush_unused();
    }
}
