// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/checkbox.hpp"
#include "resources/include/asset_registry.hpp"
#include "resources/include/resource_manager.hpp"
#include "events/include/sfml_event_manager.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"

// ============================================================================
// Macros
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
#define NULL_CHECK(obj)                           \
    if ((obj) == nullptr) {                       \
                                                  \
        LOG(Log_lvl::WARN) <<  _data.name << ": " \
        << #obj << " == nullptr";                 \
                                                  \
        return {};                                \
    }

// ----------------------------------------------------------------------------
#define NULL_CHECK_VOID(obj)                      \
    if ((obj) == nullptr) {                       \
                                                  \
        LOG(Log_lvl::WARN) <<  _data.name << ": " \
        << #obj << " == nullptr";                 \
                                                  \
        return;                                   \
    }

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
// Class Checkbox
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Checkbox::Checkbox(
    sf::RenderWindow& window,
    std::string_view const name
    )
    :
    Widget(window, Widget_data{ .name = name, .type = Widget_type::CHECKBOX })
    , _state(State::DEFAULT)
    , _rect(nullptr)
    , _checkmark_piece_left(nullptr)
    , _checkmark_piece_right(nullptr)
    , _on_hover()
    , _on_exit_hover()
    {

    _create__rect();
    _create__checkmark();

    _register_events();

    LOG(Log_lvl::TRACE) << "Created checkbox " << _data.name;
}

// ----------------------------------------------------------------------------
Checkbox::~Checkbox() {

    auto& sfml_event_manager = SFML_event_manager::instance();

    sfml_event_manager.deregister_listener(listener_id());
}

// ----------------------------------------------------------------------------
void Checkbox::draw() {

    if (_has_flag(State::HIDDEN)) {

        // Not visible so don't draw
    } else {

        if (_rect != nullptr) {

            _window.draw(*_rect);
        }

        if ((_checkmark_piece_left != nullptr) && _has_flag(State::CHECKED)) {

            _window.draw(*_checkmark_piece_left);
        }

        if ((_checkmark_piece_right != nullptr) && _has_flag(State::CHECKED)) {

            _window.draw(*_checkmark_piece_right);
        }
    }
}

// ----------------------------------------------------------------------------
void Checkbox::update(
    float const dt
    ) {

    // Do nothing
    (void) dt;
}

// ----------------------------------------------------------------------------
void Checkbox::enable(
    bool const enable
    ) {

    _set_flag(State::DISABLED, !enable);
}

// ----------------------------------------------------------------------------
void Checkbox::_create__rect() {

    _rect = std::make_unique<sf::RectangleShape>(sf::Vector2f{ 24.0f, 24.0f });

    NULL_CHECK_VOID(_rect)

    _rect->setFillColor(colors::DEFAULT_CONTAINER_COLOR);
    _rect->setOutlineColor(colors::DEFAULT_BORDER);
    _rect->setOutlineThickness(0.0f);
}

// ----------------------------------------------------------------------------
void Checkbox::_create__checkmark() {

    _checkmark_piece_left = std::make_unique<sf::RectangleShape>(sf::Vector2f{ 16.0f, 4.0f });

    NULL_CHECK_VOID(_checkmark_piece_left)

    _checkmark_piece_left->setOrigin(sf::Vector2f{ 8.0f, 2.0f });
    _checkmark_piece_left->setRotation(sf::degrees(45.0f));
    _checkmark_piece_left->setFillColor(colors::DEFAULT_FILL_COLOR);

    _checkmark_piece_right = std::make_unique<sf::RectangleShape>(sf::Vector2f{ 16.0f, 4.0f });

    NULL_CHECK_VOID(_checkmark_piece_right)

    _checkmark_piece_right->setOrigin(sf::Vector2f{ 8.0f, 2.0f });
    _checkmark_piece_right->setRotation(sf::degrees(-45.0f));
    _checkmark_piece_right->setFillColor(colors::DEFAULT_FILL_COLOR);

    _update_checkmark_pos();
}

// ----------------------------------------------------------------------------
void Checkbox::_update_checkmark_pos() {

    NULL_CHECK_VOID(_rect)

    sf::FloatRect const bounds = _rect->getGlobalBounds();
    sf::Vector2f const center{
        bounds.position.x + (bounds.size.x * 0.5f),
        bounds.position.y + (bounds.size.y * 0.5f)
        };

    NULL_CHECK_VOID(_checkmark_piece_left)
    NULL_CHECK_VOID(_checkmark_piece_right)

    _checkmark_piece_left->setPosition(center);
    _checkmark_piece_right->setPosition(center);
}

// ----------------------------------------------------------------------------
void Checkbox::_register_events() {

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
void Checkbox::_handle_event__mouse_button_left_release() {

    if (!_has_flag(State::DISABLED) && is_hovering()) {

        LOG(Log_lvl::TRACE) << _data.name << ": left clicked";

        if (_has_flag(State::CHECKED)) {

            // Uncheck
            _set_flag(State::CHECKED, false);
        } else {

            // Check
            _set_flag(State::CHECKED, true);
        }
    }
}

// ----------------------------------------------------------------------------
void Checkbox::_handle_event__mouse_moved() {

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
std::string Checkbox::_state_to_string(
    std::uint8_t const state
    ) {

    if (state == State::NONE) { return "NONE"; }

    std::string result;

    if (state & State::HOVERING) { result += "HOVERING|"; }
    if (state & State::DISABLED) { result += "DISABLED|"; }
    if (state & State::HIDDEN) { result += "HIDDEN|"; }
    if (state & State::CHECKED) { result += "CHECKED|"; }

    if (!result.empty()) { result.pop_back(); }

    return result;
}

// ----------------------------------------------------------------------------
void Checkbox::_set_flag(
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
bool Checkbox::_has_flag(
    State const flag
    ) const {

    return (_state & flag) != 0;
}

// ----------------------------------------------------------------------------
sf::Vector2f Checkbox::get_pos() const {

    NULL_CHECK(_rect)

    return _rect->getPosition();
}

// ----------------------------------------------------------------------------
sf::Vector2f Checkbox::get_size() const {

    NULL_CHECK(_rect)

    return _rect->getGlobalBounds().size;
}

// ----------------------------------------------------------------------------
sf::Vector2f Checkbox::get_scale() const {

    NULL_CHECK(_rect)

    return _rect->getScale();
}

// ----------------------------------------------------------------------------
sf::Color Checkbox::get_color() const {

    NULL_CHECK(_rect)

    return _rect->getFillColor();
}

// ----------------------------------------------------------------------------
Widget_border Checkbox::get_border() const {

    NULL_CHECK(_rect)

    return Widget_border{
        .color = _rect->getOutlineColor(),
        .size = _rect->getOutlineThickness()
        };
}

// ----------------------------------------------------------------------------
sf::Color Checkbox::get_checkmark_color() const {

    NULL_CHECK(_checkmark_piece_left)
    NULL_CHECK(_checkmark_piece_right)

    return _checkmark_piece_left->getFillColor();
}

// ----------------------------------------------------------------------------
sf::Vector2f Checkbox::get_checkmark_size() const {

    NULL_CHECK(_checkmark_piece_left)
    NULL_CHECK(_checkmark_piece_right)

    return _checkmark_piece_left->getSize();
}

// ----------------------------------------------------------------------------
Widget_border Checkbox::get_checkmark_border() const {

    NULL_CHECK(_checkmark_piece_left)
    NULL_CHECK(_checkmark_piece_right)

    return Widget_border{
        .color = _checkmark_piece_left->getOutlineColor(),
        .size = _checkmark_piece_left->getOutlineThickness()
        };
}

// ----------------------------------------------------------------------------
bool Checkbox::is_enabled() const {

    return !_has_flag(State::DISABLED);
}

// ----------------------------------------------------------------------------
bool Checkbox::is_hovering() const {

    NULL_CHECK(_rect)

    sf::Vector2i const mouse_pixel_pos = sf::Mouse::getPosition(_window);
    sf::Vector2f const mouse_world_pos = _window.mapPixelToCoords(mouse_pixel_pos);

    return _rect->getGlobalBounds().contains(mouse_world_pos);
}

// ----------------------------------------------------------------------------
bool Checkbox::is_visible() const {

    return !_has_flag(State::HIDDEN);
}

// ----------------------------------------------------------------------------
bool Checkbox::is_checked() const {

    return _has_flag(State::CHECKED);
}

// ----------------------------------------------------------------------------
void Checkbox::set_pos(
    sf::Vector2f const pos
    ) {

    NULL_CHECK_VOID(_rect)

    LOG(Log_lvl::TRACE) << _data.name << ": set_pos from (" << _rect->getPosition().x << ", "
    << _rect->getPosition().y << ") to (" << pos.x << ", " << pos.y << ')';

    _rect->setPosition(pos);

    _update_checkmark_pos();
}

// ----------------------------------------------------------------------------
void Checkbox::set_size(
    sf::Vector2f const size
    ) {

    NULL_CHECK_VOID(_rect)

    LOG(Log_lvl::TRACE) << _data.name << ": set_size to (" << size.x << ", " << size.y << ')';

    _rect->setSize(size);

    _update_checkmark_pos();
}

// ----------------------------------------------------------------------------
void Checkbox::set_scale(
    sf::Vector2f const scale
    ) {

    NULL_CHECK_VOID(_rect)

    LOG(Log_lvl::TRACE) << _data.name << ": set_scale from (" << _rect->getScale().x
    << ", " << _rect->getScale().y << ") to (" << scale.x << ", " << scale.y << ')';

    _rect->setScale(scale);

    NULL_CHECK_VOID(_checkmark_piece_left)
    NULL_CHECK_VOID(_checkmark_piece_right)

    _checkmark_piece_left->setScale(scale);
    _checkmark_piece_right->setScale(scale);

    _update_checkmark_pos();
}

// ----------------------------------------------------------------------------
void Checkbox::set_color(
    sf::Color const color
    ) {

    NULL_CHECK_VOID(_rect)

    _rect->setFillColor(color);
}

// ----------------------------------------------------------------------------
void Checkbox::set_border(
    Widget_border const border
    ) {

    NULL_CHECK_VOID(_rect)

    _rect->setOutlineColor(border.color);
    _rect->setOutlineThickness(border.size);
}

// ----------------------------------------------------------------------------
void Checkbox::set_checkmark_color(
    sf::Color const color
    ) {

    NULL_CHECK_VOID(_checkmark_piece_left)
    NULL_CHECK_VOID(_checkmark_piece_right)

    _checkmark_piece_left->setFillColor(color);
    _checkmark_piece_right->setFillColor(color);
}

// ----------------------------------------------------------------------------
void Checkbox::set_checkmark_size(
    sf::Vector2f const size
    ) {

    NULL_CHECK_VOID(_checkmark_piece_left)
    NULL_CHECK_VOID(_checkmark_piece_right)

    _checkmark_piece_left->setSize(size);
    _checkmark_piece_left->setOrigin(sf::Vector2f{ size.x * 0.5f, size.y * 0.5f });

    _checkmark_piece_right->setSize(size);
    _checkmark_piece_right->setOrigin(sf::Vector2f{ size.x * 0.5f, size.y * 0.5f });

    _update_checkmark_pos();
}

// ----------------------------------------------------------------------------
void Checkbox::set_checkmark_border(
    Widget_border const border
    ) {

    NULL_CHECK_VOID(_checkmark_piece_left)
    NULL_CHECK_VOID(_checkmark_piece_right)

    _checkmark_piece_left->setOutlineColor(border.color);
    _checkmark_piece_left->setOutlineThickness(border.size);

    _checkmark_piece_right->setOutlineColor(border.color);
    _checkmark_piece_right->setOutlineThickness(border.size);
}

// ----------------------------------------------------------------------------
void Checkbox::set_on_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_hover";

    _on_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Checkbox::set_on_exit_hover(
    std::function<void()> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_exit_hover";

    _on_exit_hover = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Checkbox::set_visible(
    bool const visible
    ) {

    _set_flag(State::HIDDEN, !visible);
}

}
