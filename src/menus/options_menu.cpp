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
#include "settings/include/settings_manager.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

using namespace battleships::ui;
using namespace battleships::utils;
using namespace battleships::resources;
using namespace battleships::settings;

namespace battleships::menus {

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
    , _widgets(29u)
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

    _build_lbl_section("lbl_section_audio", "Audio", {200.0f, 50.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl_section__video() {

    _build_lbl_section("lbl_section_video", "Video", {600.0f, 50.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl_section__ctrls() {

    _build_lbl_section("lbl_section_ctrls", "Controls", {1000.0f, 50.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__music_volume() {

    _build_lbl("lbl_music_volume", "Music Volume", {1000.0f, 700.0f});
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

        widget->set_size({300.0f, 20.0f});
        widget->set_fill_color(sf::Color::Red);
        widget->set_track_color(sf::Color::Yellow);
        widget->set_track_border({ .color = colors::DEFAULT_BORDER, .size = 5.0f });
        widget->set_pos({200.0f, 300.0f});
        widget->set_handle_radius(15.0f);
        widget->set_handle_color(sf::Color::Red);
        widget->set_handle_border({ .color = colors::DEFAULT_BORDER, .size = 5.0f });
        widget->set_text_offset({0.0f, -30.0f});
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });

        widget->set_on_value_changed([this](float const vol){

            _changed_settings.music_volume = vol;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__sfx_volume() {

    _build_lbl("lbl_sfx_volume", "SFX Volume", {1000.0f, 700.0f});
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

        widget->set_size({300.0f, 20.0f});
        widget->set_fill_color(sf::Color::Red);
        widget->set_track_color(sf::Color::Yellow);
        widget->set_track_border({ .color = colors::DEFAULT_BORDER, .size = 5.0f });
        widget->set_pos({200.0f, 300.0f});
        widget->set_handle_radius(15.0f);
        widget->set_handle_color(sf::Color::Red);
        widget->set_handle_border({ .color = colors::DEFAULT_BORDER, .size = 5.0f });
        widget->set_text_offset({0.0f, -30.0f});
        widget->set_text_border({ .color = colors::DEFAULT_BORDER, .size = 2.0f });

        widget->set_on_value_changed([this](float const vol){

            _changed_settings.sfx_volume = vol;
        });
    }
}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__mute() {

    _build_lbl("lbl_mute", "Mute", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__mute() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__window_mode() {

    _build_lbl("lbl_window_mode", "Window Mode", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__window_mode() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__vsync() {

    _build_lbl("lbl_vsync", "VSYNC", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__vsync() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__fps_cap() {

    _build_lbl("lbl_fps_cap", "FPS Cap", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__spinbox__fps_cap() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__show_fps_counter() {

    _build_lbl("lbl_show_fps_counter", "Show FPS Counter", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__checkbox__show_fps_counter() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__rotate_ship() {

    _build_lbl("lbl_rotate_ship", "Rotate Ship", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__rotate_ship() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__fire() {

    _build_lbl("lbl_fire", "Rotate Ship", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__fire() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__confirm() {

    _build_lbl("lbl_confirm", "Confirm", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__confirm() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__cancel() {

    _build_lbl("lbl_cancel", "Cancel", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__cancel() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__lbl__menu() {

    _build_lbl("lbl_menu", "Menu", {1000.0f, 700.0f});
}

// ----------------------------------------------------------------------------
void Options_menu::_build__hotkey_recorder__menu() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__btn__apply() {


}

// ----------------------------------------------------------------------------
void Options_menu::_build__btn__back() {


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
        sizes::BUTTON_TEXT
        );

    if (!lbl) {

        LOG(utils::Log_lvl::WARN) << name << " is nullptr on options menu. Failed to build.";
    } else {

        auto widget = _add_widget_to_container(std::move(lbl));

        widget->set_text_color(colors::DEFAULT_TEXT);
        widget->set_border({.color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER });
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
        widget->set_pos(pos);
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

    set_if_changed("keybinds_rotate_ship", _changed_settings.rotate_ship);
    set_if_changed("keybinds_fire", _changed_settings.fire);
    set_if_changed("keybinds_confirm", _changed_settings.confirm);
    set_if_changed("keybinds_cancel", _changed_settings.cancel);
    set_if_changed("keybinds_menu", _changed_settings.menu);

    settings_manager.save();
}

}