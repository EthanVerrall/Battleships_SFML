// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/menu_manager.hpp"
#include "events/include/game_event_manager.hpp"

#include "SFML/Graphics.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::menu {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::utils;
using namespace battleships::events;

// ============================================================================
// Class Menu_manager
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Menu_manager::Menu_manager(
    sf::RenderWindow& window
    )
    : _window(window)
    , _curr_menu(nullptr)
    , _pending_menu_change(std::nullopt)
    {

    // Create first menu of the game
    change_menu(Menu_id::FIRST_GAME_MENU);

    _register_events();
}

// ----------------------------------------------------------------------------
Menu_manager& Menu_manager::instance(
    sf::RenderWindow& window
    ) {

    static Menu_manager instance(window);
    return instance;
}

// ----------------------------------------------------------------------------
void Menu_manager::draw() {

    if (_curr_menu == nullptr) {

        LOG(Log_lvl::WARN) << "_curr_menu == nullptr";
    } else {

        _curr_menu->draw();
    }
}

// ----------------------------------------------------------------------------
void Menu_manager::update(
    float const dt
    ) {

    if (_curr_menu == nullptr) {

        LOG(Log_lvl::WARN) << "_curr_menu == nullptr";
    } else {

        _curr_menu->update(dt);
    }
}

// ----------------------------------------------------------------------------
void Menu_manager::change_menu(
    Menu_id const menu_id
    ) {

    LOG(Log_lvl::TRACE) << "Changing menu to " << to_string(menu_id);

    _curr_menu = _create_menu(menu_id);

    _pending_menu_change.reset();
}

// ----------------------------------------------------------------------------
void Menu_manager::deregister_events() {

    auto& game_event_manager = Game_event_manager::instance();

    game_event_manager.deregister_listener(listener_id());
}

// ----------------------------------------------------------------------------
std::unique_ptr<Menu> Menu_manager::_create_menu(
    Menu_id const menu_id
    ) {

    switch (menu_id) {

        // TODO:    case Menu_id::MAIN_MENU: return std::make_unique<Main_menu>();

        default: return nullptr;
    }
}

// ----------------------------------------------------------------------------
void Menu_manager::_register_events() {

    auto& game_event_manager = Game_event_manager::instance();

    game_event_manager.register_callback(
        Game_event_type::PENDING_MENU_CHANGE,
        [this](Game_event_data const& data){ _handle_event__pending_menu_change(data); },
        listener_id()
        );
}

// ----------------------------------------------------------------------------
void Menu_manager::_handle_event__pending_menu_change(
    Game_event_data const& data
    ) {

    _pending_menu_change = std::any_cast<Menu_id>(data.payload);
}

// ----------------------------------------------------------------------------
std::optional<Menu_id> Menu_manager::get_if_pending_menu_change() const {

    return _pending_menu_change;
}

}
