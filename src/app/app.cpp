// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/app.hpp"
#include "utils/include/logger.hpp"
#include "events/include/event_type.hpp"

#include <algorithm>
#include <any>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::app {

// ============================================================================
// Class App
// ----------------------------------------------------------------------------

    //--------------------------
    // Constructor / Destructor
    //--------------------------
    App::App()
        : _window()
        , _fps_counter(nullptr)
    {
        settings::Settings_manager::instance().load();

        create_render_window();

        _build__fps_counter();

        _register_events();
    }

    //--------------------------
    // Class specific functions
    //--------------------------
    void App::create_render_window() {

        auto& settings_manager = settings::Settings_manager::instance();
        sf::State startup_state (settings_manager.get<sf::State>("video_window_mode"));

        sf::ContextSettings startup_context_settings
            { .antiAliasingLevel = settings_manager.get<std::uint8_t>("video_anti_aliasing")};

        // Close the old window before constructing the new one. Without this,
        // requesting Fullscreen while the previous fullscreen window is still
        // alive causes SFML to silently fall back to Windowed ("Creating two
        // fullscreen windows is not allowed").
        if (_window.isOpen()) {

            _window.close();
        }

        _window = sf::RenderWindow(
            sf::VideoMode({1920u, 1080u}),
            "Battleships",
            sf::Style::Default,
            startup_state,
            startup_context_settings
        );

        _window.setVerticalSyncEnabled(settings_manager.get<bool>("video_vsync"));
        _window.setFramerateLimit(settings_manager.get<std::uint8_t>("video_fps_cap"));

        LOG(utils::Log_lvl::TRACE) << "Render window created.";
    }

    void App::_build__fps_counter() {

        auto const& settings_manager = settings::Settings_manager::instance();

        _fps_counter = std::make_unique<ui::Fps_counter>(_window, "fps_counter");

        if (_fps_counter == nullptr) {

            LOG(utils::Log_lvl::WARN) << "fps_counter is nullptr. Failed to build.";
            return;
        }

        _fps_counter->set_pos({ 20.0f, 20.0f });
        _fps_counter->set_visible(settings_manager.get<bool>("video_show_fps_counter"));
    }

    void App::_update(
        float const dt
        ) {

        menus::Menu_manager::instance(_window).update(dt);

        if (_fps_counter != nullptr) {

            _fps_counter->update(dt);
        }
    }

    void App::_draw() {

        _window.clear(sf::Color::Black);

        menus::Menu_manager::instance(_window).draw();

        // Drawn after the menu so the counter is never hidden behind it.
        if (_fps_counter != nullptr) {

            _fps_counter->draw();
        }

        _window.display();
    }

    void App::_register_events() {

        auto& sfml_event_manager = events::SFML_event_manager::instance();
        auto& game_event_manager = events::Game_event_manager::instance();

        // For testing purposes, delete in future TODO
        sfml_event_manager.register_callback(
            events::SFML_event_type::KEYRELEASE_ESCAPE,
            [this](events::SFML_event_data const&) { _window.close(); },
            listener_id()
            );

        game_event_manager.register_callback(
            events::Game_event_type::SETTINGS_SAVED,
            [this](events::Game_event_data const& data) { _handle_event__settings_saved(data); },
            listener_id()
            );
    }

    void App::_handle_event__settings_saved(
        events::Game_event_data const& data
        ) {

        auto const* changed_keys = std::any_cast<std::vector<std::string>>(&data.payload);

        if (changed_keys == nullptr) {

            LOG(utils::Log_lvl::WARN) << "SETTINGS_SAVED payload was not a vector<string>.";
            return;
        }

        auto const& settings_manager = settings::Settings_manager::instance();

        auto const has_changed = [&changed_keys](std::string_view const key) {

            return std::find(changed_keys->begin(), changed_keys->end(), key) != changed_keys->end();
        };

        // The counter is applied before the recreate check below, because a
        // recreated window is handed the same counter rather than a new one.
        if ((_fps_counter != nullptr) && has_changed("video_show_fps_counter")) {

            _fps_counter->set_visible(settings_manager.get<bool>("video_show_fps_counter"));
        }

        // A recreated window is built from the current settings, so it already
        // picks up vsync and the fps cap. Nothing left to apply here.
        if (has_changed("video_window_mode") || has_changed("video_anti_aliasing")) {

            create_render_window();
            return;
        }

        if (has_changed("video_vsync")) {

            _window.setVerticalSyncEnabled(settings_manager.get<bool>("video_vsync"));
        }

        if (has_changed("video_fps_cap")) {

            _window.setFramerateLimit(settings_manager.get<std::uint8_t>("video_fps_cap"));
        }
    }

    void App::run_program() {

        LOG_LVLS_ENABLED(
            utils::Log_lvl::INFO |
            utils::Log_lvl::DEBUG |
            utils::Log_lvl::ERR |
            utils::Log_lvl::WARN
            );

        LOG(utils::Log_lvl::INFO) << "Program started.";

        auto& sfml_event_manager = events::SFML_event_manager::instance();
        auto& resource_manager = resources::Resource_manager::instance();
        auto& menu_manager = menus::Menu_manager::instance(_window);

        sf::Clock frame_clock;

        while (_window.isOpen()) {

            float const dt = frame_clock.restart().asSeconds();

            sfml_event_manager.process_events(_window);

            _update(dt);

            _draw();

            if (auto const menu_id = menu_manager.get_if_pending_menu_change()) {

                menu_manager.change_menu(menu_id.value());
            }

            resource_manager.flush_unused();
        }

        LOG(utils::Log_lvl::INFO) << "Program terminated.";
    }

}
