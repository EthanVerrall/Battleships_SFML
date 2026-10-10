#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "widget.hpp"
#include "label.hpp"

#include "SFML/Graphics.hpp"

#include <memory>
#include <string_view>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Fps_counter
// ----------------------------------------------------------------------------

class Fps_counter : public Widget {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Fps_counter(sf::RenderWindow& window, std::string_view const name);

    ~Fps_counter() = default;

    Fps_counter(Fps_counter const&) = delete;
    Fps_counter& operator=(Fps_counter const&) = delete;
    Fps_counter(Fps_counter&&) = delete;
    Fps_counter& operator=(Fps_counter&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;

    void update(float const dt) override;

private:

    void _create__lbl();

    void _refresh_text(float const fps);

    //--------------------------
    // Getters
    //--------------------------
public:

    sf::Vector2f get_pos() const override;

    sf::Vector2f get_size() const override;

    sf::Vector2f get_scale() const override;

    sf::Color get_color() const;

    Widget_border get_border() const;

    unsigned int get_char_size() const;

    float get_fps() const;

    bool is_visible() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(sf::Vector2f const pos) override;

    void set_size(sf::Vector2f const size) override;

    void set_scale(sf::Vector2f const scale) override;

    void set_color(sf::Color const color);

    void set_border(Widget_border const border);

    void set_char_size(unsigned int const char_size);

    void set_visible(bool const visible);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<Label> _lbl;

    unsigned int _frames;
    float _elapsed_time;

    float _fps;

    bool _is_visible;
};

}
