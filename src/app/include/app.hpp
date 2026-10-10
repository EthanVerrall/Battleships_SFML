#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "events/include/game_event_manager.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "menus/include/menu_manager.hpp"
#include "resources/include/resource_manager.hpp"
#include "settings/include/settings_manager.hpp"
#include "ui/include/fps_counter.hpp"

#include "sfml/Graphics.hpp"

#include <memory>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::app {

// ============================================================================
// Class App
// ----------------------------------------------------------------------------

class App {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:
    App();

    ~App() = default;

    App(App const&) = delete;
    App& operator=(App const&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void run_program();

private:

    void create_render_window();

    void _build__fps_counter();

    void _handle_event___settings_saved(events::Game_event_data const& data);

    //--------------------------
    // Attributes
    //--------------------------
private:

    // Owns the window. Destroyed with App, ending the application.
    // Other classes hold references to it.
    sf::RenderWindow _window;

    // Owned by App rather than by a menu so the counter persists across menu
    // changes and stays drawn on top of whichever menu is current.
    std::unique_ptr<ui::Fps_counter> _fps_counter;
};

}
