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
using namespace battleships::event;
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

    if (_state == State::DISABLED) {

        // Disabled so don't draw
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

    bool const is_the_btn_enabled = this->is_enabled();

    if (enable && is_the_btn_enabled) {

        // Already enabled
    } else if (enable && !is_the_btn_enabled) {

        _change_state(State::ENABLED_NOT_HOVERING);
    } else if (!enable && is_the_btn_enabled) {

        _change_state(State::DISABLED);
    } else {

        // Already disabled
    }
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

    if (_state != State::DISABLED && is_hovering() && _on_left_click) {

        LOG(Log_lvl::TRACE) << "Button left clicked";

        _on_left_click();
    }
}

// ----------------------------------------------------------------------------
void Button::_handle_event__mouse_moved() {

    if (_state == State::DISABLED) {

        // Do nothing
    } else if (_state == State::ENABLED_HOVERING) {

        // Already in hovering state so check if we are no longer hovering anymore
        if (!is_hovering() && _on_exit_hover) {

            _change_state(State::ENABLED_NOT_HOVERING);
            _on_exit_hover();
        }
    } else {

        // Not in hovering state so check if we are now hovering
        if (is_hovering() && _on_hover) {

            _change_state(State::ENABLED_HOVERING);
            _on_hover();
        }
    }
}

// ----------------------------------------------------------------------------
std::string_view Button::_to_string(
    State const state
    ) {

    switch (state) {

        case State::DISABLED: return "DISABLED";
        case State::ENABLED_HOVERING: return "ENABLED_HOVERING";
        case State::ENABLED_NOT_HOVERING: return "ENABLED_NOT_HOVERING";

        default: return "UNKNOWN";
    }
}

// ----------------------------------------------------------------------------
void Button::_change_state(
    State const state
    ) {

    LOG(Log_lvl::TRACE) << "Changing button state from " <<
    _to_string(_state) << " to " << _to_string(state);

    _state = state;
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

    return (_state == State::ENABLED_HOVERING) || (_state == State::ENABLED_NOT_HOVERING);
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

}
