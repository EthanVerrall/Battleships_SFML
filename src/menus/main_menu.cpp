// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/main_menu.hpp"
#include "utils/include/logger.hpp"
#include "resources/include/asset_registry.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

using namespace battleships::ui;

namespace battleships::menus {

// ============================================================================
// Class Label
// ----------------------------------------------------------------------------   

    //--------------------------
    // Constructor / Destructor
    //-------------------------- 
    Main_menu::Main_menu(sf::RenderWindow& window) : Menu::Menu(window) 
    {   
        build_title_lbl();
        build_playgame_btn();
        build_scoreboard_btn();
        build_options_btn();
        build_profile_btn(); 
    }

    //--------------------------
    // Class builder functions
    //--------------------------
    sf::Vector2f Main_menu::get_window_mp() const {

        const sf::Vector2u window_size = _window.getSize();
        const sf::Vector2f window_mp {window_size.x / 2.0f, window_size.y / 2.0f };
        return window_mp;
    }

    void Main_menu::build_title_lbl() {
        
        _title_lbl = std::make_unique<Label> (_window, "Title_label", "pixel_bold","Battleships",100u);
        _title_lbl->set_color(sf::Color::Red);
        _title_lbl->set_border({.color = sf::Color::Yellow, .size = 6u });

        const float label_mpx = _title_lbl->get_size().x / 2.0f;
        const sf::Vector2f window_mp = get_window_mp(); 
        const sf::Vector2f label_pos {window_mp.x - label_mpx, 50.0f};
        _title_lbl->set_pos(label_pos);
    }

    void Main_menu::build_playgame_btn() {

        
    }

    void Main_menu::build_scoreboard_btn() {

        
    }

    void Main_menu::build_options_btn() {

        
    }

    void Main_menu::build_profile_btn() {

        
    }

    void Main_menu::draw() {

        if (!_title_lbl) {
            return;
        }
        else {
            _title_lbl->draw();
        }
    }

    void Main_menu::update(float const dt) { static_cast<void> (dt); /*Not implemented*/ };
}