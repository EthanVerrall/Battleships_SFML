#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"
#include "utils/include/colors.hpp"
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

struct Widget_border {

    sf::Color _color;
    float _size;
};

// ============================================================================
// Class Widget
// ----------------------------------------------------------------------------

class Widget  {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
protected:

    Widget(
        sf::RenderWindow & window,
        Widget_data const& data
        )
        : _window(window)
        , _data(data)
    {}

public:

    virtual ~Widget() = default;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    virtual void draw() = 0;

    virtual void update(float const dt) = 0;

    //--------------------------
    // Getters
    //--------------------------
public:

    Widget_data get_data() const { return _data; }

    virtual sf::Vector2f get_pos() const = 0;

    virtual sf::Vector2f get_scale() const = 0;

    //--------------------------
    // Setters
    //--------------------------
public:

    virtual void set_pos(sf::Vector2f const pos) = 0;

    virtual void set_scale(sf::Vector2f const scale) = 0;
    
    //--------------------------
    // Attributes
    //--------------------------
protected:

    sf::RenderWindow & _window;
    Widget_data _data;
};

}
