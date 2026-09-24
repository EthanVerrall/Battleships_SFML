#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"

#include <string_view>
#include <span>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Enums
// ----------------------------------------------------------------------------

enum class Animation_mode : std::uint8_t {

    LOOP,
    ONE_SHOT,
    HOLD,
    PING_PONG
};

// ============================================================================
// Structs
// ----------------------------------------------------------------------------

struct Static_region_entry {

    std::string_view name{};
    sf::IntRect rect{};
};

struct Static_sheet_entry {

    std::string_view name{};
    std::string_view path{};
    std::span<Static_region_entry const> regions{};
};

struct Animation_entry {

    std::string_view name{};
    std::span<sf::IntRect const> frames{};
    float fps{};
    Animation_mode mode{};
};

struct Animated_sheet_entry {

    std::string_view name{};
    std::string_view path{};
    std::span<Animation_entry const> animations{};
};

struct Audio_entry {

    std::string_view name{};
    std::string_view path{};
};

struct Sound_entry {

    std::string_view name{};
    std::string_view path{};
};

struct Font_entry {

    std::string_view name{};
    std::string_view path{};
};

}
