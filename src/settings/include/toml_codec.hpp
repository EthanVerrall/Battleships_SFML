#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"

#include <toml++/toml.hpp>

#include <cstdint>
#include <string>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::settings {

// ============================================================================
// Toml_codec
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
template<typename T>
struct Toml_codec {

    static T encode(T const& value) { return value; }

    static T decode(toml::node_view<toml::node> const& node) {

        return node.value_or<T>(T{});
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::int8_t> {

    static std::int64_t encode(std::int8_t const value) { return value; }

    static std::int8_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::int8_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::int16_t> {

    static std::int64_t encode(std::int16_t const value) { return value; }

    static std::int16_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::int16_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::int32_t> {

    static std::int64_t encode(std::int32_t const value) { return value; }

    static std::int32_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::int32_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::uint8_t> {

    static std::int64_t encode(std::uint8_t const value) { return static_cast<std::int64_t>(value); }

    static std::uint8_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::uint8_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::uint16_t> {

    static std::int64_t encode(std::uint16_t const value) { return static_cast<std::int64_t>(value); }

    static std::uint16_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::uint16_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::uint32_t> {

    static std::int64_t encode(std::uint32_t const value) { return static_cast<std::int64_t>(value); }

    static std::uint32_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::uint32_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<std::uint64_t> {

    static std::int64_t encode(std::uint64_t const value) { return static_cast<std::int64_t>(value); }

    static std::uint64_t decode(toml::node_view<toml::node> const& node) {

        return static_cast<std::uint64_t>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<char> {

    static std::int64_t encode(char const value) { return static_cast<std::int64_t>(value); }

    static char decode(toml::node_view<toml::node> const& node) {

        return static_cast<char>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<float> {

    static double encode(float const value) { return static_cast<double>(value); }

    static float decode(toml::node_view<toml::node> const& node) {

        return static_cast<float>(node.value_or<double>(0.0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Vector2i> {

    static toml::table encode(sf::Vector2i const& value) {

        return toml::table{{ "x", value.x }, { "y", value.y }};
    }

    static sf::Vector2i decode(toml::node_view<toml::node> const& node) {

        return {
            node["x"].value_or<int>(0),
            node["y"].value_or<int>(0)
            };
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Vector2u> {

    static toml::table encode(sf::Vector2u const& value) {

        return toml::table{{ "x", static_cast<std::int64_t>(value.x) }, { "y", static_cast<std::int64_t>(value.y) }};
    }

    static sf::Vector2u decode(toml::node_view<toml::node> const& node) {

        return {
            static_cast<unsigned int>(node["x"].value_or<std::int64_t>(0)),
            static_cast<unsigned int>(node["y"].value_or<std::int64_t>(0))
            };
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Vector2f> {

    static toml::table encode(sf::Vector2f const& value) {

        return toml::table{{ "x", value.x }, { "y", value.y }};
    }

    static sf::Vector2f decode(toml::node_view<toml::node> const& node) {

        return {
            node["x"].value_or<float>(0.0f),
            node["y"].value_or<float>(0.0f)
            };
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Color> {

    static toml::table encode(sf::Color const& value) {

        return toml::table{

            { "r", static_cast<std::int64_t>(value.r) },
            { "g", static_cast<std::int64_t>(value.g) },
            { "b", static_cast<std::int64_t>(value.b) },
            { "a", static_cast<std::int64_t>(value.a) },
            };
    }

    static sf::Color decode(toml::node_view<toml::node> const& node) {

        return {
            static_cast<std::uint8_t>(node["r"].value_or<std::int64_t>(0)),
            static_cast<std::uint8_t>(node["g"].value_or<std::int64_t>(0)),
            static_cast<std::uint8_t>(node["b"].value_or<std::int64_t>(0)),
            static_cast<std::uint8_t>(node["a"].value_or<std::int64_t>(255)),
            };
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::VideoMode> {

    static toml::table encode(sf::VideoMode const& value) {

        return toml::table{

            { "width", static_cast<std::int64_t>(value.size.x) },
            { "height", static_cast<std::int64_t>(value.size.y) },
            { "bpp", static_cast<std::int64_t>(value.bitsPerPixel) },
            };
    }

    static sf::VideoMode decode(toml::node_view<toml::node> const& node) {

        sf::VideoMode mode;
        mode.size.x = static_cast<unsigned int>(node["width"].value_or<std::int64_t>(0));
        mode.size.y = static_cast<unsigned int>(node["height"].value_or<std::int64_t>(0));
        mode.bitsPerPixel = static_cast<unsigned int>(node["bpp"].value_or<std::int64_t>(32));

        return mode;
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::State> {

    static std::int64_t encode(sf::State const value) {

        return static_cast<std::int64_t>(value);
    }

    static sf::State decode(toml::node_view<toml::node> const& node) {

        return static_cast<sf::State>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Keyboard::Key> {

    static std::int64_t encode(sf::Keyboard::Key const value) {

        return static_cast<std::int64_t>(value);
    }

    static sf::Keyboard::Key decode(toml::node_view<toml::node> const& node) {

        return static_cast<sf::Keyboard::Key>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Keyboard::Scancode> {

    static std::int64_t encode(sf::Keyboard::Scancode const value) {

        return static_cast<std::int64_t>(value);
    }

    static sf::Keyboard::Scancode decode(toml::node_view<toml::node> const& node) {

        return static_cast<sf::Keyboard::Scancode>(node.value_or<std::int64_t>(0));
    }
};

// ----------------------------------------------------------------------------
template<>
struct Toml_codec<sf::Mouse::Button> {

    static std::int64_t encode(sf::Mouse::Button const value) {

        return static_cast<std::int64_t>(value);
    }

    static sf::Mouse::Button decode(toml::node_view<toml::node> const& node) {

        return static_cast<sf::Mouse::Button>(node.value_or<std::int64_t>(0));
    }
};

}
