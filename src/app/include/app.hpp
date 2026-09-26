#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

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

    //--------------------------
    // Attributes
    //--------------------------
private:

    // Owns the window. Destroyed with App, ending the application.
    // Other classes hold references to it.
    sf::RenderWindow _window;
};

}
