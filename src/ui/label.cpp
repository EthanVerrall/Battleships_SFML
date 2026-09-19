// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/label.hpp"

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
    sf::RenderWindow& target,
    const std::string_view widget_name
    )
    : Widget(target, Widget_data{.name = widget_name, .type = Widget_type::LABEL})
    , _font_name("pixel_bold") 
    , _is_visible(true)
{   
    if (const auto temp_font = resources::Asset_registry::load_font("pixel_bold")) {
        _font = std::move(temp_font);
        _text = std::make_unique<sf::Text>(*_font);
        LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name << '\n';
    }
    else {
        LOG(utils::Log_lvl::WARN) << "Failed to load font " << "pixel_bold for label " << widget_name << '\n';
    }
}

Label::Label(
    sf::RenderWindow& target,
    const std::string_view widget_name,
    const std::string_view font_name,
    const std::string_view text,
    const unsigned int character_size
    )
    : Widget(target, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    , _font_name(font_name)
    , _is_visible(true)
{   
    if (const auto temp_font = resources::Asset_registry::load_font("pixel_bold")) {
        _font = std::move(temp_font);
        _text = std::make_unique<sf::Text>(*_font);
        set_text(text);
        set_char_size(character_size);

        LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name << '\n';
    }
    else {
        LOG(utils::Log_lvl::WARN) << "Failed to load font " << font_name << " for label " << widget_name << '\n';
    }
}

Label::Label(
    sf::RenderWindow& target,
    const std::string_view widget_name,
    const std::string_view font_name,
    const std::string_view text,
    const unsigned int character_size,
    const sf::Vector2f pos
    )
    : Widget(target, Widget_data{.name = widget_name, .type = Widget_type::LABEL}) 
    , _font_name(font_name)
    , _is_visible(true)
{   
    if (const auto temp_font = resources::Asset_registry::load_font("pixel_bold")) {
        _font = std::move(temp_font);
        _text = std::make_unique<sf::Text>(*_font);
        set_text(text);
        set_char_size(character_size);
        set_pos(pos);

        LOG(utils::Log_lvl::TRACE) << "Created label " << widget_name << '\n';
    }
    else {
        LOG(utils::Log_lvl::WARN) << "Failed to load font " << font_name << " for label " << widget_name << '\n';
    }
}  


//--------------------------
// Class specific functions
//--------------------------
    void Label::draw() {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << "Unable to draw " << _data.name << " object is nullptr.\n";
            return;
        }

        if (!_is_visible) {
            LOG(utils::Log_lvl::TRACE) << _data.name << " is currently invisible and not being drawn.\n";
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
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get char size.\n";
            return 0;
        }
        else {
            return _text->getCharacterSize();
        }
    }

    sf::Vector2f Label::get_scale() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get scaling size.\n";
            return sf::Vector2f {0.0f , 0.0f};
        }
        else {
            return _text->getScale();
        }
    }

    sf::Vector2f Label::get_pos() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get position.\n";
            return sf::Vector2f {0.0f , 0.0f}; 
        }
        else {
            return _text->getPosition();
        }
    }

    std::string_view Label::get_text() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't get scaling size.\n";
            return "";
        }
        else {
            return _text->getString().toAnsiString();
        }
    }

    std::string_view Label::get_font() const {

        if (!_font || !_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " font or text is nullptr. Can't find a font to return.\n";
            return "";
        }
        else {
            return _font_name;
        }
    }

    sf::Color Label::get_color() const {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, default color returned.\n";
            return utils::colors::DEFAULT_TEXT;
        }
        else {
            return _text->getFillColor();
        }
    }

    Widget_border Label::get_border() const { 
            
        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, default border returned.\n";
            return Widget_border {._color = utils::colors::DEFAULT_BORDER , ._size = 0.0f };
        }
        else {
            Widget_border border_data {
                ._color = _text->getOutlineColor() ,
                ._size = _text->getOutlineThickness()
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
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set char size.\n";
        }
        else {
            _text->setCharacterSize(char_size);
        }
    }

    void Label::set_scale(sf::Vector2f const scale) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set scaling.\n";
        }
        else {
            _text->setScale(scale);
        }
    }

    void Label::set_pos(sf::Vector2f const pos) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set pos.\n";
        }
        else {
            _text->setPosition(pos);
        }
    }

    void Label::set_text(const std::string_view text) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " is nullptr, can't set text.\n";
        }
        else {
            _text->setString(text);
        }
    }

    void Label::set_font(const std::string_view font) {

        if (!_font || !_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " font or text is nullptr, can't set font.\n";
        }
        else {

            if (const auto temp_font = resources::Asset_registry::load_font(font)) {
                _font = std::move(temp_font);
                _text->setFont(*_font);
            }
            else {
                LOG(utils::Log_lvl::WARN) << "failed to change font for " << _data.name << ", font was nullptr.\n";
            }
        }
    }

    void Label::set_color(const sf::Color color) {

        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " text is nullptr, can't set color.\n";
        }
        else {
            _text->setFillColor(color);
        }
    }

    void Label::set_border(const Widget_border border) {
        
        if (!_text) {
            LOG(utils::Log_lvl::WARN) << _data.name << " text is nullptr, can't set border.\n";
        }
        else {
            _text->setOutlineColor(border._color);
            _text->setOutlineThickness(border._size);
        }
    }

    void Label::set_visible(const bool flag) { _is_visible = flag; }
}
