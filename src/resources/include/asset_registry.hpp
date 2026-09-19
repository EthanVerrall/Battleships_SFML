#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "defs.hpp"
#include "spritesheet.hpp"

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

#include <memory>
#include <span>
#include <string_view>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Class Asset_registry
// ----------------------------------------------------------------------------

class Asset_registry final {

public:
    Asset_registry() = delete;

    static std::shared_ptr<Static_spritesheet> load_static_spritesheet(std::string_view const name);

    static std::shared_ptr<Animated_spritesheet> load_animated_spritesheet(std::string_view const name);

    static std::shared_ptr<sf::Music> load_music(std::string_view const name);

    static std::shared_ptr<sf::SoundBuffer> load_sound(std::string_view const name);

    static std::shared_ptr<sf::Font> load_font(std::string_view const name);
};

}
