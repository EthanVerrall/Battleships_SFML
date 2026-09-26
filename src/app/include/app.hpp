#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "events/include/game_event_manager.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "menus/include/menu_manager.hpp"
#include "resources/include/resource_manager.hpp"
#include "settings/include/settings_manager.hpp"

#include "sfml/Graphics.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::app {

// ============================================================================
// Class Label
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

    //--------------------------
    // Attributes
    //--------------------------
private:

    // Owns the window. Destroyed with App, ending the application.
    // Other classes hold references to it.
    sf::RenderWindow _window;
};

}
