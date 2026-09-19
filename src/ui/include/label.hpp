#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "utils/include/logger.hpp"
#include "include/widget.hpp"
#include "resources/include/asset_registry.hpp"
#include "SFML/Graphics.hpp"
#include <memory.hpp>
#include <string_view>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Label
// ----------------------------------------------------------------------------

class Label : Widget  {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Label(
        sf::RenderWindow& target,
        const std::string_view widget_name
        )
        : Widget(target, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    {   
        if (const auto temp_font = resources::Asset_registry::load_font("pixel_bold")) {
            _font = std::move(temp_font);
            _text = std::make_unique<sf::Text>(*_font);
            LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name << '\n';
        }
        else {
            LOG(utils::Log_lvl::ERR) << "Failed to load font " << "pixel_bold for label " << widget_name << '\n';
        }
    }

    Label(
        sf::RenderWindow& target,
        const std::string_view font_name,
        const std::string_view text,
        const unsigned int character_size,
        const std::string_view widget_name
        )
        : Widget(target, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    {   
        if (const auto temp_font = resources::Asset_registry::load_font("pixel_bold")) {
            _font = std::move(temp_font);
            _text = std::make_unique<sf::Text>(*_font);
            LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name << '\n';
        }
        else {
            LOG(utils::Log_lvl::ERR) << "Failed to load font " << font_name << " for label " << widget_name << '\n';
        }
    }

    Label(
        sf::RenderWindow& target,
        const std::string_view font_name,
        const std::string_view text,
        const unsigned int character_size,
        const sf::Vector2f pos,
        const std::string_view widget_name
        )
        : Widget(target, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    {   
        if (const auto temp_font = resources::Asset_registry::load_font("pixel_bold")) {
            _font = std::move(temp_font);
            _text = std::make_unique<sf::Text>(*_font);
            LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name << '\n';
        }
        else {
            LOG(utils::Log_lvl::ERR) << "Failed to load font " << font_name << " for label " << widget_name << '\n';
        }
    }

    ~Label() = default; 
    
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

    unsigned int get_size() const;
    sf::Vector2f get_scale() const override;
    sf::Vector2f get_pos() const override;
    std::string_view get_text() const;
    const sf::Font& get_font() const;
    sf::Color get_color() const;
    Widget_border get_border() const;

    //--------------------------
    // Setters
    //--------------------------
public:

    void set_size(const unsigned int pos);
    void set_scale(sf::Vector2f const scale) override;
    void set_pos(sf::Vector2f const pos) override;
    void set_text(const std::string_view text);
    void set_font(const sf::Font& font);
    void set_color(const sf::Color color);
    void set_border(const Widget_border border);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unique_ptr<sf::Text> _text;
    std::shared_ptr<sf::Font> _font;

};

}