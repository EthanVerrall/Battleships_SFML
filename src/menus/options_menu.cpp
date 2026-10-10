// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/options_menu.hpp"
#include "utils/include/logger.hpp"
#include "resources/include/asset_registry.hpp"
#include "resources/include/resource_manager.hpp"
#include "utils/include/sizes.hpp"
#include "utils/include/colors.hpp"
#include "ui/include/label.hpp"
#include "ui/include/slider.hpp"
#include "ui/include/checkbox.hpp"
#include "ui/include/spinbox.hpp"
#include "ui/include/hotkey_recorder.hpp"
#include "ui/include/button.hpp"
#include "settings/include/settings_manager.hpp"

#include <variant>

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::ui;
using namespace battleships::utils;
using namespace battleships::resources;
using namespace battleships::settings;

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::menus {

// ============================================================================
// Constants
// ----------------------------------------------------------------------------

namespace {

    inline constexpr float COL_AUDIO_X        = 80.0f;
    inline constexpr float COL_AUDIO_WIDGET_X = COL_AUDIO_X + 260.0f;

    inline constexpr float COL_VIDEO_X        = 760.0f;
    inline constexpr float COL_VIDEO_WIDGET_X = COL_VIDEO_X + 320.0f;

    inline constexpr float COL_CTRLS_X        = 1360.0f;
    inline constexpr float COL_CTRLS_WIDGET_X = COL_CTRLS_X + 230.0f;

    inline constexpr float SECTION_Y      = 80.0f;
    inline constexpr float ROW_START_Y    = 260.0f;

    inline constexpr float AUDIO_ROW_STEP = 150.0f;
    inline constexpr float VIDEO_ROW_STEP = 100.0f;
    inline constexpr float CTRLS_ROW_STEP = 100.0f;

    inline constexpr float CHECKBOX_ALIGN_Y = 4.0f;
    inline constexpr float SLIDER_ALIGN_Y   = 14.0f;
    inline constexpr float SPINBOX_ALIGN_Y  = 1.5f;
    inline constexpr float HOTKEY_ALIGN_Y   = 8.0f;

    inline constexpr float SPINBOX_BUTTON_DOWN_Y = 12.8f;

}

// ============================================================================
// Class Options_menu
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Options_menu::Options_menu(
    sf::RenderWindow& window
    )
    : Menu(window)
    , _changed_settings()
    , _background_static_spritesheet(nullptr)
    , _widgets()
    , _background_img(nullptr)
    {

    std::size_t const total_number_of_widgets = 32u;

    _widgets.reserve(total_number_of_widgets);

    _build__background_image();

    _build__lbl_section__audio();
    _build__lbl_section__video();
    _build__lbl_section__ctrls();

    _build__lbl__music_volume();     _build__slider__music_volume();
    _build__lbl__sfx_volume();       _build__slider__sfx_volume();
    _build__lbl__mute();             _build__checkbox__mute();

    _build__lbl__window_mode();      _build__spinbox__window_mode();
    _build__lbl__vsync();            _build__checkbox__vsync();
    _build__lbl__fps_cap();          _build__spinbox__fps_cap();
    _build__lbl__show_fps_counter(); _build__checkbox__show_fps_counter();
    _build__lbl__anti_aliasing();    _build__spinbox__anti_aliasing();

    _build__lbl__rotate_ship();      _build__hotkey_recorder__rotate_ship();
    _build__lbl__fire();             _build__hotkey_recorder__fire();
    _build__lbl__confirm();          _build__hotkey_recorder__confirm();
    _build__lbl__cancel();           _build__hotkey_recorder__cancel();
    _build__lbl__menu();             _build__hotkey_recorder__menu();

    _build__btn__apply();
    _build__btn__back();
}

// ----------------------------------------------------------------------------
void Options_menu::draw() {

    if (_background_img) { _window.draw(*_background_img); }

    for (auto& widget : _widgets) {

        if (widget) { widget->draw(); }
    }
}

// ----------------------------------------------------------------------------
void Options_menu::update(
    float const dt
    ) {

    for (auto& widget : _widgets) {

        if (widget) { widget->update(dt); }
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__background_image() {

    auto& resource_manager = Resource_manager::instance();

    if (resource_manager.is_loaded("background_coast")) {

        _background_static_spritesheet = std::move(Asset_registry::load_static_spritesheet("background_control_room_coast"));
    } else if (resource_manager.is_loaded("background_storm")) {

        _background_static_spritesheet = std::move(Asset_registry::load_static_spritesheet("background_control_room_storm"));
    } else if (resource_manager.is_loaded("background_sunset")) {

        _background_static_spritesheet = std::move(Asset_registry::load_static_spritesheet("background_control_room_sunset"));
    } else {

        LOG(Log_lvl::WARN) << "Unknown background image is used on the main menu.";
    }

    if (_background_static_spritesheet == nullptr) {

        LOG(utils::Log_lvl::WARN) << "Static spritesheet was unable to load from asset registry.";
        return;
    }

    _background_static_spritesheet->texture().setSmooth(true);

    if (const auto background_region = _background_static_spritesheet->get_region("background")) {

        _background_img = std::make_unique<sf::Sprite>(_background_static_spritesheet->texture());
        _background_img->setTextureRect(background_region->rect);
    } else {

        LOG(utils::Log_lvl::WARN) << "Unable to find region for background image.";
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl_section__audio() {

    _build_lbl_section("lbl_section_audio", "Audio", {380.0f, SECTION_Y});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl_section__video() {

    _build_lbl_section("lbl_section_video", "Video", {995.0f, SECTION_Y});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl_section__ctrls() {

    _build_lbl_section("lbl_section_ctrls", "Controls", {1525.0f, SECTION_Y});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__music_volume() {

    _build_lbl("lbl_music_volume", "Music Volume", {COL_AUDIO_X, ROW_START_Y + (0.0f * AUDIO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__slider__music_volume() {

    auto& settings_manager = Settings_manager::instance();

    _build_slider(
        "slider_music_volume",
        {COL_AUDIO_WIDGET_X, ROW_START_Y + (0.0f * AUDIO_ROW_STEP) + SLIDER_ALIGN_Y},
        settings_manager.get<float>("audio_music_volume"),
        [this](float const vol){ _changed_settings["audio_music_volume"] = vol; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__sfx_volume() {

    _build_lbl("lbl_sfx_volume", "SFX Volume", {COL_AUDIO_X, ROW_START_Y + (1.0f * AUDIO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__slider__sfx_volume() {

    auto& settings_manager = Settings_manager::instance();

    _build_slider(
        "slider_sfx_volume",
        {COL_AUDIO_WIDGET_X, ROW_START_Y + (1.0f * AUDIO_ROW_STEP) + SLIDER_ALIGN_Y},
        settings_manager.get<float>("audio_sfx_volume"),
        [this](float const vol){ _changed_settings["audio_sfx_volume"] = vol; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__mute() {

    _build_lbl("lbl_mute", "Mute", {COL_AUDIO_X, ROW_START_Y + (2.0f * AUDIO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__mute() {

    auto& settings_manager = Settings_manager::instance();

    _build_checkbox(
        "checkbox_mute",
        {COL_AUDIO_WIDGET_X, ROW_START_Y + (2.0f * AUDIO_ROW_STEP) + CHECKBOX_ALIGN_Y},
        settings_manager.get<bool>("audio_mute"),
        [this](bool const checked){ _changed_settings["audio_mute"] = checked; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__window_mode() {

    _build_lbl("lbl_window_mode", "Window Mode", {COL_VIDEO_X, ROW_START_Y + (0.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__window_mode() {

    auto& settings_manager = Settings_manager::instance();

    sf::State const current_mode = settings_manager.get<sf::State>("video_window_mode");

    _build_spinbox(
        "spinbox_window_mode",
        {COL_VIDEO_WIDGET_X, ROW_START_Y + (0.0f * VIDEO_ROW_STEP) + SPINBOX_ALIGN_Y},
        { "Windowed", "Fullscreen" },
        (current_mode == sf::State::Fullscreen) ? "Fullscreen" : "Windowed",
        [this](std::string_view const val){

            _changed_settings["video_window_mode"] =
                (val == "Fullscreen") ? sf::State::Fullscreen : sf::State::Windowed;
        });
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__vsync() {

    _build_lbl("lbl_vsync", "VSYNC", {COL_VIDEO_X, ROW_START_Y + (1.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__vsync() {

    auto& settings_manager = Settings_manager::instance();

    _build_checkbox(
        "checkbox_vsync",
        {COL_VIDEO_WIDGET_X, ROW_START_Y + (1.0f * VIDEO_ROW_STEP) + CHECKBOX_ALIGN_Y},
        settings_manager.get<bool>("video_vsync"),
        [this](bool const checked){ _changed_settings["video_vsync"] = checked; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__fps_cap() {

    _build_lbl("lbl_fps_cap", "FPS Cap", {COL_VIDEO_X, ROW_START_Y + (2.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__fps_cap() {

    auto& settings_manager = Settings_manager::instance();

    std::uint8_t const current_cap = settings_manager.get<std::uint8_t>("video_fps_cap");
    std::string const current_label = (current_cap == 0u) ? "Unlimited" : std::to_string(static_cast<int>(current_cap));

    _build_spinbox(
        "spinbox_fps_cap",
        {COL_VIDEO_WIDGET_X, ROW_START_Y + (2.0f * VIDEO_ROW_STEP) + SPINBOX_ALIGN_Y},
        { "30", "60", "75", "120", "144", "165", "240", "Unlimited" },
        current_label,
        [this](std::string_view const val){

            if (val == "Unlimited") {

                _changed_settings["video_fps_cap"] = std::uint8_t{0u};
            } else {

                _changed_settings["video_fps_cap"] =
                    static_cast<std::uint8_t>(std::stoi(std::string(val)));
            }
        });
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__show_fps_counter() {

    _build_lbl("lbl_show_fps_counter", "Show FPS Counter", {COL_VIDEO_X, ROW_START_Y + (3.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__show_fps_counter() {

    auto& settings_manager = Settings_manager::instance();

    _build_checkbox(
        "checkbox_show_fps_counter",
        {COL_VIDEO_WIDGET_X, ROW_START_Y + (3.0f * VIDEO_ROW_STEP) + CHECKBOX_ALIGN_Y},
        settings_manager.get<bool>("video_show_fps_counter"),
        [this](bool const checked){ _changed_settings["video_show_fps_counter"] = checked; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__anti_aliasing() {

    _build_lbl("lbl_anti_aliasing", "Anti-Aliasing", {COL_VIDEO_X, ROW_START_Y + (4.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__anti_aliasing() {

    auto& settings_manager = Settings_manager::instance();

    std::uint8_t const current_aa = settings_manager.get<std::uint8_t>("video_anti_aliasing");
    std::string const current_label = (current_aa == 0u) ? "Off" : (std::to_string(static_cast<int>(current_aa)) + "x");

    _build_spinbox(
        "spinbox_anti_aliasing",
        {COL_VIDEO_WIDGET_X, ROW_START_Y + (4.0f * VIDEO_ROW_STEP) + SPINBOX_ALIGN_Y},
        { "Off", "2x", "4x", "8x", "16x" },
        current_label,
        [this](std::string_view const val){

            if (val == "Off") {

                _changed_settings["video_anti_aliasing"] = std::uint8_t{0u};
            } else {

                _changed_settings["video_anti_aliasing"] =
                    static_cast<std::uint8_t>(std::stoi(std::string(val)));
            }
        });
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__rotate_ship() {

    _build_lbl("lbl_rotate_ship", "Rotate Ship", {COL_CTRLS_X, ROW_START_Y + (0.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__rotate_ship() {

    auto& settings_manager = Settings_manager::instance();

    _build_hotkey_recorder(
        "hotkey_recorder_rotate_ship",
        {COL_CTRLS_WIDGET_X, ROW_START_Y + (0.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y},
        settings_manager.get<sf::Keyboard::Key>("keybinds_rotate_ship"),
        [this](sf::Keyboard::Key const key){ _changed_settings["keybinds_rotate_ship"] = key; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__fire() {

    _build_lbl("lbl_fire", "Fire", {COL_CTRLS_X, ROW_START_Y + (1.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__fire() {

    auto& settings_manager = Settings_manager::instance();

    _build_hotkey_recorder(
        "hotkey_recorder_fire",
        {COL_CTRLS_WIDGET_X, ROW_START_Y + (1.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y},
        settings_manager.get<sf::Keyboard::Key>("keybinds_fire"),
        [this](sf::Keyboard::Key const key){ _changed_settings["keybinds_fire"] = key; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__confirm() {

    _build_lbl("lbl_confirm", "Confirm", {COL_CTRLS_X, ROW_START_Y + (2.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__confirm() {

    auto& settings_manager = Settings_manager::instance();

    _build_hotkey_recorder(
        "hotkey_recorder_confirm",
        {COL_CTRLS_WIDGET_X, ROW_START_Y + (2.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y},
        settings_manager.get<sf::Keyboard::Key>("keybinds_confirm"),
        [this](sf::Keyboard::Key const key){ _changed_settings["keybinds_confirm"] = key; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__cancel() {

    _build_lbl("lbl_cancel", "Cancel", {COL_CTRLS_X, ROW_START_Y + (3.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__cancel() {

    auto& settings_manager = Settings_manager::instance();

    _build_hotkey_recorder(
        "hotkey_recorder_cancel",
        {COL_CTRLS_WIDGET_X, ROW_START_Y + (3.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y},
        settings_manager.get<sf::Keyboard::Key>("keybinds_cancel"),
        [this](sf::Keyboard::Key const key){ _changed_settings["keybinds_cancel"] = key; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__menu() {

    _build_lbl("lbl_menu", "Menu", {COL_CTRLS_X, ROW_START_Y + (4.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__menu() {

    auto& settings_manager = Settings_manager::instance();

    _build_hotkey_recorder(
        "hotkey_recorder_menu",
        {COL_CTRLS_WIDGET_X, ROW_START_Y + (4.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y},
        settings_manager.get<sf::Keyboard::Key>("keybinds_menu"),
        [this](sf::Keyboard::Key const key){ _changed_settings["keybinds_menu"] = key; }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__btn__apply() {

    _build_btn(
        "btn_apply",
        "Apply",
        {754.5f, 960.0f},
        [this](){ _save_changed_settings(); }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build__btn__back() {

    _build_btn(
        "btn_back",
        "Back",
        {1009.5f, 960.0f},
        [this](){ _emit_event___pending_menu_change(Menu_id::MAIN_MENU); }
        );
}

// ----------------------------------------------------------------------------
void Options_menu::_build_lbl(
    std::string_view const name,
    std::string_view const text,
    sf::Vector2f const pos
    ) {

    auto lbl = std::make_unique<Label>(
        _window,
        name,
        "pixel_bold",
        text,
        sizes::ROW_LABEL_TEXT
        );

    if (!lbl) {

        LOG(utils::Log_lvl::WARN) << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(lbl));

        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_border({.color = colors::DEFAULT_BORDER, .size = sizes::ROW_LABEL_BORDER });
        widget->set_pos(pos);
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build_lbl_section(
    std::string_view const name,
    std::string_view const text,
    sf::Vector2f const pos
    ) {

    auto lbl = std::make_unique<Label>(
        _window,
        name,
        "pixel_bold",
        text,
        sizes::SECTIONS_TEXT
        );

    if (!lbl) {

        LOG(utils::Log_lvl::WARN) << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(lbl));

        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_border({.color = colors::HEADING_BORDER, .size = sizes::SECTIONS_BORDER });

        float const label_half_width = widget->get_size().x / 2.0f;
        widget->set_pos({ pos.x - label_half_width, pos.y });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build_slider(
    std::string_view const name,
    sf::Vector2f const pos,
    float const val,
    std::function<void(float)> on_value_changed
    ) {

    auto slider = std::make_unique<Slider>(
        _window,
        name
        );

    if (!slider) {

        LOG(utils::Log_lvl::WARN) << "slider " << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(slider));

        widget->set_size({220.0f, 20.0f});
        widget->set_fill_color(colors::HEADING_TEXT);
        widget->set_track_color(colors::BUTTON_TEXT_HOVER);
        widget->set_track_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_pos(pos);
        widget->set_handle_radius(15.0f);
        widget->set_handle_color(colors::HEADING_TEXT);
        widget->set_handle_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_text_offset({0.0f, -35.0f});
        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_range(0.0f, 100.0f);
        widget->set_value(val);

        widget->set_on_hover([widget]() { widget->set_handle_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_handle_color(colors::HEADING_TEXT); });

        widget->set_on_value_changed(std::move(on_value_changed));
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build_spinbox(
    std::string_view const name,
    sf::Vector2f const pos,
    std::vector<std::string> const& values,
    std::string_view const selected_value,
    std::function<void(std::string_view)> on_value_changed
    ) {

    auto spinbox = std::make_unique<Spinbox>(
        _window,
        name
        );

    if (!spinbox) {

        LOG(utils::Log_lvl::WARN) << "spinbox " << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(spinbox));

        widget->set_pos(pos);
        widget->set_label_text_color(colors::DEFAULT_TEXT);
        widget->set_label_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_text_color(colors::BUTTON_TEXT_HOVER);
        widget->set_button_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_perpendicular_shift(SPINBOX_BUTTON_DOWN_Y);

        widget->set_values(values);

        for (std::size_t i = 0u; i < values.size(); ++i) {

            if (values[i] == selected_value) {

                widget->set_selected_index(i);
                break;
            }
        }

        widget->set_button_prev_on_hover([widget]() { widget->set_button_prev_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_prev_on_exit_hover([widget]() { widget->set_button_prev_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_button_next_on_hover([widget]() { widget->set_button_next_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_next_on_exit_hover([widget]() { widget->set_button_next_text_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed(std::move(on_value_changed));
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build_checkbox(
    std::string_view const name,
    sf::Vector2f const pos,
    bool const checked,
    std::function<void(bool)> on_value_changed
    ) {

    auto checkbox = std::make_unique<Checkbox>(
        _window,
        name
        );

    if (!checkbox) {

        LOG(utils::Log_lvl::WARN) << "checkbox " << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(checkbox));

        widget->set_size({40.0f, 40.0f});
        widget->set_pos(pos);
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_checkmark_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_checkmark_color(colors::HEADING_TEXT);

        widget->set_checked(checked);

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed(std::move(on_value_changed));
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build_hotkey_recorder(
    std::string_view const name,
    sf::Vector2f const pos,
    sf::Keyboard::Key const key,
    std::function<void(sf::Keyboard::Key)> on_key_changed
    ) {

    auto recorder = std::make_unique<Hotkey_recorder>(
        _window,
        name
        );

    if (!recorder) {

        LOG(utils::Log_lvl::WARN) << "hotkey recorder " << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(recorder));

        widget->set_pos(pos);
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_key(key);

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_key_changed(std::move(on_key_changed));
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build_btn(
    std::string_view const name,
    std::string_view const text,
    sf::Vector2f const pos,
    std::function<void()> on_left_click
    ) {

    auto button = std::make_unique<Button>(
        _window,
        name
        );

    if (!button) {

        LOG(utils::Log_lvl::WARN) << "button " << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(button));

        widget->set_text(text);
        widget->set_text_char_size(sizes::BUTTON_TEXT);
        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_pos(pos);

        widget->set_on_hover([widget]() { widget->set_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_on_exit_hover([widget]() { widget->set_text_color(colors::DEFAULT_TEXT); });

        widget->set_on_left_click(std::move(on_left_click));
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_save_changed_settings() {

    auto& settings_manager = Settings_manager::instance();

    for (auto const& [key, value] : _changed_settings) {

        std::visit(
            [&settings_manager, key](auto const& data) { settings_manager.set(key, data); },
            value
            );
    }

    settings_manager.save();

    _changed_settings.clear();
}

}
