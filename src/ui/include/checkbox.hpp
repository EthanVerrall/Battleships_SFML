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
// Class Checkbox
// ----------------------------------------------------------------------------

class Checkbox
    : public Widget
    , public battleships::events::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Checkbox(sf::RenderWindow& window, std::string_view const name);

    ~Checkbox();

    Checkbox(Checkbox const&) = delete;
    Checkbox& operator=(Checkbox const&) = delete;
    Checkbox(Checkbox&&) = delete;
    Checkbox& operator=(Checkbox&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;

    void update(float const dt) override;

    void enable(bool const enable);

private:

    void _create__rect();
    void _create__checkmark();

    void _update_checkmark_pos();

    void _register_events();

    void _handle_event__mouse_button_left_release();
    void _handle_event__mouse_moved();

    enum State : std::uint8_t {

        NONE     = 0,
        HOVERING = 1 << 0,
        DISABLED = 1 << 1,
        HIDDEN   = 1 << 2,
        CHECKED  = 1 << 3,

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

    sf::Vector2f get_size() const override;

    sf::Vector2f get_scale() const override;

    sf::Color get_color() const;

    Widget_border get_border() const;

    sf::Color get_checkmark_color() const;

    sf::Vector2f get_checkmark_size() const;

    Widget_border get_checkmark_border() const;

    bool is_enabled() const;
    bool is_hovering() const;
    bool is_visible() const;
    bool is_checked() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(sf::Vector2f const pos) override;

    void set_size(sf::Vector2f const size) override;

    void set_scale(sf::Vector2f const scale) override;

    void set_color(sf::Color const color);

    void set_border(Widget_border const border);

    void set_checkmark_color(sf::Color const color);

    void set_checkmark_size(sf::Vector2f const size);

    void set_checkmark_border(Widget_border const border);

    void set_on_hover(std::function<void()> call_back);
    void set_on_exit_hover(std::function<void()> call_back);

    void set_visible(bool const visible);

    //--------------------------
    // Attributes
    //--------------------------
private:
    std::unique_ptr<sf::RectangleShape> _rect;
    std::unique_ptr<sf::RectangleShape> _checkmark_piece_left;
    std::unique_ptr<sf::RectangleShape> _checkmark_piece_right;

    std::function<void()> _on_hover;
    std::function<void()> _on_exit_hover;
};

}
