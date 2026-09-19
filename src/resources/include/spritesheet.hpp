#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "defs.hpp"

#include "SFML/Graphics.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>
#include <optional>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Class Static_spritesheet
// ----------------------------------------------------------------------------

class Static_spritesheet {

public:
    explicit Static_spritesheet(std::shared_ptr<sf::Texture const> texture);

    void add_region(Static_region_entry const& region);

    std::optional<Static_region_entry> get_region(std::string_view const name) const;

    sf::Texture const& texture() const;

private:
    std::shared_ptr<sf::Texture const> _texture;
    std::unordered_map<std::string_view, Static_region_entry> _regions;
};

// ============================================================================
// Class Animated_spritesheet
// ----------------------------------------------------------------------------

class Animated_spritesheet {

public:
    explicit Animated_spritesheet(std::shared_ptr<sf::Texture const> texture);

    void add_animation(Animation_entry const& animation);

    std::optional<Animation_entry> get_animation(std::string_view const name) const;

    // Invalidated if add_animation later overwrites this same name
    Animation_entry const* find_animation(std::string_view const name) const;

    sf::Texture const& texture() const;

private:
    std::shared_ptr<sf::Texture const> _texture;
    std::unordered_map<std::string_view, Animation_entry> _animations;
};

}
