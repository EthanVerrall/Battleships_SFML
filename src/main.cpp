//UI Elements
#include "ui/include/button.hpp"
#include "ui/include/label.hpp"

//Managers
#include "events/include/sfml_event_manager.hpp"
#include "resources/include/resource_manager.hpp"
#include "menus/include/menu_manager.hpp"
#include "menus/include/main_menu.hpp"

//Logging And Tests
#include "utils/include/logger.hpp"

//SFML Dependencies 
#include <SFML/Graphics.hpp>

//Project namespace
using namespace battleships;

int main() {

    // create the window
    sf::RenderWindow window(
        sf::VideoMode(
                    {1920, 1080}), 
                    "Battleships",
                    sf::Style::Default,
                    sf::State::Fullscreen,
                    sf::ContextSettings{.antiAliasingLevel = 16});

    

    auto& event_manager = events::SFML_event_manager::instance();
    auto& resource_manager = resources::Resource_manager::instance();
    auto& menu_manager = menus::Menu_manager::instance(window);

    event_manager.register_callback(
        events::SFML_event_type::KEYPRESS_ESCAPE,
        [&window](events::SFML_event_data const&) { window.close(); },
        0u
        );
    

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // dispatch SFML events
        event_manager.process_events(window);

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw the current active menu
        menu_manager.draw();

        // end the current frame
        window.display();

        // unload resources
        resource_manager.flush_unused();
    }
}
