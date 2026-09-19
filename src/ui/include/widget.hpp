#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"

#include <string_view>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Enum
// ----------------------------------------------------------------------------

enum class Widget_type {

    LABEL,
    BUTTON,
    TEXT_AREA,
    SLIDER
};

// ============================================================================
// Struct
// ----------------------------------------------------------------------------

struct Widget_data {

    std::string_view name;
    Widget_type type;
};

// ============================================================================
// Class Widget
// ----------------------------------------------------------------------------

class Widget  {

public:
    virtual ~Widget() = default;

    virtual void draw() = 0;

    virtual void update(float const dt) = 0;

    Widget_data get_data() const { return _data; }

    virtual sf::Vector2f get_pos() const = 0;

    virtual sf::Vector2f get_scale() const = 0;

    virtual void set_pos(sf::Vector2f const pos) = 0;

    virtual sf::Vector2f set_scale(sf::Vector2f const scale) = 0;

protected:
    Widget(
        sf::RenderWindow const& window,
        Widget_data const& data
        )
        : _window(window)
        , _data(data)
    {}

    sf::RenderWindow const& _window;
    Widget_data _data{};
};

}
