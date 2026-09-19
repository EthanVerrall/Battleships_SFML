#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menu.hpp"
#include "events/include/event_listener.hpp"
#include "events/include/event_data.hpp"

#include "SFML/Graphics.hpp"

#include <optional>
#include <memory>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::menu {

// ============================================================================
// Class Menu_manager
// ----------------------------------------------------------------------------

class Menu_manager final
    : public battleships::event::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Menu_manager(Menu_manager const&) = delete;
    Menu_manager& operator=(Menu_manager const&) = delete;
    Menu_manager(Menu_manager&&) = delete;
    Menu_manager& operator=(Menu_manager&&) = delete;

private:
    Menu_manager(sf::RenderWindow& window);

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    static Menu_manager& instance(sf::RenderWindow& window);

    void draw();

    void update(float const dt);

    void change_menu(Menu_id const menu_id);

    void deregister_events();

private:

    std::unique_ptr<Menu> _create_menu(Menu_id const menu_id);

    void _register_events();

    void _handle_event__pending_menu_change(battleships::event::Game_event_data const& data);

    //--------------------------
    // Getters
    //--------------------------
public:

    std::optional<Menu_id> get_if_pending_menu_change() const;

    //--------------------------
    // Attributes
    //--------------------------

private:

    sf::RenderWindow& _window;

    std::unique_ptr<Menu> _curr_menu;

    std::optional<Menu_id> _pending_menu_change;
};

}
