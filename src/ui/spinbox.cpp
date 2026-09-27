// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/spinbox.hpp"
#include "utils/include/logger.hpp"

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

using namespace battleships::events;
using namespace battleships::utils;

// ============================================================================
// Class Spinbox
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Spinbox::Spinbox(
    sf::RenderWindow& window,
    std::string_view const name
    )
    :
    Widget(window, Widget_data{ .name = name, .type = Widget_type::SPINBOX })
    , _state(State::DEFAULT)
    , _label(nullptr)
    , _button_prev(nullptr)
    , _button_next(nullptr)
    , _layout_mode(Spinbox_layout::HORIZONTAL)
    , _button_offset(8.0f, 0.0f)
    , _button_prev_manual_pos(false)
    , _button_next_manual_pos(false)
    , _values()
    , _selected_index(0u)
    , _first_button_increments(false)
    , _on_value_changed()
    {

    _label = std::make_unique<Label>(_window, "spinbox_label");

    NULL_CHECK_VOID(_label)

    _button_prev = std::make_unique<Button>(_window, "spinbox_button_prev");

    NULL_CHECK_VOID(_button_prev)

    _button_prev->set_text("<");
    _button_prev->set_on_left_click([this](){ _handle_button_prev_click(); });

    _button_next = std::make_unique<Button>(_window, "spinbox_button_next");

    NULL_CHECK_VOID(_button_next)

    _button_next->set_text(">");
    _button_next->set_on_left_click([this](){ _handle_button_next_click(); });

    _update_button_rotation();
    _update_button_visibility();
    _layout();

    LOG(Log_lvl::TRACE) << "Created spinbox " << _data.name;
}

// ----------------------------------------------------------------------------
void Spinbox::draw() {

    if (_has_flag(State::HIDDEN)) {

        // Not visible so don't draw
    } else {

        if (_label != nullptr) { _label->draw(); }
        if (_button_prev != nullptr) { _button_prev->draw(); }
        if (_button_next != nullptr) { _button_next->draw(); }
    }
}

// ----------------------------------------------------------------------------
void Spinbox::update(
    float const dt
    ) {

    // Do nothing
    (void) dt;
}

// ----------------------------------------------------------------------------
void Spinbox::enable(
    bool const enable
    ) {

    _set_flag(State::DISABLED, !enable);

    if (_button_prev != nullptr) { _button_prev->enable(enable); }
    if (_button_next != nullptr) { _button_next->enable(enable); }

    _update_button_visibility();
}

// ----------------------------------------------------------------------------
void Spinbox::_layout() {

    NULL_CHECK_VOID(_label)
    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    sf::Vector2f const origin = _label->get_pos();
    sf::Vector2f const label_size = _label->get_size();
    sf::Vector2f const prev_size = _button_prev->get_size();

    if (!_button_prev_manual_pos) {

        if (_layout_mode == Spinbox_layout::HORIZONTAL) {

            _button_prev->set_bounds_pos({ origin.x - prev_size.x, origin.y });
        } else {

            _button_prev->set_bounds_pos({ origin.x, origin.y - prev_size.y });
        }

        _button_prev->set_bounds_pos(_button_prev->get_bounds_pos() - _button_offset);
    }

    if (!_button_next_manual_pos) {

        if (_layout_mode == Spinbox_layout::HORIZONTAL) {

            _button_next->set_bounds_pos({ origin.x + label_size.x, origin.y });
        } else {

            _button_next->set_bounds_pos({ origin.x, origin.y + label_size.y });
        }

        _button_next->set_bounds_pos(_button_next->get_bounds_pos() + _button_offset);
    }
}

// ----------------------------------------------------------------------------
void Spinbox::_update_button_rotation() {

    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    sf::Angle const angle = (_layout_mode == Spinbox_layout::HORIZONTAL)
        ? sf::degrees(0.0f)
        : sf::degrees(90.0f);

    _button_prev->set_rotation(angle);
    _button_next->set_rotation(angle);
}

// ----------------------------------------------------------------------------
void Spinbox::_update_button_visibility() {

    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    bool const at_first = (_selected_index == 0u);
    bool const at_last = _values.empty() || (_selected_index == (_values.size() - 1u));

    bool const prev_goes_back = !_first_button_increments;

    _button_prev->set_visible(prev_goes_back ? !at_first : !at_last);
    _button_next->set_visible(prev_goes_back ? !at_last : !at_first);
}

// ----------------------------------------------------------------------------
void Spinbox::_select_prev() {

    if (_selected_index == 0u) {

        return;
    }

    set_selected_index(_selected_index - 1u);
}

// ----------------------------------------------------------------------------
void Spinbox::_select_next() {

    if (_values.empty() || (_selected_index >= (_values.size() - 1u))) {

        return;
    }

    set_selected_index(_selected_index + 1u);
}

// ----------------------------------------------------------------------------
void Spinbox::_handle_button_prev_click() {

    if (_first_button_increments) {

        _select_next();
    } else {

        _select_prev();
    }
}

// ----------------------------------------------------------------------------
void Spinbox::_handle_button_next_click() {

    if (_first_button_increments) {

        _select_prev();
    } else {

        _select_next();
    }
}

// ----------------------------------------------------------------------------
std::string Spinbox::_state_to_string(
    std::uint8_t const state
    ) {

    if (state == State::NONE) { return "NONE"; }

    std::string result;

    if (state & State::DISABLED) { result += "DISABLED|"; }
    if (state & State::HIDDEN) { result += "HIDDEN|"; }

    if (!result.empty()) { result.pop_back(); }

    return result;
}

// ----------------------------------------------------------------------------
void Spinbox::_set_flag(
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
bool Spinbox::_has_flag(
    State const flag
    ) const {

    return (_state & flag) != 0;
}

// ----------------------------------------------------------------------------
sf::Vector2f Spinbox::get_pos() const {

    NULL_CHECK(_label)

    return _label->get_pos();
}

// ----------------------------------------------------------------------------
sf::Vector2f Spinbox::get_size() const {

    NULL_CHECK(_label)
    NULL_CHECK(_button_prev)
    NULL_CHECK(_button_next)

    sf::Vector2f const label_size = _label->get_size();
    sf::Vector2f const prev_size = _button_prev->get_size();
    sf::Vector2f const next_size = _button_next->get_size();

    if (_layout_mode == Spinbox_layout::HORIZONTAL) {

        return {
            prev_size.x + _button_offset.x + label_size.x + _button_offset.x + next_size.x,
            label_size.y
            };
    } else {

        return {
            label_size.x,
            prev_size.y + _button_offset.y + label_size.y + _button_offset.y + next_size.y
            };
    }
}

// ----------------------------------------------------------------------------
sf::Vector2f Spinbox::get_scale() const {

    NULL_CHECK(_label)

    return _label->get_scale();
}

// ----------------------------------------------------------------------------
Spinbox_layout Spinbox::get_layout() const {

    return _layout_mode;
}

// ----------------------------------------------------------------------------
sf::Vector2f Spinbox::get_button_offset() const {

    return _button_offset;
}

// ----------------------------------------------------------------------------
std::vector<std::string> const& Spinbox::get_values() const {

    return _values;
}

// ----------------------------------------------------------------------------
std::size_t Spinbox::get_selected_index() const {

    return _selected_index;
}

// ----------------------------------------------------------------------------
std::string_view Spinbox::get_selected_value() const {

    if (_selected_index >= _values.size()) {

        return {};
    }

    return _values[_selected_index];
}

// ----------------------------------------------------------------------------
bool Spinbox::is_enabled() const {

    return !_has_flag(State::DISABLED);
}

// ----------------------------------------------------------------------------
bool Spinbox::is_visible() const {

    return !_has_flag(State::HIDDEN);
}

// ----------------------------------------------------------------------------
void Spinbox::set_pos(
    sf::Vector2f const pos
    ) {

    NULL_CHECK_VOID(_label)

    LOG(Log_lvl::TRACE) << _data.name << ": set_pos from (" << _label->get_pos().x << ", "
    << _label->get_pos().y << ") to (" << pos.x << ", " << pos.y << ')';

    _label->set_pos(pos);

    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_size(
    sf::Vector2f const size
    ) {

    NULL_CHECK_VOID(_label)

    sf::Vector2f const label_size = _label->get_size();

    if ((label_size.x <= 0.0f) || (label_size.y <= 0.0f)) {

        LOG(Log_lvl::WARN) << _data.name << ": label has zero size, can't set size.";
        return;
    }

    sf::Vector2f const target_scale{
        size.x / label_size.x,
        size.y / label_size.y
        };

    set_scale(target_scale);
}

// ----------------------------------------------------------------------------
void Spinbox::set_scale(
    sf::Vector2f const scale
    ) {

    NULL_CHECK_VOID(_label)
    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    LOG(Log_lvl::TRACE) << _data.name << ": set_scale to (" << scale.x << ", " << scale.y << ')';

    _label->set_scale(scale);
    _button_prev->set_scale(scale);
    _button_next->set_scale(scale);

    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_layout(
    Spinbox_layout const layout
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_layout";

    _layout_mode = layout;

    _update_button_rotation();
    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_button_offset(
    sf::Vector2f const offset
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_button_offset from (" << _button_offset.x << ", "
    << _button_offset.y << ") to (" << offset.x << ", " << offset.y << ')';

    _button_offset = offset;

    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_button_prev_pos(
    sf::Vector2f const pos
    ) {

    NULL_CHECK_VOID(_button_prev)

    LOG(Log_lvl::TRACE) << _data.name << ": set_button_prev_pos to (" << pos.x << ", " << pos.y << ')';

    _button_prev_manual_pos = true;

    _button_prev->set_bounds_pos(pos);
}

// ----------------------------------------------------------------------------
void Spinbox::set_button_next_pos(
    sf::Vector2f const pos
    ) {

    NULL_CHECK_VOID(_button_next)

    LOG(Log_lvl::TRACE) << _data.name << ": set_button_next_pos to (" << pos.x << ", " << pos.y << ')';

    _button_next_manual_pos = true;

    _button_next->set_bounds_pos(pos);
}

// ----------------------------------------------------------------------------
void Spinbox::set_values(
    std::vector<std::string> const& values
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_values, count = " << values.size();

    _values = values;
    _selected_index = 0u;

    NULL_CHECK_VOID(_label)

    _label->set_text(_values.empty() ? "" : _values[_selected_index]);

    _update_button_visibility();
    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_selected_index(
    std::size_t const index
    ) {

    if (index >= _values.size()) {

        LOG(Log_lvl::WARN) << _data.name << ": index " << index << " out of range, can't select.";
        return;
    }

    LOG(Log_lvl::TRACE) << _data.name << ": set_selected_index from " << _selected_index << " to " << index;

    _selected_index = index;

    NULL_CHECK_VOID(_label)

    _label->set_text(_values[_selected_index]);

    _update_button_visibility();
    _layout();

    if (_on_value_changed) {

        _on_value_changed(_values[_selected_index]);
    }
}

// ----------------------------------------------------------------------------
void Spinbox::set_increment_direction(
    bool const first_button_increments
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_increment_direction to " << first_button_increments;

    _first_button_increments = first_button_increments;

    _update_button_visibility();
}

// ----------------------------------------------------------------------------
void Spinbox::set_on_value_changed(
    std::function<void(std::string_view const)> call_back
    ) {

    LOG(Log_lvl::TRACE) << _data.name << ": set_on_value_changed";

    _on_value_changed = std::move(call_back);
}

// ----------------------------------------------------------------------------
void Spinbox::set_font(
    std::string_view const font_name
    ) {

    NULL_CHECK_VOID(_label)
    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    _label->set_font(font_name);
    _button_prev->set_font(font_name);
    _button_next->set_font(font_name);

    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_text_color(
    sf::Color const color
    ) {

    NULL_CHECK_VOID(_label)
    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    _label->set_text_color(color);
    _button_prev->set_text_color(color);
    _button_next->set_text_color(color);
}

// ----------------------------------------------------------------------------
void Spinbox::set_label_size(
    sf::Vector2f const size
    ) {

    NULL_CHECK_VOID(_label)

    sf::Vector2f const label_scale = _label->get_scale();
    sf::Vector2f const unscaled_size{
        _label->get_size().x / label_scale.x,
        _label->get_size().y / label_scale.y
        };

    if ((unscaled_size.x <= 0.0f) || (unscaled_size.y <= 0.0f)) {

        LOG(Log_lvl::WARN) << _data.name << ": label has zero size, can't set label size.";
        return;
    }

    _label->set_scale({
        size.x / unscaled_size.x,
        size.y / unscaled_size.y
        });

    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_button_size(
    sf::Vector2f const size
    ) {

    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    sf::Vector2f const prev_scale = _button_prev->get_scale();
    sf::Vector2f const unscaled_size{
        _button_prev->get_size().x / prev_scale.x,
        _button_prev->get_size().y / prev_scale.y
        };

    if ((unscaled_size.x <= 0.0f) || (unscaled_size.y <= 0.0f)) {

        LOG(Log_lvl::WARN) << _data.name << ": button has zero size, can't set button size.";
        return;
    }

    sf::Vector2f const target_scale{
        size.x / unscaled_size.x,
        size.y / unscaled_size.y
        };

    _button_prev->set_scale(target_scale);
    _button_next->set_scale(target_scale);

    _layout();
}

// ----------------------------------------------------------------------------
void Spinbox::set_visible(
    bool const visible
    ) {

    _set_flag(State::HIDDEN, !visible);

    NULL_CHECK_VOID(_label)
    NULL_CHECK_VOID(_button_prev)
    NULL_CHECK_VOID(_button_next)

    _label->set_visible(visible);

    if (visible) {

        _update_button_visibility();
    } else {

        _button_prev->set_visible(false);
        _button_next->set_visible(false);
    }
}

}
