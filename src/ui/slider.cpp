// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/slider.hpp"
#include "resources/include/asset_registry.hpp"
#include "resources/include/resource_manager.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"
#include "utils/include/math.hpp"

#include <algorithm>
#include <cmath>
#include <sstream>
#include <iomanip>

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
// Class Slider
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Slider::Slider(
    sf::RenderWindow& window,
    std::string_view const name
    )
    : Widget(window, Widget_data{ .name = name, .type = Widget_type::SLIDER })
    , _state(State::DEFAULT)
    , _text(nullptr)
    , _rect_outside(nullptr)
    , _rect_inside(nullptr)
    , _handle(nullptr)
    , _font(nullptr)
    , _on_drag()
    , _on_exit_drag()
    , _on_hover()
    , _on_exit_hover()
    , _on_value_changed()
    , _show_value_text(true)
    , _text_offset(0.0f, 0.0f)
    , _text_decimal_precision(0u)
    , _min_value(0.0f)
    , _max_value(100.0f)
    , _value(0.0f)
    {

    auto const font = Asset_registry::load_font("pixel_bold");

    if (font == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": font pixel_bold == nullptr";
    } else {

        _font = std::move(font);
    }

    _create__text();
    _create__rect_outside();
    _create__rect_inside();
    _create__handle();

    _update_handle_from_value();

    _register_events();

    LOG(Log_lvl::TRACE) << "Created slider " << _data.name;
}

// ----------------------------------------------------------------------------
Slider::~Slider() {

    auto& sfml_event_manager = SFML_event_manager::instance();

    sfml_event_manager.deregister_listener(listener_id());
}

// ----------------------------------------------------------------------------
void Slider::draw() {

    if (_has_flag(State::HIDDEN)) {

        // Not visible so don't draw
    } else {

        if (_rect_outside != nullptr) { _window.draw(*_rect_outside); }
        if (_rect_inside != nullptr) { _window.draw(*_rect_inside); }
        if (_handle != nullptr) { _window.draw(*_handle); }
        if (_show_value_text && _text != nullptr) { _window.draw(*_text); }
    }
}

// ----------------------------------------------------------------------------
void Slider::update(
    float const dt
    ) {

    // Do nothing
    (void) dt;
}

// ----------------------------------------------------------------------------
void Slider::enable(
    bool const enable
    ) {

    _set_flag(State::DISABLED, !enable);
}

// ----------------------------------------------------------------------------
void Slider::_create__text() {

    _text = std::make_unique<sf::Text>(*_font, "");
    set_text_color(colors::DEFAULT_TEXT);
}

// ----------------------------------------------------------------------------
void Slider::_create__rect_outside() {

    _rect_outside = std::make_unique<sf::RectangleShape>(sf::Vector2f{ 200.0f, 20.0f });
    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr";
    } else {

        _rect_outside->setFillColor(colors::DEFAULT_CONTAINER_COLOR);
        _rect_outside->setOutlineColor(colors::DEFAULT_BORDER);
        _rect_outside->setOutlineThickness(0.0f);
    }
}

// ----------------------------------------------------------------------------
void Slider::_create__rect_inside() {

    _rect_inside = std::make_unique<sf::RectangleShape>(sf::Vector2f{ 0.0f, 8.0f });
    if (_rect_inside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_inside == nullptr";
    } else {

        _rect_inside->setFillColor(colors::DEFAULT_FILL_COLOR);
    }
}

// ----------------------------------------------------------------------------
void Slider::_create__handle() {

    _handle = std::make_unique<sf::CircleShape>(10.0f);
    if (_handle == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _handle == nullptr";
    } else {

        _handle->setOrigin(sf::Vector2f{ 10.0f, 10.0f });
        _handle->setFillColor(colors::DEFAULT_CONTAINER_COLOR);
        _handle->setOutlineColor(colors::DEFAULT_BORDER);
        _handle->setOutlineThickness(0.0f);
    }
}

// ----------------------------------------------------------------------------
void Slider::_register_events() {

    auto& sfml_event_manager = SFML_event_manager::instance();

    sfml_event_manager.register_callback(
        SFML_event_type::MOUSE_BUTTON_LEFT_PRESS,
        [this](SFML_event_data const&){ _handle_event__mouse_button_left_press(); },
        listener_id()
        );

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
void Slider::_handle_event__mouse_button_left_press() {

    if (_has_flag(State::DISABLED)) {

        return;
    }

    if (is_hovering()) {

        _set_flag(State::DRAGGING, true);

        LOG(Log_lvl::TRACE) << _data.name << ": started dragging";

        float const mouse_pos_x = _get_mouse_world_pos().x;

        // Snaps the handle straight to the click position, whether the
        // click landed on the handle itself or elsewhere on the track
        _update_value_from_mouse_x(mouse_pos_x);

        if (_on_drag) {

            _on_drag();
        }
    }
}

// ----------------------------------------------------------------------------
void Slider::_handle_event__mouse_button_left_release() {

    if (_has_flag(State::DRAGGING)) {

        _set_flag(State::DRAGGING, false);

        LOG(Log_lvl::TRACE) << _data.name << ": stopped dragging";

        if (_on_exit_drag) {

            _on_exit_drag();
        }
    }
}

// ----------------------------------------------------------------------------
void Slider::_handle_event__mouse_moved() {

    if (_has_flag(State::DISABLED)) {

        return;
    }

    if (_has_flag(State::DRAGGING)) {

        float const mouse_pos_x = _get_mouse_world_pos().x;

        _update_value_from_mouse_x(mouse_pos_x);

        // Keep the hovering effects active while dragging for
        // better user experience
        return;
    }

    if (_has_flag(State::HOVERING)) {

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
void Slider::_update_handle_from_value() {

    if (_rect_outside == nullptr || _rect_inside == nullptr || _handle == nullptr) {

        return;
    }

    float const ratio = math::inverse_lerp(
        _min_value,
        _max_value,
        _value
        );

    sf::FloatRect const track = _get_track_bounds();

    _rect_inside->setPosition(track.position);
    _rect_inside->setSize({
        track.size.x * ratio,
        track.size.y
        });

    _handle->setPosition({
        track.position.x + (track.size.x * ratio),
        track.position.y + (track.size.y * 0.5f)
        });

    if (_text != nullptr) {

        std::ostringstream stream;
        stream << std::fixed << std::setprecision(_text_decimal_precision) << _value;

        _text->setString(stream.str());
        _update_text_pos();
    }
}

// ----------------------------------------------------------------------------
void Slider::_update_text_pos() {

    if (_rect_outside == nullptr || _text == nullptr) {

        return;
    }

    sf::FloatRect const track = _get_track_bounds();
    sf::Vector2f const slider_center{
        track.position.x + (track.size.x * 0.5f),
        track.position.y + (track.size.y * 0.5f)
        };

    sf::FloatRect const text_bounds = _text->getLocalBounds();
    _text->setOrigin({
        text_bounds.position.x + (text_bounds.size.x * 0.5f),
        text_bounds.position.y + (text_bounds.size.y * 0.5f)
        });

    _text->setPosition({
        slider_center.x + _text_offset.x,
        slider_center.y + _text_offset.y
        });
}

// ----------------------------------------------------------------------------
void Slider::_update_value_from_mouse_x(
    float const mouse_x
    ) {

    if (_rect_outside == nullptr) {

        return;
    }

    sf::FloatRect const track = _get_track_bounds();

    float const ratio = math::inverse_lerp(
        track.position.x,
        track.position.x + track.size.x,
        mouse_x
        );

    float const new_value = math::lerp(_min_value, _max_value, ratio);

    set_value(new_value);
}

// ----------------------------------------------------------------------------
sf::Vector2f Slider::_get_mouse_world_pos() const {

    sf::Vector2i const mouse_pixel_pos = sf::Mouse::getPosition(_window);
    sf::Vector2f const mouse_coords = _window.mapPixelToCoords(mouse_pixel_pos);

    return mouse_coords;
}

// ----------------------------------------------------------------------------
sf::FloatRect Slider::_get_track_bounds() const {

    if (_rect_outside == nullptr) {

        return {};
    } else {

        return _rect_outside->getGlobalBounds();
    }
}

// ----------------------------------------------------------------------------
std::string Slider::_state_to_string(
    std::uint8_t const state
    ) {

    if (state == State::NONE) { return "NONE"; }

    std::string result;

    if (state & State::HOVERING) { result += "HOVERING|"; }
    if (state & State::DISABLED) { result += "DISABLED|"; }
    if (state & State::HIDDEN) { result += "HIDDEN|"; }
    if (state & State::DRAGGING) { result += "DRAGGING|"; }

    if (!result.empty()) { result.pop_back(); }

    return result;
}

// ----------------------------------------------------------------------------
void Slider::_set_flag(
    State const flag,
    bool const value
    ) {

    std::uint8_t const new_state = value ? (_state | flag) : (_state & ~flag);

    if (new_state == _state) { return; }

    LOG(Log_lvl::TRACE) << _data.name << ": changing state from " <<
    _state_to_string(_state) << " to " << _state_to_string(new_state);

    _state = new_state;
}

// ----------------------------------------------------------------------------
bool Slider::_has_flag(
    State const flag
    ) const {

    return (_state & flag) != 0;
}

// ----------------------------------------------------------------------------
sf::Vector2f Slider::get_pos() const {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't get pos.";

        return {};
    } else {

        return _rect_outside->getPosition();
    }
}

// ----------------------------------------------------------------------------
sf::Vector2f Slider::get_scale() const {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't get scale.";

        return {};
    } else {

        return _rect_outside->getScale();
    }
}

// ----------------------------------------------------------------------------
sf::Vector2f Slider::get_size() const {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't get size.";

        return {};
    } else {

        return _rect_outside->getSize();
    }
}

// ----------------------------------------------------------------------------
float Slider::get_value() const {

    return _value;
}

// ----------------------------------------------------------------------------
float Slider::get_min_value() const {

    return _min_value;
}

// ----------------------------------------------------------------------------
float Slider::get_max_value() const {

    return _max_value;
}

// ----------------------------------------------------------------------------
float Slider::get_handle_radius() const {

    if (_handle == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _handle == nullptr, can't get handle radius.";

        return {};
    } else {

        return _handle->getRadius();
    }
}

// ----------------------------------------------------------------------------
sf::Vector2f Slider::get_text_offset() const {

    return _text_offset;
}

// ----------------------------------------------------------------------------
std::size_t Slider::get_text_decimal_precision() const {

    return _text_decimal_precision;
}

// ----------------------------------------------------------------------------
bool Slider::is_enabled() const {

    return !_has_flag(State::DISABLED);
}

// ----------------------------------------------------------------------------
bool Slider::is_visible() const {

    return !_has_flag(State::HIDDEN);
}

// ----------------------------------------------------------------------------
bool Slider::is_hovering() const {

    if (_handle == nullptr || _rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _handle or _rect_outside == nullptr, can't check hovering.";
        return false;
    } else {

        sf::Vector2f const mouse_pos = _get_mouse_world_pos();

        return _handle->getGlobalBounds().contains(mouse_pos)
            || _rect_outside->getGlobalBounds().contains(mouse_pos);
    }
}

// ----------------------------------------------------------------------------
bool Slider::is_dragging() const {

    return _has_flag(State::DRAGGING);
}

// ----------------------------------------------------------------------------
void Slider::set_pos(
    sf::Vector2f const pos
    ) {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't set pos.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_pos from (" << _rect_outside->getPosition().x << ", "
        << _rect_outside->getPosition().y << ") to (" << pos.x << ", " << pos.y << ')';

        _rect_outside->setPosition(pos);

        _update_handle_from_value();
    }
}

// ----------------------------------------------------------------------------
void Slider::set_scale(
    sf::Vector2f const scale
    ) {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't set scale.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_scale from (" << _rect_outside->getScale().x
        << ", " << _rect_outside->getScale().y << ") to (" << scale.x << ", " << scale.y << ')';

        _rect_outside->setScale(scale);

        if (_rect_inside != nullptr) { _rect_inside->setScale(scale); }
        if (_handle != nullptr) { _handle->setScale(scale); }
        if (_text != nullptr) { _text->setScale(scale); }
    }
}

// ----------------------------------------------------------------------------
void Slider::set_size(
    sf::Vector2f const size
    ) {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't set size.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_size to (" << size.x << ", " << size.y << ')';

        _rect_outside->setSize(size);

        _update_handle_from_value();
    }
}

// ----------------------------------------------------------------------------
void Slider::set_on_drag(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_drag";

    _on_drag = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Slider::set_on_exit_drag(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_exit_drag";

    _on_exit_drag = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Slider::set_on_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_hover";

    _on_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Slider::set_on_exit_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_exit_hover";

    _on_exit_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Slider::set_on_value_changed(
    std::function<void(float const)> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_value_changed";

    _on_value_changed = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Slider::set_font(
    std::string_view const font_name
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _text == nullptr, can't set font.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_font to " << font_name;

        auto const font = Asset_registry::load_font(font_name);

        if (font == nullptr) {

            LOG(Log_lvl::WARN) << _data.name << ": font " << font_name << " == nullptr";
        } else {

            _font = std::move(font);
            _text->setFont(*_font);
        }
    }
}

// ----------------------------------------------------------------------------
void Slider::set_text_color(
    sf::Color const color
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _text == nullptr, can't set text color.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_text_color from " <<
        _text->getFillColor().toInteger() << " to " << color.toInteger();

        _text->setFillColor(color);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_text_border(
    Widget_border const border
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _text == nullptr, can't set text border.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_text_border from (color=" <<
        _text->getOutlineColor().toInteger() << ", size=" <<
        _text->getOutlineThickness() << ") to (color=" << border.color.toInteger() <<
        ", size=" << border.size << ')';

        _text->setOutlineColor(border.color);
        _text->setOutlineThickness(border.size);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_text_char_size(
    std::size_t const size
    ) {

    if (_text == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _text == nullptr, can't set text char size.";
    } else {

        LOG(Log_lvl::TRACE) << _data.name << ": set_text_char_size from " << _text->getCharacterSize() << " to " << size;

        _text->setCharacterSize(size);

        _update_text_pos();
    }
}

// ----------------------------------------------------------------------------
void Slider::set_text_offset(
    sf::Vector2f const offset
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_text_offset from (" << _text_offset.x
    << ", " << _text_offset.y << ") to (" << offset.x << ", " << offset.y << ')';

    _text_offset = offset;

    _update_text_pos();
}

// ----------------------------------------------------------------------------
void Slider::set_show_value_text(
    bool const show
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_show_value_text to " << show;

    _show_value_text = show;
}

// ----------------------------------------------------------------------------
void Slider::set_text_decimal_precision(
    std::size_t const precision
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_text_decimal_precision from "
    << _text_decimal_precision << " to " << precision;

    _text_decimal_precision = precision;

    _update_handle_from_value();
}

// ----------------------------------------------------------------------------
void Slider::set_track_color(
    sf::Color const color
    ) {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't set track color.";
    } else {

        _rect_outside->setFillColor(color);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_track_border(
    Widget_border const border
    ) {

    if (_rect_outside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_outside == nullptr, can't set track border.";
    } else {

        _rect_outside->setOutlineColor(border.color);
        _rect_outside->setOutlineThickness(border.size);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_fill_color(
    sf::Color const color
    ) {

    if (_rect_inside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_inside == nullptr, can't set fill color.";
    } else {

        _rect_inside->setFillColor(color);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_fill_border(
    Widget_border const border
    ) {

    if (_rect_inside == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _rect_inside == nullptr, can't set fill border.";
    } else {

        _rect_inside->setOutlineColor(border.color);
        _rect_inside->setOutlineThickness(border.size);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_handle_radius(
    float const radius
    ) {

    if (_handle == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _handle == nullptr, can't set handle radius.";
    } else {

        _handle->setRadius(radius);
        _handle->setOrigin({ radius, radius });

        _update_handle_from_value();
    }
}

// ----------------------------------------------------------------------------
void Slider::set_handle_color(
    sf::Color const color
    ) {

    if (_handle == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _handle == nullptr, can't set handle color.";
    } else {

        _handle->setFillColor(color);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_handle_border(
    Widget_border const border
    ) {

    if (_handle == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": _handle == nullptr, can't set handle border.";
    } else {

        _handle->setOutlineColor(border.color);
        _handle->setOutlineThickness(border.size);
    }
}

// ----------------------------------------------------------------------------
void Slider::set_visible(
    bool const visible
    ) {

    _set_flag(State::HIDDEN, !visible);
}

// ----------------------------------------------------------------------------
void Slider::set_range(
    float const min,
    float const max
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_range from (" << _min_value
    << ", " << _max_value << ") to (" << min << ", " << max << ')';

    _min_value = min;
    _max_value = max;

    float const clamped_value = std::clamp(_value, _min_value, _max_value);

    set_value(clamped_value);
}

// ----------------------------------------------------------------------------
void Slider::set_value(
    float const val
    ) {

    float const clamped_value = std::clamp(val, _min_value, _max_value);

    if (clamped_value == _value) {

        return;
    }

    LOG(Log_lvl::TRACE) << _data.name << ": set_value from " << _value << " to " << clamped_value;

    _value = clamped_value;

    _update_handle_from_value();

    if (_on_value_changed) {

        _on_value_changed(_value);
    }
}

}
