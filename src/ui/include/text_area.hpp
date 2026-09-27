#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "ui/include/widget.hpp"

#include "SFML/Graphics.hpp"

#include <cstdint>
#include <string_view>
#include <memory>


// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class text_area
// ----------------------------------------------------------------------------

class Text_area : public Widget  {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Text_area(
            sf::RenderWindow& window, const
            std::string_view widget_name
            );

    ~Text_area() = default;

    Text_area(Text_area const&) = delete;
    Text_area& operator=(Text_area const&) = delete;
    Text_area(Text_area&&) = delete;
    Text_area& operator=(Text_area&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;
    void update(float const dt) override;

    //--------------------------
    // States
    //--------------------------
private:

    enum State : std::uint8_t {
        NONE        = 0,
        HIDDEN      = 1 << 0,
        HOVERING    = 1 << 1,
        FOCUS       = 1 << 2,
        DISABLED    = 1 << 3,
        SCROLLABLE  = 1 << 4,

        DEFAULT = NONE
    }; std::uint8_t _state;

    void set_state(const State state);
    void add_state(const State state);
    void remove_state(const State state);
    bool has_state(const State state) const;

    //--------------------------
    // Getters
    //--------------------------
public:

    sf::Vector2f get_pos() const;
    sf::Vector2f get_size() const;
    sf::Vector2f get_scale() const;

    //Rect functions
    sf::Color get_rect_color() const;

    //Scroll functions
    float get_scroll_bar_width() const;
    sf::Color get_scroll_bar_color() const;
    bool is_scrollable() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_pos(const sf::Vector2f pos);
    void set_size(const sf::Vector2f size);
    void set_scale(const sf::Vector2f scale);

    //Rect functions
    void set_rect_color(const sf::Color color);

    //Scroll functions
    void set_scroll_bar_width(const float width);
    void set_scroll_bar_color(const sf::Color color);
    void set_scrollable(const bool flag);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<sf::RectangleShape> _rect;
    std::unique_ptr<sf::RectangleShape> _scroll_bar;
    std::unique_ptr<sf::RectangleShape> _caret;
    std::string_view _font_name;
    std::shared_ptr<sf::Font> _font;
    std::unique_ptr<sf::Text> _text;

};

}