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

namespace battleships::menus {

// ============================================================================
// Enum
// ----------------------------------------------------------------------------

enum class Menu_id {

    MAIN_MENU,
    GAME_MENU,
    OPTIONS_MENU,
    SCORE_BOARD_MENU,
    PROFILE_MENU,

    STARTUP_MENU = MAIN_MENU
}; inline std::string_view to_string(Menu_id const menu_id) {

    switch (menu_id) {

        case Menu_id::MAIN_MENU: return "MAIN_MENU";
        case Menu_id::GAME_MENU: return "GAME_MENU";
        case Menu_id::OPTIONS_MENU: return "OPTIONS_MENU";
        case Menu_id::SCORE_BOARD_MENU: return "SCORE_BOARD_MENU";
        case Menu_id::PROFILE_MENU: return "PROFILE_MENU";

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

        battleships::events::Game_event_type const event =
            battleships::events::Game_event_type::PENDING_MENU_CHANGE;

        LOG(battleships::utils::Log_lvl::TRACE) << "Emitting event: " << event;

        auto& game_event_manager = battleships::events::Game_event_manager::instance();

        game_event_manager.emit(
            event,
            battleships::events::Game_event_data{ .payload = menu_id }
            );
    }

    //--------------------------
    // Attributes
    //--------------------------

protected:

    sf::RenderWindow& _window;
};

}
