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
// Class Options_menu
// ----------------------------------------------------------------------------

class Options_menu : public Menu {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Options_menu(sf::RenderWindow& window);

    ~Options_menu() = default;

    Options_menu(Options_menu const&) = delete;
    Options_menu& operator=(Options_menu const&) = delete;
    Options_menu(Options_menu&&) = delete;
    Options_menu& operator=(Options_menu&&) = delete;

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

    void _build__background_image();

    //--------------------------
    // Getters
    //--------------------------

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::shared_ptr<resources::Static_spritesheet> _background_static_spritesheet;

    std::unique_ptr<sf::Sprite> _background_img;
};

}
