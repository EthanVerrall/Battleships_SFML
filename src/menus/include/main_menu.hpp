#pragma once 

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/menu.hpp"
#include "ui/include/button.hpp"
#include "ui/include/label.hpp"

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
    // Attributes
    //--------------------------
private:

    std::unique_ptr<ui::Label> _title_lbl;
    std::unique_ptr<ui::Button> _playgame_btn;
    std::unique_ptr<ui::Button> _scoreboard_btn;
    std::unique_ptr<ui::Button> _options_btn;
    std::unique_ptr<ui::Button> _profile_btn;

};

}

