#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "widget.hpp"
#include "events/include/event_listener.hpp"

#include "SFML/Graphics.hpp"

#include <string_view>
#include <string>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <functional>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Slider
// ----------------------------------------------------------------------------

class Slider
    : public Widget
    , public battleships::events::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Slider(sf::RenderWindow& window, std::string_view const name);

    ~Slider();

    Slider(Slider const&) = delete;
    Slider& operator=(Slider const&) = delete;
    Slider(Slider&&) = delete;
    Slider& operator=(Slider&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;

    void update(float const dt) override;

    void enable(bool const enable);

private:

    void _create__text();
    void _create__rect_outside();
    void _create__rect_inside();
    void _create__handle();

    void _register_events();

    void _handle_event__mouse_button_left_press();
    void _handle_event__mouse_button_left_release();
    void _handle_event__mouse_moved();

    void _update_handle_from_value();
    void _update_value_from_mouse_x(float const mouse_x);
    void _update_text_pos();

    sf::Vector2f _get_mouse_world_pos() const;
    sf::FloatRect _get_track_bounds() const;

    enum State : std::uint8_t {

        NONE     = 0,
        HOVERING = 1 << 0,
        DISABLED = 1 << 1,
        HIDDEN   = 1 << 2,
        DRAGGING = 1 << 3,

        DEFAULT = NONE
    }; std::uint8_t _state;

    std::string _state_to_string(std::uint8_t const state);
    void _set_flag(State const flag, bool const value);
    bool _has_flag(State const flag) const;

    //--------------------------
    // Getters
    //--------------------------
public:

    sf::Vector2f get_pos() const override;

    sf::Vector2f get_scale() const override;

    sf::Vector2f get_size() const;

    float get_value() const;

    float get_min_value() const;
    float get_max_value() const;

    float get_handle_radius() const;

    sf::Vector2f get_text_offset() const;

    std::size_t get_text_decimal_precision() const;

    bool is_enabled() const;
    bool is_hovering() const;
    bool is_dragging() const;
    bool is_visible() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(sf::Vector2f const pos) override;

    void set_scale(sf::Vector2f const scale) override;

    void set_size(sf::Vector2f const size);

    void set_on_drag(std::function<void()> call_back);
    void set_on_exit_drag(std::function<void()> call_back);
    void set_on_hover(std::function<void()> call_back);
    void set_on_exit_hover(std::function<void()> call_back);
    void set_on_value_changed(std::function<void(float const)> call_back);

    void set_font(std::string_view const font_name);

    void set_text_color(sf::Color const color);

    void set_text_border(Widget_border const border);

    void set_text_char_size(std::size_t const size);

    void set_text_offset(sf::Vector2f const offset);

    void set_show_value_text(bool const show);

    void set_text_decimal_precision(std::size_t const precision);

    void set_track_color(sf::Color const color);
    void set_track_border(Widget_border const border);

    void set_fill_color(sf::Color const color);
    void set_fill_border(Widget_border const border);

    void set_handle_radius(float const radius);
    void set_handle_color(sf::Color const color);
    void set_handle_border(Widget_border const border);

    void set_visible(bool const visible);

    void set_range(float const min, float const max);

    void set_value(float const val);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<sf::Text> _text;
    std::unique_ptr<sf::RectangleShape> _rect_outside;
    std::unique_ptr<sf::RectangleShape> _rect_inside;
    std::unique_ptr<sf::CircleShape> _handle;

    std::shared_ptr<sf::Font> _font;

    std::function<void()> _on_drag;
    std::function<void()> _on_exit_drag;
    std::function<void()> _on_hover;
    std::function<void()> _on_exit_hover;
    std::function<void(float const)> _on_value_changed;

    bool _show_value_text;

    sf::Vector2f _text_offset;

    std::size_t _text_decimal_precision;

    float _min_value;
    float _max_value;
    float _value;
};

}
