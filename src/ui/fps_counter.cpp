// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/fps_counter.hpp"
#include "utils/include/logger.hpp"
#include "utils/include/colors.hpp"
#include "utils/include/sizes.hpp"
#include "utils/include/null_check.hpp"

#include <format>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::ui {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::utils;

// ============================================================================
// Constants
// ----------------------------------------------------------------------------

namespace {

    inline constexpr float REFRESH_INTERVAL = 0.5f;

    inline constexpr char const* PLACEHOLDER_TEXT = "FPS: --";

}

// ============================================================================
// Class Fps_counter
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Fps_counter::Fps_counter(
    sf::RenderWindow& window,
    std::string_view const name
    )
    :
    Widget(window, Widget_data{ .name = name, .type = Widget_type::FPS_COUNTER })
    , _lbl(nullptr)
    , _frames(0u)
    , _elapsed_time(0.0f)
    , _fps(0.0f)
    , _is_visible(true)
    {

    _create__lbl();

    LOG(Log_lvl::TRACE) << "Created fps counter " << _data.name;
}

// ----------------------------------------------------------------------------
void Fps_counter::draw() {

    if (!_is_visible) {

        // Not visible so don't draw
    } else {

        if (_lbl != nullptr) {

            _lbl->draw();
        }
    }
}

// ----------------------------------------------------------------------------
void Fps_counter::update(
    float const dt
    ) {

    if (!_is_visible) {

        // Hidden, so don't accrue samples that would stale the next average
        return;
    }

    ++_frames;
    _elapsed_time += dt;

    if (_elapsed_time < REFRESH_INTERVAL) {

        // Still averaging over the current interval
        return;
    }

    _fps = static_cast<float>(_frames) / _elapsed_time;

    _refresh_text(_fps);

    _frames = 0u;
    _elapsed_time = 0.0f;
}

// ----------------------------------------------------------------------------
void Fps_counter::_create__lbl() {

    _lbl = std::make_unique<Label>(
        _window,
        _data.name,
        "pixel_bold",
        PLACEHOLDER_TEXT,
        sizes::FPS_COUNTER_TEXT
        );

    NULL_CHECK_VOID(_lbl)

    _lbl->set_text_color(colors::FPS_COUNTER_TEXT);
    _lbl->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::FPS_COUNTER_BORDER });
}

// ----------------------------------------------------------------------------
void Fps_counter::_refresh_text(
    float const fps
    ) {

    NULL_CHECK_VOID(_lbl)

    _lbl->set_text(std::format("FPS: {:.0f}", fps));
}

// ----------------------------------------------------------------------------
sf::Vector2f Fps_counter::get_pos() const {

    NULL_CHECK(_lbl)

    return _lbl->get_pos();
}

// ----------------------------------------------------------------------------
sf::Vector2f Fps_counter::get_size() const {

    NULL_CHECK(_lbl)

    return _lbl->get_size();
}

// ----------------------------------------------------------------------------
sf::Vector2f Fps_counter::get_scale() const {

    NULL_CHECK(_lbl)

    return _lbl->get_scale();
}

// ----------------------------------------------------------------------------
sf::Color Fps_counter::get_color() const {

    NULL_CHECK(_lbl)

    return _lbl->get_text_color();
}

// ----------------------------------------------------------------------------
Widget_border Fps_counter::get_border() const {

    NULL_CHECK(_lbl)

    return _lbl->get_border();
}

// ----------------------------------------------------------------------------
unsigned int Fps_counter::get_char_size() const {

    NULL_CHECK(_lbl)

    return _lbl->get_char_size();
}

// ----------------------------------------------------------------------------
float Fps_counter::get_fps() const {

    return _fps;
}

// ----------------------------------------------------------------------------
bool Fps_counter::is_visible() const {

    return _is_visible;
}

// ----------------------------------------------------------------------------
void Fps_counter::set_pos(
    sf::Vector2f const pos
    ) {

    NULL_CHECK_VOID(_lbl)

    _lbl->set_pos(pos);
}

// ----------------------------------------------------------------------------
void Fps_counter::set_size(
    sf::Vector2f const size
    ) {

    static_cast<void> (size);
    LOG(Log_lvl::WARN) << "Setting size for fps counter is unimplemented";
}

// ----------------------------------------------------------------------------
void Fps_counter::set_scale(
    sf::Vector2f const scale
    ) {

    NULL_CHECK_VOID(_lbl)

    _lbl->set_scale(scale);
}

// ----------------------------------------------------------------------------
void Fps_counter::set_color(
    sf::Color const color
    ) {

    NULL_CHECK_VOID(_lbl)

    _lbl->set_text_color(color);
}

// ----------------------------------------------------------------------------
void Fps_counter::set_border(
    Widget_border const border
    ) {

    NULL_CHECK_VOID(_lbl)

    _lbl->set_border(border);
}

// ----------------------------------------------------------------------------
void Fps_counter::set_char_size(
    unsigned int const char_size
    ) {

    NULL_CHECK_VOID(_lbl)

    _lbl->set_char_size(char_size);
}

// ----------------------------------------------------------------------------
void Fps_counter::set_visible(
    bool const visible
    ) {

    if (_is_visible == visible) {

        return;
    }

    _is_visible = visible;

    // Drop the samples gathered before hiding, and show a placeholder until
    // the first interval after showing has been averaged.
    _frames = 0u;
    _elapsed_time = 0.0f;
    _fps = 0.0f;

    if (_lbl != nullptr) {

        _lbl->set_text(PLACEHOLDER_TEXT);
    }
}

}
