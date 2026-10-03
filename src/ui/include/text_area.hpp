#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "ui/include/widget.hpp"
#include "ui/include/button.hpp"
#include "events/include/event_listener.hpp"

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

class Text_area
    : public Widget
    , public events::Event_listener
    {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Text_area(
            sf::RenderWindow& window,
            const std::string_view widget_name
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
        NONE                    = 0,
        HIDDEN                  = 1 << 0,
        HOVERING_CONTENT_AREA   = 1 << 1, //Not implemented //Is your mouse hovering over the text area where the box is
        FOCUSED                 = 1 << 2,
        TYPING_DISABLED         = 1 << 3, //Not implemented //Are you able to type in it
        SCROLLABLE              = 1 << 4,
        HOVERING_BAR            = 1 << 5, //Not implemented //Are you hovering the scrollbar
        DRAGGING                = 1 << 6,

        DEFAULT = NONE
    }; std::uint8_t _state;

    //Overloading the | operator to allow chaining states
    friend inline State operator|(State a, State b) {
        return static_cast<State>(
            static_cast<std::uint8_t> (a) | static_cast<std::uint8_t> (b)
        );
    }

    void set_state(const State state);
    void add_state(const State state);
    void remove_state(const State state);
    bool has_state(const State state) const;

    //--------------------------
    // Getters
    //--------------------------
public:

    //Positioning and sizing
    sf::Vector2f get_pos() const;
    sf::Vector2f get_size() const; // This returns the total size of the text area, includes the scrollbar dimensions
    sf::Vector2f get_scale() const; // Not implemented

    //Rect functions
    sf::Color get_content_area_color() const;
    sf::Vector2f get_content_area_size() const; // This returns the size of the typing zone for your text area

    //Scroll functions
    float get_scoll_bar_width() const;
    sf::Color get_scroll_bar_bg_color() const; // Color for behind the bar
    sf::Color get_scroll_bar_color() const; // Color for the scrollbar
    bool is_scrollable() const;

    //Other functionality
    bool is_visible() const;
    bool is_typeable() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    //Positioning and sizing

    /*
    This function sets the position of your text_area to its top left most point
    The scroll bar will automatically be moved along and attached to the far right.
    */
    void set_pos(const sf::Vector2f pos);

    /*
    This function will adjust the content area size.
    This is the space where text will be contained in and written to.
    This function will not adjust the width of the scroll bar.
    Will automatically move the scroll bar to the far right of the new provided size.

    Example: if your size is { 400.0f, 200.0f } your scroll bar will start from 400.0f ->
    and attach to the right by its width
    */
    void set_size(const sf::Vector2f size);

    //Not implemented
    void set_scale(const sf::Vector2f scale);

    //Rect functions
    void set_content_area_color(const sf::Color color);
    void set_content_area_width(const float width);
    void set_content_area_height(const float height);

    //Scroll functions
    void set_scroll_bar_width(const float width);
    void set_scroll_bar_bg_color(const sf::Color color); //Color for behind the bar
    void set_scroll_bar_color(const sf::Color color); //Color for the scrollbar
    void set_scrollable(const bool flag);

    //Other functionality
    void set_visible(const bool flag);
    void set_typeable(const bool flag);

    //--------------------------
    // Private functions
    //--------------------------
private:

    void reposition_scroll_bar();
    void resize_scroll_bar();
    void register_events();

    void _handle_event__mouse_button_left_held();
    void scrolling(const sf::Vector2f mouse_pos);

    void _handle_event__mouse_button_left_release();

    void _handle_event__window_text_entered(const sf::Event& event_data);
    void build_string(const char32_t unicode);

    //--------------------------
    // Attributes
    //--------------------------
private:

    // Area the user types in, _content_area is where the text displays,
    std::unique_ptr<sf::RectangleShape> _content_area;
    // The _caret is the blinking cursor.
    std::unique_ptr<sf::RectangleShape> _caret;

    // All the different pieces of the scroll bar
    std::unique_ptr<sf::RectangleShape> _bar_background;
    std::unique_ptr<sf::RectangleShape> _scroll_bar;
    std::unique_ptr<Button> _up_arrow;                  //Not implemented yet
    std::unique_ptr<Button> _down_arrow;                //Not implemented yet

    //Used for scroll bar, recording old mouse position
    sf::Vector2f _old_mouse_pos;

    // Font details
    std::string_view _font_name;
    std::shared_ptr<sf::Font> _font;
    std::unique_ptr<sf::Text> _text;
};

}