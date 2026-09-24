#pragma once 

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/menu.hpp"
#include "ui/include/button.hpp"
#include "ui/include/label.hpp"
#include "resources/include/spritesheet.hpp"

#include "sfml/Graphics.hpp"

#include <memory>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::menus {

// ============================================================================
// Class Main_menu
// ----------------------------------------------------------------------------

class Main_menu : public Menu {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Main_menu(sf::RenderWindow& window);

    ~Main_menu() = default;

    Main_menu(Main_menu const&) = delete;
    Main_menu& operator=(Main_menu const&) = delete;
    Main_menu(Main_menu&&) = delete;
    Main_menu& operator=(Main_menu&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;
    void update(float const dt) override;

    //--------------------------
    // Class builder functions
    //--------------------------
private:

    void build_title_lbl();
    void build_play_game_btn();
    void build_scoreboard_btn();
    void build_options_btn();
    void build_profile_btn();
    void build_exit_btn();
    void build_background_image();

    //--------------------------
    // Getters
    //--------------------------
private:   
    sf::Vector2f get_window_mp() const;

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<ui::Label> _title_lbl;
    std::unique_ptr<ui::Button> _play_game_btn;
    std::unique_ptr<ui::Button> _scoreboard_btn;
    std::unique_ptr<ui::Button> _options_btn;
    std::unique_ptr<ui::Button> _profile_btn;
    std::unique_ptr<ui::Button> _exit_btn;
    std::shared_ptr<resources::Static_spritesheet> _static_spritesheet;
    std::unique_ptr<sf::Sprite> _background_img;
};      

}

