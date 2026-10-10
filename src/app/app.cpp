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

    void App::_handle_event___settings_saved(
        events::Game_event_data const& data
        ) {

        auto const* changed_keys = std::any_cast<std::vector<std::string>>(&data.payload);

        if (changed_keys == nullptr) {

            LOG(utils::Log_lvl::WARN) << "SETTINGS_SAVED payload was not a vector<string>.";
            return;
        }

        auto const& settings_manager = settings::Settings_manager::instance();

        bool const needs_recreate = std::any_of(
            changed_keys->begin(),
            changed_keys->end(),
            [](std::string const& key) {

                return (key == "video_window_mode") || (key == "video_anti_aliasing");
            }
            );

        if (needs_recreate) {

            create_render_window();
            return;
        }

        if ((std::find(changed_keys->begin(), changed_keys->end(), "video_show_fps_counter") != changed_keys->end())
            && (_fps_counter != nullptr)) {

            _fps_counter->set_visible(settings_manager.get<bool>("video_show_fps_counter"));
        }

        if (std::find(changed_keys->begin(), changed_keys->end(), "video_vsync") != changed_keys->end()) {

            _window.setVerticalSyncEnabled(settings_manager.get<bool>("video_vsync"));
        }

        if (std::find(changed_keys->begin(), changed_keys->end(), "video_fps_cap") != changed_keys->end()) {

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

        auto& sfml_events_manager = events::SFML_event_manager::instance();
        auto& game_events_manager = events::Game_event_manager::instance();
        auto& resource_manager = resources::Resource_manager::instance();
        auto& menu_manager = menus::Menu_manager::instance(_window);

        // For testing purposes, delete in future TODO
        sfml_events_manager.register_callback(
            events::SFML_event_type::KEYRELEASE_ESCAPE,
            [this](events::SFML_event_data const&) { _window.close(); },
            0
            );

        game_events_manager.register_callback(
            events::Game_event_type::SETTINGS_SAVED,
            [this](events::Game_event_data const& data) { _handle_event___settings_saved(data); },
            0
            );

        sf::Clock frame_clock;

        while (_window.isOpen()){

            float const dt = frame_clock.restart().asSeconds();

            sfml_events_manager.process_events(_window);

            menu_manager.update(dt);

            if (_fps_counter != nullptr) { _fps_counter->update(dt); }

            _window.clear(sf::Color::Black);

            menu_manager.draw();

            // Drawn after the menu so the counter is never hidden behind it.
            if (_fps_counter != nullptr) { _fps_counter->draw(); }

            _window.display();

            if (auto const menu_id = menu_manager.get_if_pending_menu_change()) {

                menu_manager.change_menu(menu_id.value());
            }

            resource_manager.flush_unused();
        }

        LOG(utils::Log_lvl::INFO) << "Program terminated.";
    }

}
