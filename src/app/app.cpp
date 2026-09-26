// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/app.hpp"
#include "utils/include/logger.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::app {

// ============================================================================
// Class Label
// ----------------------------------------------------------------------------

    //--------------------------
    // Constructor / Destructor
    //--------------------------
    App::App() 
        : _window(create_render_window())
        , _menu_manager(menus::Menu_manager::instance(_window))
        , _sfml_events_manager(events::SFML_event_manager::instance())
        , _resource_manager(resources::Resource_manager::instance())
        , _settings_manager(settings::Settings_manager::instance())
    {
        LOG(utils::Log_lvl::TRACE) << "Render window created.";
    }

    //--------------------------
    // Class specific functions
    //--------------------------
    sf::RenderWindow App::create_render_window() {   
        
        sf::RenderWindow temp_window = sf::RenderWindow(
            sf::VideoMode({1920u, 1080u}),
            "Battleships",
            sf::Style::Default,
            sf::State::Fullscreen,
            sf::ContextSettings{.antiAliasingLevel = 16u}
        );

        return temp_window;
    }

    void App::run_program() {

        LOG(utils::Log_lvl::INFO) << "Program started.";

        _settings_manager.load();

        while (_window.isOpen()){
            
            _sfml_events_manager.process_events(_window);

            _window.clear(sf::Color::Black);

            _menu_manager.draw();

            _window.display();

            _resource_manager.flush_unused();
        }

        LOG(utils::Log_lvl::INFO) << "Program terminated.";
    }

}
