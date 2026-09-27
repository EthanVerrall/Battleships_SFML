// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/hotkey_recorder.hpp"
#include "resources/include/asset_registry.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"
#include "utils/include/null_check.hpp"

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
// Class Hotkey_recorder
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Hotkey_recorder::Hotkey_recorder(
    sf::RenderWindow& window,
    std::string_view const name
    )
    :
    Widget(window, Widget_data{ .name = name, .type = Widget_type::HOTKEY_RECORDER })
    , _state(State::DEFAULT)
    , _rect(nullptr)
    , _text(nullptr)
    , _font(nullptr)
    , _key(sf::Keyboard::Key::Unknown)
    , _text_padding(6.0f)
    , _on_key_changed()
    , _on_hover()
    , _on_exit_hover()
    {

    auto const font = Asset_registry::load_font("pixel_bold");

    if (font == nullptr) {

        LOG(Log_lvl::WARN) << _data.name << ": font pixel_bold == nullptr";
    } else {

        _font = std::move(font);
    }

    _create__rect();
    _create__text();

    _update_label();

    _register_events();

    LOG(Log_lvl::TRACE) << "Created hotkey recorder " << _data.name;
}

// ----------------------------------------------------------------------------
Hotkey_recorder::~Hotkey_recorder() {

    auto& sfml_event_manager = SFML_event_manager::instance();

    sfml_event_manager.deregister_listener(listener_id());
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::draw() {

    if (_has_flag(State::HIDDEN)) {

        // Not visible so don't draw
    } else {

        if (_rect != nullptr) { _window.draw(*_rect); }
        if (_text != nullptr) { _window.draw(*_text); }
    }
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::update(
    float const dt
    ) {

    // Do nothing
    (void) dt;
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::enable(
    bool const enable
    ) {

    _set_flag(State::DISABLED, !enable);

    if (!enable) {

        _set_flag(State::RECORDING, false);
        _update_label();
    }
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_create__rect() {

    _rect = std::make_unique<sf::RectangleShape>(sf::Vector2f{ 64.0f, 32.0f });

    NULL_CHECK_VOID(_rect)

    _rect->setFillColor(colors::DEFAULT_CONTAINER_COLOR);
    _rect->setOutlineColor(colors::DEFAULT_BORDER);
    _rect->setOutlineThickness(0.0f);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_create__text() {

    NULL_CHECK_VOID(_font)

    _text = std::make_unique<sf::Text>(*_font, "");
    _text->setFillColor(colors::DEFAULT_TEXT);
    _text->setCharacterSize(18u);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_register_events() {

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

    for (
        std::uint16_t type_value = static_cast<std::uint16_t>(SFML_event_type::KEYPRESS_A);
        type_value <= static_cast<std::uint16_t>(SFML_event_type::KEYPRESS_PAUSE);
        ++type_value
        ) {

        sfml_event_manager.register_callback(
            static_cast<SFML_event_type>(type_value),
            [this](SFML_event_data const& data){ _handle_event__key_pressed(data); },
            listener_id()
            );
    }
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_handle_event__mouse_button_left_release() {

    if (!_has_flag(State::DISABLED) && is_hovering()) {

        LOG(Log_lvl::TRACE) << _data.name << ": left clicked, entering recording state";

        _set_flag(State::RECORDING, true);

        _update_label();
    }
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_handle_event__mouse_moved() {

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
void Hotkey_recorder::_handle_event__key_pressed(
    SFML_event_data const& data
    ) {

    if (!_has_flag(State::RECORDING)) {

        return;
    }

    auto const* key_pressed = data.sfml_event.getIf<sf::Event::KeyPressed>();

    if (key_pressed == nullptr) {

        return;
    }

    LOG(Log_lvl::TRACE) << _data.name << ": recorded key " << _key_to_string(key_pressed->code);

    _key = key_pressed->code;

    _set_flag(State::RECORDING, false);

    _update_label();

    if (_on_key_changed) {

        _on_key_changed(_key);
    }
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_update_label() {

    NULL_CHECK_VOID(_text)

    if (_has_flag(State::RECORDING)) {

        _text->setString("...");
    } else {

        _text->setString(_key_to_string(_key));
    }

    _fit_text_to_box();
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_fit_text_to_box() {

    NULL_CHECK_VOID(_rect)
    NULL_CHECK_VOID(_text)

    sf::Vector2f const box_size = _rect->getSize();
    float const max_width = box_size.x - (_text_padding * 2.0f);
    float const max_height = box_size.y - (_text_padding * 2.0f);

    unsigned int char_size = 100u; // Some high value

    _text->setCharacterSize(char_size);

    while (

        (char_size > 1u) &&
        ((_text->getGlobalBounds().size.x > max_width) ||
         (_text->getGlobalBounds().size.y > max_height)
        )
        ) {

        --char_size;
        _text->setCharacterSize(char_size);
    }

    sf::FloatRect const local_bounds = _text->getLocalBounds();
    sf::Vector2f const scale = _text->getScale();
    sf::Vector2f const box_center = _rect->getPosition() + (box_size * 0.5f);

    sf::Vector2f const ink_half_size{
        (local_bounds.size.x * scale.x) * 0.5f,
        (local_bounds.size.y * scale.y) * 0.5f
        };

    sf::Vector2f const ink_offset{
        local_bounds.position.x * scale.x,
        local_bounds.position.y * scale.y
        };

    _text->setPosition(box_center - ink_offset - ink_half_size);
}

// ----------------------------------------------------------------------------
std::string Hotkey_recorder::_key_to_string(
    sf::Keyboard::Key const key
    ) const {

    if (key == sf::Keyboard::Key::Unknown) {

        return "None";
    }

    sf::Keyboard::Scancode const scancode = sf::Keyboard::delocalize(key);

    if (scancode == sf::Keyboard::Scan::Unknown) {

        return "Unknown";
    }

    return sf::Keyboard::getDescription(scancode).toAnsiString();
}

// ----------------------------------------------------------------------------
std::string Hotkey_recorder::_state_to_string(
    std::uint8_t const state
    ) {

    if (state == State::NONE) { return "NONE"; }

    std::string result;

    if (state & State::HOVERING) { result += "HOVERING|"; }
    if (state & State::DISABLED) { result += "DISABLED|"; }
    if (state & State::HIDDEN) { result += "HIDDEN|"; }
    if (state & State::RECORDING) { result += "RECORDING|"; }

    if (!result.empty()) { result.pop_back(); }

    return result;
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::_set_flag(
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
bool Hotkey_recorder::_has_flag(
    State const flag
    ) const {

    return (_state & flag) != 0;
}

// ----------------------------------------------------------------------------
sf::Vector2f Hotkey_recorder::get_pos() const {

    NULL_CHECK(_rect)

    return _rect->getPosition();
}

// ----------------------------------------------------------------------------
sf::Vector2f Hotkey_recorder::get_size() const {

    NULL_CHECK(_rect)

    return _rect->getGlobalBounds().size;
}

// ----------------------------------------------------------------------------
sf::Vector2f Hotkey_recorder::get_scale() const {

    NULL_CHECK(_rect)

    return _rect->getScale();
}

// ----------------------------------------------------------------------------
sf::Color Hotkey_recorder::get_color() const {

    NULL_CHECK(_rect)

    return _rect->getFillColor();
}

// ----------------------------------------------------------------------------
Widget_border Hotkey_recorder::get_border() const {

    NULL_CHECK(_rect)

    return Widget_border{
        .color = _rect->getOutlineColor(),
        .size = _rect->getOutlineThickness()
        };
}

// ----------------------------------------------------------------------------
sf::Keyboard::Key Hotkey_recorder::get_key() const {

    return _key;
}

// ----------------------------------------------------------------------------
bool Hotkey_recorder::is_enabled() const {

    return !_has_flag(State::DISABLED);
}

// ----------------------------------------------------------------------------
bool Hotkey_recorder::is_hovering() const {

    NULL_CHECK(_rect)

    sf::Vector2i const mouse_pixel_pos = sf::Mouse::getPosition(_window);
    sf::Vector2f const mouse_world_pos = _window.mapPixelToCoords(mouse_pixel_pos);

    return _rect->getGlobalBounds().contains(mouse_world_pos);
}

// ----------------------------------------------------------------------------
bool Hotkey_recorder::is_visible() const {

    return !_has_flag(State::HIDDEN);
}

// ----------------------------------------------------------------------------
bool Hotkey_recorder::is_recording() const {

    return _has_flag(State::RECORDING);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_pos(
    sf::Vector2f const pos
    ) {

    NULL_CHECK_VOID(_rect)

    LOG(Log_lvl::TRACE) << _data.name << ": set_pos from (" << _rect->getPosition().x << ", "
    << _rect->getPosition().y << ") to (" << pos.x << ", " << pos.y << ')';

    _rect->setPosition(pos);

    _fit_text_to_box();
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_size(
    sf::Vector2f const size
    ) {

    NULL_CHECK_VOID(_rect)

    LOG(Log_lvl::TRACE) << _data.name << ": set_size to (" << size.x << ", " << size.y << ')';

    _rect->setSize(size);

    _fit_text_to_box();
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_scale(
    sf::Vector2f const scale
    ) {

    NULL_CHECK_VOID(_rect)

    LOG(Log_lvl::TRACE) << _data.name << ": set_scale from (" << _rect->getScale().x
    << ", " << _rect->getScale().y << ") to (" << scale.x << ", " << scale.y << ')';

    _rect->setScale(scale);

    if (_text != nullptr) { _text->setScale(scale); }

    _fit_text_to_box();
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_color(
    sf::Color const color
    ) {

    NULL_CHECK_VOID(_rect)

    _rect->setFillColor(color);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_border(
    Widget_border const border
    ) {

    NULL_CHECK_VOID(_rect)

    _rect->setOutlineColor(border.color);
    _rect->setOutlineThickness(border.size);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_text_color(
    sf::Color const color
    ) {

    NULL_CHECK_VOID(_text)

    _text->setFillColor(color);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_key(
    sf::Keyboard::Key const key
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_key to " << _key_to_string(key);

    _key = key;

    _set_flag(State::RECORDING, false);

    _update_label();
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_on_key_changed(
    std::function<void(sf::Keyboard::Key const)> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_key_changed";

    _on_key_changed = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_on_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_hover";

    _on_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_on_exit_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_exit_hover";

    _on_exit_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Hotkey_recorder::set_visible(
    bool const visible
    ) {

    _set_flag(State::HIDDEN, !visible);
}

}
