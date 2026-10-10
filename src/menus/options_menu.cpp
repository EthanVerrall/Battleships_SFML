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
    , _widgets(31u)
    , _background_img(nullptr)
    {

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

    auto slider = std::make_unique<Slider>(
        _window,
        "slider_music_volume"
        );

    if (!slider) {

        LOG(utils::Log_lvl::WARN) << "slider_music_volume" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(slider));

        auto& settings_manager = Settings_manager::instance();

        widget->set_size({220.0f, 20.0f});
        widget->set_fill_color(colors::HEADING_TEXT);
        widget->set_track_color(colors::BUTTON_TEXT_HOVER);
        widget->set_track_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_pos({COL_AUDIO_WIDGET_X, ROW_START_Y + (0.0f * AUDIO_ROW_STEP) + SLIDER_ALIGN_Y});
        widget->set_handle_radius(15.0f);
        widget->set_handle_color(colors::HEADING_TEXT);
        widget->set_handle_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_text_offset({0.0f, -35.0f});
        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_range(0.0f, 100.0f);
        widget->set_value(settings_manager.get<float>("audio_music_volume"));

        widget->set_on_hover([widget]() { widget->set_handle_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_handle_color(colors::HEADING_TEXT); });

        widget->set_on_value_changed([this](float const vol){

            _changed_settings.music_volume = vol;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__sfx_volume() {

    _build_lbl("lbl_sfx_volume", "SFX Volume", {COL_AUDIO_X, ROW_START_Y + (1.0f * AUDIO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__slider__sfx_volume() {

    auto slider = std::make_unique<Slider>(
        _window,
        "slider_sfx_volume"
        );

    if (!slider) {

        LOG(utils::Log_lvl::WARN) << "slider_sfx_volume" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(slider));

        auto& settings_manager = Settings_manager::instance();

        widget->set_size({220.0f, 20.0f});
        widget->set_fill_color(colors::HEADING_TEXT);
        widget->set_track_color(colors::BUTTON_TEXT_HOVER);
        widget->set_track_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_pos({COL_AUDIO_WIDGET_X, ROW_START_Y + (1.0f * AUDIO_ROW_STEP) + SLIDER_ALIGN_Y});
        widget->set_handle_radius(15.0f);
        widget->set_handle_color(colors::HEADING_TEXT);
        widget->set_handle_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_text_offset({0.0f, -35.0f});
        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_range(0.0f, 100.0f);
        widget->set_value(settings_manager.get<float>("audio_sfx_volume"));

        widget->set_on_hover([widget]() { widget->set_handle_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_handle_color(colors::HEADING_TEXT); });

        widget->set_on_value_changed([this](float const vol){

            _changed_settings.sfx_volume = vol;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__mute() {

    _build_lbl("lbl_mute", "Mute", {COL_AUDIO_X, ROW_START_Y + (2.0f * AUDIO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__mute() {

    auto checkbox = std::make_unique<Checkbox>(
        _window,
        "checkbox_mute"
        );

    if (!checkbox) {

        LOG(utils::Log_lvl::WARN) << "checkbox_mute" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(checkbox));

        auto& settings_manager = Settings_manager::instance();

        widget->set_size({40.0f, 40.0f});
        widget->set_pos({COL_AUDIO_WIDGET_X, ROW_START_Y + (2.0f * AUDIO_ROW_STEP) + CHECKBOX_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_checkmark_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_checkmark_color(colors::HEADING_TEXT);

        widget->set_checked(settings_manager.get<bool>("audio_mute"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed([this](bool const checked){

            _changed_settings.mute = checked;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__window_mode() {

    _build_lbl("lbl_window_mode", "Window Mode", {COL_VIDEO_X, ROW_START_Y + (0.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__window_mode() {

    auto spinbox = std::make_unique<Spinbox>(
        _window,
        "spinbox_window_mode"
        );

    if (!spinbox) {

        LOG(utils::Log_lvl::WARN) << "spinbox_window_mode" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(spinbox));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_VIDEO_WIDGET_X, ROW_START_Y + (0.0f * VIDEO_ROW_STEP) + SPINBOX_ALIGN_Y});
        widget->set_label_text_color(colors::DEFAULT_TEXT);
        widget->set_label_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_text_color(colors::BUTTON_TEXT_HOVER);
        widget->set_button_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_perpendicular_shift(SPINBOX_BUTTON_DOWN_Y);

        widget->set_values({
            "Windowed", "Fullscreen"
            });

        sf::State const current_mode = settings_manager.get<sf::State>("video_window_mode");
        widget->set_selected_index(current_mode == sf::State::Fullscreen ? 1u : 0u);

        widget->set_button_prev_on_hover([widget]() { widget->set_button_prev_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_prev_on_exit_hover([widget]() { widget->set_button_prev_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_button_next_on_hover([widget]() { widget->set_button_next_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_next_on_exit_hover([widget]() { widget->set_button_next_text_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed([this](std::string_view const val){

            if (val == "Windowed") { _changed_settings.window_mode = sf::State::Windowed; }
            if (val == "Fullscreen") { _changed_settings.window_mode = sf::State::Fullscreen; }
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__vsync() {

    _build_lbl("lbl_vsync", "VSYNC", {COL_VIDEO_X, ROW_START_Y + (1.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__vsync() {

    auto checkbox = std::make_unique<Checkbox>(
        _window,
        "checkbox_vsync"
        );

    if (!checkbox) {

        LOG(utils::Log_lvl::WARN) << "checkbox_vsync" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(checkbox));

        auto& settings_manager = Settings_manager::instance();

        widget->set_size({40.0f, 40.0f});
        widget->set_pos({COL_VIDEO_WIDGET_X, ROW_START_Y + (1.0f * VIDEO_ROW_STEP) + CHECKBOX_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_checkmark_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_checkmark_color(colors::HEADING_TEXT);

        widget->set_checked(settings_manager.get<bool>("video_vsync"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed([this](bool const checked){

            _changed_settings.vsync = checked;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__fps_cap() {

    _build_lbl("lbl_fps_cap", "FPS Cap", {COL_VIDEO_X, ROW_START_Y + (2.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__fps_cap() {

    auto spinbox = std::make_unique<Spinbox>(
        _window,
        "spinbox_fps_cap"
        );

    if (!spinbox) {

        LOG(utils::Log_lvl::WARN) << "spinbox_fps_cap" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(spinbox));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_VIDEO_WIDGET_X, ROW_START_Y + (2.0f * VIDEO_ROW_STEP) + SPINBOX_ALIGN_Y});
        widget->set_label_text_color(colors::DEFAULT_TEXT);
        widget->set_label_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_text_color(colors::BUTTON_TEXT_HOVER);
        widget->set_button_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_perpendicular_shift(SPINBOX_BUTTON_DOWN_Y);

        widget->set_values({
            "30", "60", "75", "120", "144", "165", "240", "Unlimited"
            });

        std::uint8_t const current_cap = settings_manager.get<std::uint8_t>("video_fps_cap");
        auto const& values = widget->get_values();

        for (std::size_t i = 0u; i < values.size(); ++i) {

            if (values[i] == std::to_string(static_cast<int>(current_cap))) {

                widget->set_selected_index(i);
                break;
            }
        }

        widget->set_button_prev_on_hover([widget]() { widget->set_button_prev_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_prev_on_exit_hover([widget]() { widget->set_button_prev_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_button_next_on_hover([widget]() { widget->set_button_next_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_next_on_exit_hover([widget]() { widget->set_button_next_text_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed([this](std::string_view const val){

            if (val == "Unlimited") {

                _changed_settings.fps_cap = 0u;
            } else {

                _changed_settings.fps_cap = static_cast<std::uint8_t>(std::stoi(std::string(val)));
            }
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__show_fps_counter() {

    _build_lbl("lbl_show_fps_counter", "Show FPS Counter", {COL_VIDEO_X, ROW_START_Y + (3.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__show_fps_counter() {

    auto checkbox = std::make_unique<Checkbox>(
        _window,
        "checkbox_show_fps_counter"
        );

    if (!checkbox) {

        LOG(utils::Log_lvl::WARN) << "checkbox_show_fps_counter" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(checkbox));

        auto& settings_manager = Settings_manager::instance();

        widget->set_size({40.0f, 40.0f});
        widget->set_pos({COL_VIDEO_WIDGET_X, ROW_START_Y + (3.0f * VIDEO_ROW_STEP) + CHECKBOX_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_checkmark_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_checkmark_color(colors::HEADING_TEXT);

        widget->set_checked(settings_manager.get<bool>("video_show_fps_counter"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed([this](bool const checked){

            _changed_settings.show_fps_counter = checked;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__anti_aliasing() {

    _build_lbl("lbl_anti_aliasing", "Anti-Aliasing", {COL_VIDEO_X, ROW_START_Y + (4.0f * VIDEO_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__anti_aliasing() {

    auto spinbox = std::make_unique<Spinbox>(
        _window,
        "spinbox_anti_aliasing"
        );

    if (!spinbox) {

        LOG(utils::Log_lvl::WARN) << "spinbox_anti_aliasing" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(spinbox));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_VIDEO_WIDGET_X, ROW_START_Y + (4.0f * VIDEO_ROW_STEP) + SPINBOX_ALIGN_Y});
        widget->set_label_text_color(colors::DEFAULT_TEXT);
        widget->set_label_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_text_color(colors::BUTTON_TEXT_HOVER);
        widget->set_button_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });
        widget->set_button_perpendicular_shift(SPINBOX_BUTTON_DOWN_Y);

        widget->set_values({
            "Off", "2x", "4x", "8x", "16x"
            });

        std::uint8_t const current_aa = settings_manager.get<std::uint8_t>("video_anti_aliasing");
        auto const& values = widget->get_values();
        std::string const current_label = (current_aa == 0u) ? "Off" : (std::to_string(static_cast<int>(current_aa)) + "x");

        for (std::size_t i = 0u; i < values.size(); ++i) {

            if (values[i] == current_label) {

                widget->set_selected_index(i);
                break;
            }
        }

        widget->set_button_prev_on_hover([widget]() { widget->set_button_prev_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_prev_on_exit_hover([widget]() { widget->set_button_prev_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_button_next_on_hover([widget]() { widget->set_button_next_text_color(colors::DEFAULT_TEXT); });
        widget->set_button_next_on_exit_hover([widget]() { widget->set_button_next_text_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_value_changed([this](std::string_view const val){

            if (val == "Off") {

                _changed_settings.anti_aliasing = 0u;
            } else {

                _changed_settings.anti_aliasing = static_cast<std::uint8_t>(std::stoi(std::string(val)));
            }
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__rotate_ship() {

    _build_lbl("lbl_rotate_ship", "Rotate Ship", {COL_CTRLS_X, ROW_START_Y + (0.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__rotate_ship() {

    auto recorder = std::make_unique<Hotkey_recorder>(
        _window,
        "hotkey_recorder_rotate_ship"
        );

    if (!recorder) {

        LOG(utils::Log_lvl::WARN) << "hotkey_recorder_rotate_ship" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(recorder));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_CTRLS_WIDGET_X, ROW_START_Y + (0.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_key(settings_manager.get<sf::Keyboard::Key>("keybinds_rotate_ship"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_key_changed([this](sf::Keyboard::Key const key){

            _changed_settings.rotate_ship = key;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__fire() {

    _build_lbl("lbl_fire", "Fire", {COL_CTRLS_X, ROW_START_Y + (1.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__fire() {

    auto recorder = std::make_unique<Hotkey_recorder>(
        _window,
        "hotkey_recorder_fire"
        );

    if (!recorder) {

        LOG(utils::Log_lvl::WARN) << "hotkey_recorder_fire" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(recorder));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_CTRLS_WIDGET_X, ROW_START_Y + (1.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_key(settings_manager.get<sf::Keyboard::Key>("keybinds_fire"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_key_changed([this](sf::Keyboard::Key const key){

            _changed_settings.fire = key;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__confirm() {

    _build_lbl("lbl_confirm", "Confirm", {COL_CTRLS_X, ROW_START_Y + (2.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__confirm() {

    auto recorder = std::make_unique<Hotkey_recorder>(
        _window,
        "hotkey_recorder_confirm"
        );

    if (!recorder) {

        LOG(utils::Log_lvl::WARN) << "hotkey_recorder_confirm" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(recorder));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_CTRLS_WIDGET_X, ROW_START_Y + (2.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_key(settings_manager.get<sf::Keyboard::Key>("keybinds_confirm"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_key_changed([this](sf::Keyboard::Key const key){

            _changed_settings.confirm = key;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__cancel() {

    _build_lbl("lbl_cancel", "Cancel", {COL_CTRLS_X, ROW_START_Y + (3.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__cancel() {

    auto recorder = std::make_unique<Hotkey_recorder>(
        _window,
        "hotkey_recorder_cancel"
        );

    if (!recorder) {

        LOG(utils::Log_lvl::WARN) << "hotkey_recorder_cancel" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(recorder));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_CTRLS_WIDGET_X, ROW_START_Y + (3.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_key(settings_manager.get<sf::Keyboard::Key>("keybinds_cancel"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_key_changed([this](sf::Keyboard::Key const key){

            _changed_settings.cancel = key;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__menu() {

    _build_lbl("lbl_menu", "Menu", {COL_CTRLS_X, ROW_START_Y + (4.0f * CTRLS_ROW_STEP)});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__menu() {

    auto recorder = std::make_unique<Hotkey_recorder>(
        _window,
        "hotkey_recorder_menu"
        );

    if (!recorder) {

        LOG(utils::Log_lvl::WARN) << "hotkey_recorder_menu" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(recorder));

        auto& settings_manager = Settings_manager::instance();

        widget->set_pos({COL_CTRLS_WIDGET_X, ROW_START_Y + (4.0f * CTRLS_ROW_STEP) + HOTKEY_ALIGN_Y});
        widget->set_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_color(colors::BUTTON_TEXT_HOVER);
        widget->set_text_color(colors::HEADING_TEXT);
        widget->set_key(settings_manager.get<sf::Keyboard::Key>("keybinds_menu"));

        widget->set_on_hover([widget]() { widget->set_color(colors::DEFAULT_TEXT); });
        widget->set_on_exit_hover([widget]() { widget->set_color(colors::BUTTON_TEXT_HOVER); });

        widget->set_on_key_changed([this](sf::Keyboard::Key const key){

            _changed_settings.menu = key;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__btn__apply() {

    auto button = std::make_unique<Button>(
        _window,
        "btn_apply"
        );

    if (!button) {

        LOG(utils::Log_lvl::WARN) << "btn_apply" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(button));

        widget->set_text("Apply");
        widget->set_text_char_size(sizes::BUTTON_TEXT);
        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_pos({754.5f, 960.0f});

        widget->set_on_hover([widget]() { widget->set_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_on_exit_hover([widget]() { widget->set_text_color(colors::DEFAULT_TEXT); });

        widget->set_on_left_click([this](){ _save_changed_settings(); });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__btn__back() {

    auto button = std::make_unique<Button>(
        _window,
        "btn_back"
        );

    if (!button) {

        LOG(utils::Log_lvl::WARN) << "btn_back" << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(button));

        widget->set_text("Back");
        widget->set_text_char_size(sizes::BUTTON_TEXT);
        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
        widget->set_pos({1009.5f, 960.0f});

        widget->set_on_hover([widget]() { widget->set_text_color(colors::BUTTON_TEXT_HOVER); });
        widget->set_on_exit_hover([widget]() { widget->set_text_color(colors::DEFAULT_TEXT); });

        widget->set_on_left_click([this](){ _emit_event___pending_menu_change(Menu_id::MAIN_MENU); });
    }
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
void Options_menu::_save_changed_settings() {

    auto& settings_manager = Settings_manager::instance();

    auto set_if_changed = [&settings_manager](
        std::string_view const setting_key,
        auto const& val
        ) {

        if (val.has_value()) { settings_manager.set(setting_key, val.value()); }
        };

    set_if_changed("audio_music_volume", _changed_settings.music_volume);
    set_if_changed("audio_sfx_volume", _changed_settings.sfx_volume);
    set_if_changed("audio_mute", _changed_settings.mute);

    set_if_changed("video_window_mode", _changed_settings.window_mode);
    set_if_changed("video_vsync", _changed_settings.vsync);
    set_if_changed("video_fps_cap", _changed_settings.fps_cap);
    set_if_changed("video_show_fps_counter", _changed_settings.show_fps_counter);
    set_if_changed("video_anti_aliasing", _changed_settings.anti_aliasing);

    set_if_changed("keybinds_rotate_ship", _changed_settings.rotate_ship);
    set_if_changed("keybinds_fire", _changed_settings.fire);
    set_if_changed("keybinds_confirm", _changed_settings.confirm);
    set_if_changed("keybinds_cancel", _changed_settings.cancel);
    set_if_changed("keybinds_menu", _changed_settings.menu);

    settings_manager.save();

    _changed_settings = Changed_settings{};
}

}
