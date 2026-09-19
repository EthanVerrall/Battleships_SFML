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

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Button
// ----------------------------------------------------------------------------

class Button
    : public Widget
    , public battleships::event::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Button(sf::RenderWindow& window, std::string_view const name);

    ~Button();

    Button(Button const&) = delete;
    Button& operator=(Button const&) = delete;
    Button(Button&&) = delete;
    Button& operator=(Button&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;

    void update(float const dt) override;

    void enable(bool const enable);

private:

    void _register_events();

    void _handle_event__mouse_button_left_release();
    void _handle_event__mouse_moved();

    enum State : std::uint8_t {

        NONE     = 0,
        HOVERING = 1 << 0,
        DISABLED = 1 << 1,
        HIDDEN   = 1 << 2,

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

    std::string_view get_text_str() const;

    bool is_enabled() const;
    bool is_hovering() const;
    bool is_visible() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(sf::Vector2f const pos) override;

    void set_scale(sf::Vector2f const scale) override;

    void set_on_left_click(std::function<void()> call_back);
    void set_on_hover(std::function<void()> call_back);
    void set_on_exit_hover(std::function<void()> call_back);

    void set_font(std::string_view const font_name);

    void set_text(std::string_view const text);

    void set_text_color(sf::Color const color);

    void set_text_border(Widget_border const border);

    void set_text_char_size(std::size_t const size);

    void set_visible(bool const visible);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<sf::Text> _text;
    std::shared_ptr<sf::Font> _font;

    std::function<void()> _on_left_click;
    std::function<void()> _on_hover;
    std::function<void()> _on_exit_hover;
};

}
