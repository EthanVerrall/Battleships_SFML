// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/app.hpp"
#include "utils/include/logger.hpp"

#include <cstdint>

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
    {
        settings::Settings_manager::instance().load();
        LOG(utils::Log_lvl::TRACE) << "Render window created.";
    }

    //--------------------------
    // Class specific functions
    //--------------------------
    void App::create_render_window() {

        auto& settings_manager = settings::Settings_manager::instance();
        sf::State startup_state (settings_manager.get<sf::State>("window_state"));

        sf::ContextSettings startup_context_settings
            { .antiAliasingLevel = settings_manager.get<std::uint8_t>("anti_aliasing")};

        _window = sf::RenderWindow(
            sf::VideoMode({1920u, 1080u}),
            "Battleships",
            sf::Style::Default,
            startup_state,
            startup_context_settings
        );
    }

    void App::run_program() {

        LOG(utils::Log_lvl::INFO) << "Program started.";

        auto& sfml_events_manager = events::SFML_event_manager::instance();
        auto& resource_manager = resources::Resource_manager::instance();
        auto& menu_manager = menus::Menu_manager::instance(_window);

        while (_window.isOpen()){

            sfml_events_manager.process_events(_window);

            _window.clear(sf::Color::Black);

            menu_manager.draw();

            _window.display();

            resource_manager.flush_unused();
        }

        LOG(utils::Log_lvl::INFO) << "Program terminated.";
    }

}
