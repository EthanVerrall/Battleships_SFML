// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/main_menu.hpp"
#include "utils/include/logger.hpp"
#include "resources/include/asset_registry.hpp"
#include "utils/include/sizes.hpp"
#include "utils/include/colors.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

using namespace battleships::ui;
using namespace battleships::utils;
using namespace battleships::utils;

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
        build_play_game_btn();
        build_scoreboard_btn();
        build_options_btn();
        build_profile_btn(); 
    }

    //--------------------------
    // Class builder functions
    //--------------------------

    void Main_menu::build_title_lbl() {
        
        _title_lbl = std::make_unique<Label> (_window, "Title_label", "pixel_bold","Battleships",sizes::HEADING_TEXT);

        if (!_title_lbl) { 
            LOG(utils::Log_lvl::WARN) << "Title_label is nullptr on main menu. Failed to build.";
        }
        else {

            _title_lbl->set_text_color(colors::HEADING_TEXT);
            _title_lbl->set_border({.color = colors::HEADING_BORDER, .size = sizes::HEADING_BORDER });

            const float label_mpx = _title_lbl->get_size().x / 2.0f;
            const sf::Vector2f window_mp = get_window_mp(); 
            const sf::Vector2f label_pos {window_mp.x - label_mpx, 50.0f};
            _title_lbl->set_pos(label_pos);
        }
    }

    void Main_menu::build_play_game_btn() {

        _play_game_btn = std::make_unique<Button> (_window, "Play_game_button");

        if (!_play_game_btn) {
            LOG(utils::Log_lvl::WARN) << "Play_game_button is nullptr on main menu. Failed to build.";
        }   
        else {

            !!!
            _play_game_btn->set_text_color(sf::Color::)

        }

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


    //--------------------------
    // Getters
    //--------------------------
    sf::Vector2f Main_menu::get_window_mp() const {

        const sf::Vector2u window_size = _window.getSize();
        const sf::Vector2f window_mp {window_size.x / 2.0f, window_size.y / 2.0f };
        return window_mp;
    }

}