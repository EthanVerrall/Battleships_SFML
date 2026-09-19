// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/spritesheet.hpp"
#include "utils/include/logger.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::utils;

// ============================================================================
// Class Static_spritesheet
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Static_spritesheet::Static_spritesheet(
    std::shared_ptr<sf::Texture const> texture
    )
    : _texture(std::move(texture))
    , _regions()
{}

// ----------------------------------------------------------------------------
void Static_spritesheet::add_region(
    Static_region_entry const& region
    ) {

    _regions[region.name] = region;
}

// ----------------------------------------------------------------------------
std::optional<Static_region_entry> Static_spritesheet::get_region(
    std::string_view const name
    ) const {

    auto it = _regions.find(name);
    if (it == _regions.end()) {

        LOG(Log_lvl::ERR) << "Region not found: " << name;
        return std::nullopt;
    }

    return it->second;
}

// ----------------------------------------------------------------------------
sf::Texture const& Static_spritesheet::texture() const {

    return *_texture;
}

// ============================================================================
// Class Animated_spritesheet
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Animated_spritesheet::Animated_spritesheet(
    std::shared_ptr<sf::Texture const> texture
    )
    : _texture(std::move(texture))
    , _animations()
{}

// ----------------------------------------------------------------------------
void Animated_spritesheet::add_animation(
    Animation_entry const& animation
    ) {

    _animations[animation.name] = animation;
}

// ----------------------------------------------------------------------------
std::optional<Animation_entry> Animated_spritesheet::get_animation(
    std::string_view const name
    ) const {

    auto it = _animations.find(name);
    if (it == _animations.end()) {

        LOG(Log_lvl::ERR) << "Animation not found: " << name;
        return std::nullopt;
    }

    return it->second;
}

// ----------------------------------------------------------------------------
sf::Texture const& Animated_spritesheet::texture() const {

    return *_texture;
}

// ----------------------------------------------------------------------------
Animation_entry const* Animated_spritesheet::find_animation(
    std::string_view const name
    ) const {

    auto it = _animations.find(name);
    if (it == _animations.end()) {

        return nullptr;
    }

    return &it->second;
}

}
