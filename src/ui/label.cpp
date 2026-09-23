// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/label.hpp"
#include "utils/include/logger.hpp"
#include "resources/include/asset_registry.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Class Label
// ----------------------------------------------------------------------------    

//--------------------------
// Constructor / Destructor
//--------------------------    
Label::Label(
    sf::RenderWindow& window,
    const std::string_view widget_name
    )
    : Widget(window, Widget_data{.name = widget_name, .type = Widget_type::LABEL})
    , _font_name("pixel_bold") 
    , _is_visible(true)
{   
    if (const auto temp_font = resources::Asset_registry::load_font(_font_name)) {
        _font = std::move(temp_font);
        _text = std::make_unique<sf::Text>(*_font);
        LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name;
    }
    else {
        LOG(utils::Log_lvl::WARN) << "Failed to load " << _font_name << " font for label " << widget_name;
    }
}

Label::Label(
    sf::RenderWindow& window,
    const std::string_view widget_name,
    const std::string_view font_name,
    const std::string_view text,
    const unsigned int character_size
    )
    : Widget(window, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    , _font_name(font_name)
    , _is_visible(true)
{   
    if (const auto temp_font = resources::Asset_registry::load_font(_font_name)) {
        _font = std::move(temp_font);
        _text = std::make_unique<sf::Text>(*_font, text, character_size);

        LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name;
    }
    else {
        LOG(utils::Log_lvl::WARN) << "Failed to load font " << font_name << " for label " << widget_name;
    }
}

Label::Label(
    sf::RenderWindow& window,
    const std::string_view widget_name,
    const std::string_view font_name,
    const std::string_view text,
    const unsigned int character_size,
    const sf::Vector2f pos
    )
    : Widget(window, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    , _font_name(font_name)
    , _is_visible(true)
{   
    if (const auto temp_font = resources::Asset_registry::load_font(_font_name)) {
        _font = std::move(temp_font);
        _text = std::make_unique<sf::Text>(*_font, text, character_size);
        set_pos(pos);

        LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name;
    }
    else {
        LOG(utils::Log_lvl::WARN) << "Failed to load font " << font_name << " for label " << widget_name;
    }
}  


//--------------------------
// Class specific functions
//--------------------------
    void Label::draw() {

        if (!_text || !_is_visible) {
            return;
        }

        _window.draw(*_text);
    }

    void Label::update(float const dt) { static_cast<void> (dt); /*Does nothing*/ }

//--------------------------
// Getters
//--------------------------
    unsigned int Label::get_char_size() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get char size.";
            return 0;
        }
        else {
            return _text->getCharacterSize();
        }
    }

    sf::Vector2f Label::get_size() const {

         if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get size.";
            return sf::Vector2f {0.0f , 0.0f};
        }
        else {
            return _text->getGlobalBounds().size;
        }
    }

    sf::Vector2f Label::get_scale() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get scaling size.";
            return sf::Vector2f {0.0f , 0.0f};
        }
        else {
            return _text->getScale();
        }
    }

    sf::Vector2f Label::get_pos() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get position.";
            return sf::Vector2f {0.0f , 0.0f}; 
        }
        else {
            return _text->getPosition();
        }
    }

    std::string_view Label::get_text() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get scaling size.";
            return "";
        }
        else {
            return _text->getString().toAnsiString();
        }
    }

    std::string_view Label::get_font_name() const {

        if (!_font || !_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " font or text is nullptr. Can't find a font to return.";
            return "";
        }
        else {
            return _font_name;
        }
    }

    sf::Color Label::get_text_color() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, default color returned.";
            return utils::colors::DEFAULT_TEXT;
        }
        else {
            return _text->getFillColor();
        }
    }

    Widget_border Label::get_border() const { 
            
        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, default border returned.";
            return Widget_border {.color = utils::colors::DEFAULT_BORDER , .size = 0.0f };
        }
        else {
            Widget_border border_data {
                .color = _text->getOutlineColor() ,
                .size = _text->getOutlineThickness()
            };
            return border_data;
        }
    }

    bool Label::get_visibility() const { return _is_visible; }


//--------------------------
// Setters
//--------------------------    
    void Label::set_char_size(const unsigned int char_size) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set char size.";
        }
        else {
            _text->setCharacterSize(char_size);
        }
    }

    void Label::set_size(const sf::Vector2f size) {

        static_cast<void> (size); //Do nothing - throw away
        LOG(utils::Log_lvl::WARN) << "Setting size for label is unimplemented";
    }

    void Label::set_scale(sf::Vector2f const scale) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set scaling.";
        }
        else {
            _text->setScale(scale);
        }
    }

    void Label::set_pos(sf::Vector2f const pos) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set pos.";
        }
        else {
            _text->setPosition(pos);
        }
    }

    void Label::set_text(const std::string_view text) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set text.";
        }
        else {
            _text->setString(text);
        }
    }

    void Label::set_font(const std::string_view font_name) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " text is nullptr, can't set font.";
        }
        else {

            if (const auto temp_font = resources::Asset_registry::load_font(font_name)) {
                _font = std::move(temp_font);
                _text->setFont(*_font);
                _font_name = font_name;
            }
            else {
                LOG(utils::Log_lvl::WARN) << "failed to change font for " << _data.name << ", font was nullptr.";
            }
        }
    }

    void Label::set_text_color(const sf::Color color) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " text is nullptr, can't set color.";
        }
        else {
            _text->setFillColor(color);
        }
    }

    void Label::set_border(const Widget_border border) {
        
        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " text is nullptr, can't set border.";
        }
        else {
            _text->setOutlineColor(border.color);
            _text->setOutlineThickness(border.size);
        }
    }

    void Label::set_visible(const bool flag) { _is_visible = flag; }
}
