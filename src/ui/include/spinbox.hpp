#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "widget.hpp"
#include "button.hpp"
#include "label.hpp"
#include "events/include/event_listener.hpp"

#include "SFML/Graphics.hpp"

#include <string_view>
#include <string>
#include <vector>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <functional>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Enum
// ----------------------------------------------------------------------------

enum class Spinbox_layout {

    HORIZONTAL,
    VERTICAL
};

// ============================================================================
// Class Spinbox
// ----------------------------------------------------------------------------

class Spinbox
    : public Widget
    , public battleships::events::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Spinbox(sf::RenderWindow& window, std::string_view const name);

    ~Spinbox() = default;

    Spinbox(Spinbox const&) = delete;
    Spinbox& operator=(Spinbox const&) = delete;
    Spinbox(Spinbox&&) = delete;
    Spinbox& operator=(Spinbox&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;

    void update(float const dt) override;

    void enable(bool const enable);

private:

    void _layout();

    void _update_button_rotation();

    void _update_button_visibility();

    void _select_prev();
    void _select_next();

    void _handle_button_prev_click();
    void _handle_button_next_click();

    enum State : std::uint8_t {

        NONE     = 0,
        DISABLED = 1 << 0,
        HIDDEN   = 1 << 1,

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

    Spinbox_layout get_layout() const;

    sf::Vector2f get_button_offset() const;

    std::vector<std::string> const& get_values() const;

    std::size_t get_selected_index() const;

    std::string_view get_selected_value() const;

    bool is_enabled() const;
    bool is_visible() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(sf::Vector2f const pos) override;

    void set_size(sf::Vector2f const size) override;

    void set_scale(sf::Vector2f const scale) override;

    void set_label_size(sf::Vector2f const size);

    void set_button_size(sf::Vector2f const size);

    void set_layout(Spinbox_layout const layout);

    void set_button_offset(sf::Vector2f const offset);

    void set_button_prev_pos(sf::Vector2f const pos);

    void set_button_next_pos(sf::Vector2f const pos);

    void set_values(std::vector<std::string> const& values);

    void set_selected_index(std::size_t const index);

    void set_increment_direction(bool const first_button_increments);

    void set_on_value_changed(std::function<void(std::string_view const)> call_back);

    void set_font(std::string_view const font_name);

    void set_text_color(sf::Color const color);

    void set_visible(bool const visible);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<Label> _label;
    std::unique_ptr<Button> _button_prev;
    std::unique_ptr<Button> _button_next;

    Spinbox_layout _layout_mode;

    sf::Vector2f _button_offset;

    bool _button_prev_manual_pos;
    bool _button_next_manual_pos;

    std::vector<std::string> _values;
    std::size_t _selected_index;

    bool _first_button_increments;

    std::function<void(std::string_view const)> _on_value_changed;
};

}
