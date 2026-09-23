#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "ui/include/widget.hpp"

#include "SFML/Graphics.hpp"

#include <memory>
#include <string_view>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Label
// ----------------------------------------------------------------------------

class Label : public Widget  {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Label(
        sf::RenderWindow& window,
        const std::string_view widget_name
        );

    Label(
        sf::RenderWindow& window,
        const std::string_view widget_name,
        const std::string_view font_name,
        const std::string_view text,
        const unsigned int character_size
        );

    Label(
        sf::RenderWindow& window,
        const std::string_view widget_name,
        const std::string_view font_name,
        const std::string_view text,
        const unsigned int character_size,
        const sf::Vector2f pos
        );

    ~Label() = default; 
    
    Label(Label const&) = delete;
    Label& operator=(Label const&) = delete;
    Label(Label&&) = delete;
    Label& operator=(Label&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;
    void update(float const dt) override;

    //--------------------------
    // Getters
    //--------------------------
public:

    unsigned int get_char_size() const;
    sf::Vector2f get_size() const override;
    sf::Vector2f get_scale() const override;
    sf::Vector2f get_pos() const override;
    std::string_view get_text() const;
    std::string_view get_font_name() const;
    sf::Color get_color() const;
    Widget_border get_border() const;
    bool get_visibility() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_char_size(const unsigned int char_size);
    void set_size(const sf::Vector2f size) override;
    void set_scale(sf::Vector2f const scale) override;
    void set_pos(sf::Vector2f const pos) override;
    void set_text(const std::string_view text);
    void set_font(const std::string_view font_name);
    void set_color(const sf::Color color);
    void set_border(const Widget_border border);
    void set_visible(const bool flag);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<sf::Text> _text;
    std::shared_ptr<sf::Font> _font;
    std::string_view _font_name;
    bool _is_visible; 
};

}