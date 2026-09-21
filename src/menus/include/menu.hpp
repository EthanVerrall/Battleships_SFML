#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "events/include/game_event_manager.hpp"
#include "events/include/event_type.hpp"
#include "utils/include/logger.hpp"

#include "SFML/Graphics.hpp"

#include <string_view>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::menu {

// ============================================================================
// Enum
// ----------------------------------------------------------------------------

enum class Menu_id {

    MAIN_MENU,

    FIRST_GAME_MENU = MAIN_MENU
}; inline std::string_view to_string(Menu_id const menu_id) {

    switch (menu_id) {

        case Menu_id::MAIN_MENU: return "MAIN_MENU";

        default: return "UNKNOWN";
    }
}

// ============================================================================
// Class Menu
// ----------------------------------------------------------------------------

class Menu  {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    virtual ~Menu() = default;

protected:

    Menu(sf::RenderWindow& window) : _window(window) {}

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    virtual void draw() = 0;

    virtual void update(float const dt) = 0;

protected:

    void _emit_event___pending_menu_change(
        Menu_id const menu_id
        ) {

        battleships::event::Game_event_type const event =
            battleships::event::Game_event_type::PENDING_MENU_CHANGE;

        LOG(battleships::utils::Log_lvl::TRACE) << "Emitting event: " << event;

        auto& game_event_manager = battleships::event::Game_event_manager::instance();

        game_event_manager.emit(
            event,
            battleships::event::Game_event_data{ .payload = menu_id }
            );
    }

    //--------------------------
    // Attributes
    //--------------------------

protected:

    sf::RenderWindow& _window;
};

}
