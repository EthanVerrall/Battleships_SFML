#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/menu.hpp"
#include "ui/include/widget.hpp"
#include "resources/include/spritesheet.hpp"

#include "sfml/Graphics.hpp"

#include <memory>
#include <vector>
#include <string_view>
#include <optional>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::menus {

// ============================================================================
// Class Options_menu
// ----------------------------------------------------------------------------

class Options_menu : public Menu {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Options_menu(sf::RenderWindow& window);

    ~Options_menu() = default;

    Options_menu(Options_menu const&) = delete;
    Options_menu& operator=(Options_menu const&) = delete;
    Options_menu(Options_menu&&) = delete;
    Options_menu& operator=(Options_menu&&) = delete;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    void draw() override;
    void update(float const dt) override;

    //--------------------------
    // Class builder functions
    //--------------------------
private:

    void _build__background_image();

    void _build__lbl_section__audio();
    void _build__lbl_section__video();
    void _build__lbl_section__ctrls();

    void _build__lbl__music_volume();     void _build__slider__music_volume();
    void _build__lbl__sfx_volume();       void _build__slider__sfx_volume();
    void _build__lbl__mute();             void _build__checkbox__mute();

    void _build__lbl__window_mode();      void _build__spinbox__window_mode();
    void _build__lbl__vsync();            void _build__checkbox__vsync();
    void _build__lbl__fps_cap();          void _build__spinbox__fps_cap();
    void _build__lbl__show_fps_counter(); void _build__checkbox__show_fps_counter();

    void _build__lbl__rotate_ship();      void _build__hotkey_recorder__rotate_ship();
    void _build__lbl__fire();             void _build__hotkey_recorder__fire();
    void _build__lbl__confirm();          void _build__hotkey_recorder__confirm();
    void _build__lbl__cancel();           void _build__hotkey_recorder__cancel();
    void _build__lbl__menu();             void _build__hotkey_recorder__menu();

    void _build__btn__apply();
    void _build__btn__back();

    //--------------------------
    // Helpers
    //--------------------------
private:

    template <typename T>
    T* _add_widget_to_container(
        std::unique_ptr<T> widget
        ) {

        T* ptr = widget.get();

        _widgets.emplace_back(std::move(widget));

        return ptr;
    }

    void _build_lbl(
        std::string_view const name,
        std::string_view const text,
        sf::Vector2f const pos
        );

    void _build_lbl_section(
        std::string_view const name,
        std::string_view const text,
        sf::Vector2f const pos
        );

    void _save_changed_settings();

    //--------------------------
    // Attributes
    //--------------------------
private:

    struct Changed_settings {

        std::optional<float> music_volume;
        std::optional<float> sfx_volume;
        std::optional<bool> mute;

        std::optional<sf::State> window_mode;
        std::optional<bool> vsync;
        std::optional<std::uint8_t> fps_cap;
        std::optional<bool> show_fps_counter;

        std::optional<sf::Keyboard::Key> rotate_ship;
        std::optional<sf::Keyboard::Key> fire;
        std::optional<sf::Keyboard::Key> confirm;
        std::optional<sf::Keyboard::Key> cancel;
        std::optional<sf::Keyboard::Key> menu;
    } _changed_settings;

    std::shared_ptr<resources::Static_spritesheet> _background_static_spritesheet;

    std::vector<std::unique_ptr<ui::Widget>> _widgets;
    std::unique_ptr<sf::Sprite> _background_img;
};

}
