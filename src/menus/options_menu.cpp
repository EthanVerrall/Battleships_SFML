// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/options_menu.hpp"
#include "utils/include/logger.hpp"
#include "resources/include/asset_registry.hpp"
#include "resources/include/resource_manager.hpp"
#include "utils/include/sizes.hpp"
#include "utils/include/colors.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

using namespace battleships::ui;
using namespace battleships::utils;
using namespace battleships::resources;

namespace battleships::menus {

// ============================================================================
// Class Options_menu
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Options_menu::Options_menu(
    sf::RenderWindow& window
    )
    : Menu(window)
    , _background_static_spritesheet(nullptr)
    , _background_img(nullptr)
    {

    _build__background_image();
}

// ----------------------------------------------------------------------------
void Options_menu::draw() {

    if (_background_img) { _window.draw(*_background_img); }
}

// ----------------------------------------------------------------------------
void Options_menu::update(
    float const dt
    ) {

    (void)dt;
}

// ----------------------------------------------------------------------------
void Options_menu::_build__background_image() {

    auto& resource_manager = Resource_manager::instance();

    if (resource_manager.is_loaded("background_coast")) {

        _background_static_spritesheet = std::move(Asset_registry::load_static_spritesheet("background_control_room_coast"));
    } else if (resource_manager.is_loaded("background_storm")) {

        _background_static_spritesheet = std::move(Asset_registry::load_static_spritesheet("background_control_room_storm"));
    } else if (resource_manager.is_loaded("background_sunset")) {

        _background_static_spritesheet = std::move(Asset_registry::load_static_spritesheet("background_control_room_sunset"));
    } else {

        LOG(Log_lvl::WARN) << "Unknown background image is used on the main menu.";
    }

    if (_background_static_spritesheet == nullptr) {

        LOG(utils::Log_lvl::WARN) << "Static spritesheet was unable to load from asset registry.";
        return;
    }

    _background_static_spritesheet->texture().setSmooth(true);

    if (const auto background_region = _background_static_spritesheet->get_region("background")) {

        _background_img = std::make_unique<sf::Sprite>(_background_static_spritesheet->texture());
        _background_img->setTextureRect(background_region->rect);
    } else {

        LOG(utils::Log_lvl::WARN) << "Unable to find region for background image.";
    }
}

}