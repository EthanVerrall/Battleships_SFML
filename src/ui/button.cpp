// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/button.hpp"
#include "resources/include/asset_registry.hpp"
#include "resources/include/resource_manager.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::resources;
using namespace battleships::events;
using namespace battleships::utils;

// ============================================================================
// Class Button
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Button::Button(
    sf::RenderWindow& window,
    std::string_view const name
    )
    : Widget(window, Widget_data{ .name = name, .type = Widget_type::BUTTON })
    , _state(State::DEFAULT)
    , _text(nullptr)
    , _font(nullptr)
    , _on_left_click()
    , _on_hover()
    , _on_exit_hover()
    {

    auto const font = Asset_registry::load_font("pixel_bold");

    if (font == nullptr) {

        LOG(Log_lvl::WARN) << "font pixel_bold == nullptr";
    } else {

        _font = std::move(font);
        _text = std::make_unique<sf::Text>(*_font, "");
    }

    set_text_color(colors::DEFAULT_TEXT);

    _register_events();
}

// ----------------------------------------------------------------------------
Button::~Button() {

    auto& sfml_event_manager = SFML_event_manager::instance();

    sfml_event_manager.deregister_listener(listener_id());
}

// ----------------------------------------------------------------------------
void Button::draw() {

    if (_has_flag(State::HIDDEN)) {

        // Not visible so don't draw
    } else {

        if (_text != nullptr) { _window.draw(*_text); }
    }
}

// ----------------------------------------------------------------------------
void Button::update(
    float const dt
    ) {

    // Do nothing
    (void) dt;
}

// ----------------------------------------------------------------------------
void Button::enable(
    bool const enable
    ) {

    _set_flag(State::DISABLED, !enable);
}

// ----------------------------------------------------------------------------
void Button::_register_events() {

    auto& sfml_event_manager = SFML_event_manager::instance();

    sfml_event_manager.register_callback(
        SFML_event_type::MOUSE_BUTTON_LEFT_RELEASE,
        [this](SFML_event_data const&){ _handle_event__mouse_button_left_release(); },
        listener_id()
        );

    sfml_event_manager.register_callback(
        SFML_event_type::MOUSE_MOVED,
        [this](SFML_event_data const&){ _handle_event__mouse_moved(); },
        listener_id()
        );
}

// ----------------------------------------------------------------------------
void Button::_handle_event__mouse_button_left_release() {

    if (!_has_flag(State::DISABLED) && is_hovering() && _on_left_click) {

        LOG(Log_lvl::TRACE) << "Button left clicked";

        _on_left_click();
    }
}

// ----------------------------------------------------------------------------
void Button::_handle_event__mouse_moved() {

    if (_has_flag(State::DISABLED)) {

        // Do nothing
    } else if (_has_flag(State::HOVERING)) {

        // Already in hovering state so check if we are no longer hovering anymore
        if (!is_hovering() && _on_exit_hover) {

            _set_flag(State::HOVERING, false);
            _on_exit_hover();
        }
    } else {

        // Not in hovering state so check if we are now hovering
        if (is_hovering() && _on_hover) {

            _set_flag(State::HOVERING, true);
            _on_hover();
        }
    }
}

// ----------------------------------------------------------------------------
std::string Button::_state_to_string(
    std::uint8_t const state
    ) {

    if (state == State::NONE) { return "NONE"; }

    std::string result;

    if (state & State::HOVERING) { result += "HOVERING|"; }
    if (state & State::DISABLED) { result += "DISABLED|"; }
    if (state & State::HIDDEN) { result += "HIDDEN|"; }

    if (!result.empty()) { result.pop_back(); }

    return result;
}

// ----------------------------------------------------------------------------
void Button::_set_flag(
    State const flag,
    bool const value
    ) {

    std::uint8_t const new_state = value ? (_state | flag) : (_state & ~flag);

    if (new_state == _state) { return; }

    LOG(Log_lvl::TRACE) << "Changing button state from " <<
    _state_to_string(_state) << " to " << _state_to_string(new_state);

    _state = new_state;
}

// ----------------------------------------------------------------------------
bool Button::_has_flag(
    State const flag
    ) const {

    return (_state & flag) != 0;
}

// ----------------------------------------------------------------------------
sf::Vector2f Button::get_pos() const {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";

        return {};
    }

    return _text->getPosition();
}

// ----------------------------------------------------------------------------
sf::Vector2f Button::get_scale() const {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";

        return {};
    }

    return _text->getScale();
}

// ----------------------------------------------------------------------------
std::string_view Button::get_text_str() const {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
        return {};
    }

    return _text->getString().toAnsiString();
}

// ----------------------------------------------------------------------------
bool Button::is_enabled() const {

    return !_has_flag(State::DISABLED);
}

// ----------------------------------------------------------------------------
bool Button::is_visible() const {

    return !_has_flag(State::HIDDEN);
}

// ----------------------------------------------------------------------------
bool Button::is_hovering() const {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
        return {};
    }

    sf::Vector2i const mouse_pixel_pos = sf::Mouse::getPosition(_window);
    sf::Vector2f const mouse_world_pos = _window.mapPixelToCoords(mouse_pixel_pos);

    return _text->getGlobalBounds().contains(mouse_world_pos);
}

// ----------------------------------------------------------------------------
void Button::set_pos(
    sf::Vector2f const pos
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_pos from (" << _text->getPosition().x << ", " << _text->getPosition().y << ") to ("
        << pos.x << ", " << pos.y << ')';

        _text->setPosition(pos);
    }
}

// ----------------------------------------------------------------------------
void Button::set_scale(
    sf::Vector2f const scale
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_scale from (" << _text->getScale().x << ", " << _text->getScale().y << ") to ("
        << scale.x << ", " << scale.y << ')';

        _text->setScale(scale);
    }
}

// ----------------------------------------------------------------------------
void Button::set_on_left_click(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << "set_on_left_click";

    _on_left_click = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Button::set_on_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << "set_on_hover";

    _on_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Button::set_on_exit_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << "_on_exit_hover";

    _on_exit_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Button::set_font(
    std::string_view const font_name
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_font to " << font_name;

        auto const font = Asset_registry::load_font(font_name);

        if (font == nullptr) {

            LOG(Log_lvl::WARN) << "font " << font_name << " == nullptr";
        } else {

            _font = std::move(font);
            _text->setFont(*_font);
        }
    }
}

// ----------------------------------------------------------------------------
void Button::set_text(
    std::string_view const text
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_text from \"" << _text->getString().toAnsiString() << "\" to \"" << text << '\"';

        _text->setString(text);
    }
}

// ----------------------------------------------------------------------------
void Button::set_text_color(
    sf::Color const color
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_text_color from " <<
        _text->getFillColor().toInteger() << " to " << color.toInteger();

        _text->setFillColor(color);
    }
}

// ----------------------------------------------------------------------------
void Button::set_text_border(
    Widget_border const border
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_text_border from (color=" <<
        _text->getOutlineColor().toInteger() << ", size=" <<
        _text->getOutlineThickness() << ") to (color=" << border.color.toInteger() <<
        ", size=" << border.size << ')';

        _text->setOutlineColor(border.color);
        _text->setOutlineThickness(border.size);
    }
}

// ----------------------------------------------------------------------------
void Button::set_text_char_size(
    std::size_t const size
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << "_text == nullptr";
    } else {

        LOG(Log_lvl::TRACE) << "set_text_char_size from " << _text->getCharacterSize() << " to " << size;

        _text->setCharacterSize(size);
    }
}

// ----------------------------------------------------------------------------
void Button::set_visible(
    bool const visible
    ) {

    _set_flag(State::HIDDEN, !visible);
}

}
