#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <cstdint>
#include <string>
#include <string_view>
#include <variant>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::settings {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using Setting_value = std::variant<
    // Primitives
    bool,
    char,
    std::int8_t,
    std::int16_t,
    std::int32_t,
    std::int64_t,
    std::uint8_t,
    std::uint16_t,
    std::uint32_t,
    std::uint64_t,
    float,
    double,
    std::string,

    // SFML geometry
    sf::Vector2i,
    sf::Vector2u,
    sf::Vector2f,

    // SFML video / window
    sf::VideoMode,
    sf::State,

    // SFML color
    sf::Color,

    // SFML input
    sf::Keyboard::Key,
    sf::Keyboard::Scancode,
    sf::Mouse::Button
    >;

// ============================================================================
// Structs
// ----------------------------------------------------------------------------

struct Setting_key_entry {

    std::string_view name;
    Setting_value type;
};

// ============================================================================
// Constants
// ----------------------------------------------------------------------------

inline constexpr char const* SETTINGS_FILE_PATH = "assets/data/settings.toml";

inline Setting_key_entry const SETTING_KEYS[] = {

    { "window_state", sf::State{} },
    { "music_volume", float{} },
    { "sfx_volume", float{} },
    { "anti_aliasing", std::uint8_t{} }
};

}
