#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "widget.hpp"
#include "events/include/event_listener.hpp"
#include "events/include/event_data.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/Window/Keyboard.hpp"

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
// Class Hotkey_recorder
// ----------------------------------------------------------------------------

class Hotkey_recorder
    : public Widget
    , public battleships::events::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Hotkey_recorder(sf::RenderWindow& window, std::string_view const name);

    ~Hotkey_recorder();

    Hotkey_recorder(Hotkey_recorder const&) = delete;
    Hotkey_recorder& operator=(Hotkey_recorder const&) = delete;
    Hotkey_recorder(Hotkey_recorder&&) = delete;
    Hotkey_recorder& operator=(Hotkey_recorder&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;

    void update(float const dt) override;

    void enable(bool const enable);

private:

    void _create__rect();
    void _create__text();

    void _register_events();

    void _handle_event__mouse_button_left_release();
    void _handle_event__mouse_moved();
    void _handle_event__key_pressed(events::SFML_event_data const& data);

    void _update_label();
    void _fit_text_to_box();

    std::string _key_to_string(sf::Keyboard::Key const key) const;

    enum State : std::uint8_t {

        NONE      = 0,
        HOVERING  = 1 << 0,
        DISABLED  = 1 << 1,
        HIDDEN    = 1 << 2,
        RECORDING = 1 << 3,

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

    sf::Keyboard::Key get_key() const;

    bool is_enabled() const;
    bool is_hovering() const;
    bool is_visible() const;
    bool is_recording() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(sf::Vector2f const pos) override;

    void set_size(sf::Vector2f const size) override;

    void set_scale(sf::Vector2f const scale) override;

    void set_color(sf::Color const color);

    void set_border(Widget_border const border);

    void set_text_color(sf::Color const color);

    void set_key(sf::Keyboard::Key const key);

    void set_on_key_changed(std::function<void(sf::Keyboard::Key const)> call_back);

    void set_on_hover(std::function<void()> call_back);
    void set_on_exit_hover(std::function<void()> call_back);

    void set_visible(bool const visible);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<sf::RectangleShape> _rect;
    std::unique_ptr<sf::Text> _text;
    std::shared_ptr<sf::Font> _font;

    sf::Keyboard::Key _key;

    float _text_padding;

    std::function<void(sf::Keyboard::Key const)> _on_key_changed;
    std::function<void()> _on_hover;
    std::function<void()> _on_exit_hover;
};

}
