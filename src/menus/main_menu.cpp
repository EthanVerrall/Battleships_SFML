// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "menus/include/main_menu.hpp"
#include "utils/include/logger.hpp"
#include "resources/include/asset_registry.hpp"
#include "utils/include/sizes.hpp"
#include "utils/include/colors.hpp"

#include <random>
#include <chrono>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

using namespace battleships::ui;
using namespace battleships::utils;
using namespace battleships::utils;

namespace battleships::menus {

// ============================================================================
// Class Label
// ----------------------------------------------------------------------------

    //--------------------------
    // Constructor / Destructor
    //--------------------------
    Main_menu::Main_menu(sf::RenderWindow& window) : Menu(window)
    {
        build_background_image();
        build_title_lbl();
        build_play_game_btn();
        build_scoreboard_btn();
        build_options_btn();
        build_profile_btn();
        build_exit_btn();
    }


    //--------------------------
    // Class builder functions
    //--------------------------
    void Main_menu::build_title_lbl() {

        _title_lbl = std::make_unique<Label> (_window, "Title_label", "pixel_bold","Battleships",sizes::HEADING_TEXT);

        if (!_title_lbl) {
            LOG(utils::Log_lvl::WARN) << "Title_label is nullptr on main menu. Failed to build.";
        }
        else {

            _title_lbl->set_text_color(colors::HEADING_TEXT);
            _title_lbl->set_border({.color = colors::HEADING_BORDER, .size = sizes::HEADING_BORDER });

            const float label_mpx = _title_lbl->get_size().x / 2.0f;
            const sf::Vector2f window_mp = get_window_mp();
            const sf::Vector2f label_pos {window_mp.x - label_mpx, 50.0f};
            _title_lbl->set_pos(label_pos);
        }
    }

    void Main_menu::build_play_game_btn() {

        _play_game_btn = std::make_unique<Button> (_window, "Play_game_button");

        if (!_play_game_btn) {
            LOG(utils::Log_lvl::WARN) << "Play_game_button is nullptr on main menu. Failed to build.";
        }
        else {

            _play_game_btn->set_text("Play Game");
            _play_game_btn->set_text_char_size(sizes::BUTTON_TEXT);
            _play_game_btn->set_text_color(colors::DEFAULT_TEXT);
            _play_game_btn->set_text_border({.color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER});

            _play_game_btn->set_on_hover([&]()
                {
                    _play_game_btn->set_text_color(utils::colors::BUTTON_TEXT_HOVER);
                }
            );

            _play_game_btn->set_on_exit_hover([&]()
                {
                    _play_game_btn->set_text_color(utils::colors::DEFAULT_TEXT);
                }
            );

            const sf::Vector2f button_pos {325.0f, 300.0f};
            _play_game_btn->set_pos(button_pos);
        }

    }

    void Main_menu::build_scoreboard_btn() {

        _scoreboard_btn= std::make_unique<Button> (_window, "Scoreboard_button");

        if (!_scoreboard_btn) {
            LOG(utils::Log_lvl::WARN) << "Scoreboard_button is nullptr on main menu. Failed to build.";
        }
        else {

            _scoreboard_btn->set_text("Scoreboard");
            _scoreboard_btn->set_text_char_size(sizes::BUTTON_TEXT);
            _scoreboard_btn->set_text_color(colors::DEFAULT_TEXT);
            _scoreboard_btn->set_text_border({.color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER});

            _scoreboard_btn->set_on_hover([&]()
                {
                    _scoreboard_btn->set_text_color(utils::colors::BUTTON_TEXT_HOVER);
                }
            );

            _scoreboard_btn->set_on_exit_hover([&]()
                {
                    _scoreboard_btn->set_text_color(utils::colors::DEFAULT_TEXT);
                }
            );

            const sf::Vector2f button_pos {325.0f, 450.0f};
            _scoreboard_btn->set_pos(button_pos);
        }
    }

    void Main_menu::build_options_btn() {

        _options_btn= std::make_unique<Button> (_window, "Options_button");

        if (!_options_btn) {
            LOG(utils::Log_lvl::WARN) << "Options_button is nullptr on main menu. Failed to build.";
        }
        else {

            _options_btn->set_text("Options");
            _options_btn->set_text_char_size(sizes::BUTTON_TEXT);
            _options_btn->set_text_color(colors::DEFAULT_TEXT);
            _options_btn->set_text_border({.color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER});

            _options_btn->set_on_hover([&]()
                {
                    _options_btn->set_text_color(utils::colors::BUTTON_TEXT_HOVER);
                }
            );

            _options_btn->set_on_exit_hover([&]()
                {
                    _options_btn->set_text_color(utils::colors::DEFAULT_TEXT);
                }
            );

            const sf::Vector2f button_pos {325.0f, 600.0f};
            _options_btn->set_pos(button_pos);
        }
    }

    void Main_menu::build_profile_btn() {

        _profile_btn= std::make_unique<Button> (_window, "Profile_button");

        if (!_profile_btn) {
            LOG(utils::Log_lvl::WARN) << "Profile_button is nullptr on main menu. Failed to build.";
        }
        else {

            _profile_btn->set_text("Profile");
            _profile_btn->set_text_char_size(sizes::BUTTON_TEXT);
            _profile_btn->set_text_color(colors::DEFAULT_TEXT);
            _profile_btn->set_text_border({.color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER});

            _profile_btn->set_on_hover([&]()
                {
                    _profile_btn->set_text_color(utils::colors::BUTTON_TEXT_HOVER);
                }
            );

            _profile_btn->set_on_exit_hover([&]()
                {
                    _profile_btn->set_text_color(utils::colors::DEFAULT_TEXT);
                }
            );

            const sf::Vector2f button_pos {325.0f, 750.0f};
            _profile_btn->set_pos(button_pos);
        }
    }

    void Main_menu::build_exit_btn() {

        _exit_btn= std::make_unique<Button> (_window, "Exit_button");

        if (!_exit_btn) {
            LOG(utils::Log_lvl::WARN) << "Exit_button is nullptr on main menu. Failed to build.";
        }
        else {

            _exit_btn->set_text("Exit game");
            _exit_btn->set_text_char_size(sizes::BUTTON_TEXT);
            _exit_btn->set_text_color(colors::DEFAULT_TEXT);
            _exit_btn->set_text_border({.color = colors::DEFAULT_BORDER, .size = sizes::BUTTON_BORDER});

            _exit_btn->set_on_hover([&]()
                {
                    _exit_btn->set_text_color(utils::colors::BUTTON_TEXT_HOVER);
                }
            );

            _exit_btn->set_on_exit_hover([&]()
                {
                    _exit_btn->set_text_color(utils::colors::DEFAULT_TEXT);
                }
            );

            _exit_btn->set_on_left_click([&]()
                {
                    _window.close();
                }
            );

            const sf::Vector2f button_pos {325.0f, 900.0f};
            _exit_btn->set_pos(button_pos);
        }
    }

    void Main_menu::build_background_image() {

        std::random_device rd{};
	    std::seed_seq ss{
			static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
				rd(), rd(), rd(), rd(), rd(), rd(), rd() };

        std::mt19937 mt{ ss };
        const int random_num = std::uniform_int_distribution{1, 3}(mt);

        switch (random_num) {

            case 1:
                _static_spritesheet = resources::Asset_registry::load_static_spritesheet("background_coast");
                break;

            case 2:
                _static_spritesheet = resources::Asset_registry::load_static_spritesheet("background_storm");
                break;

            case 3:
                _static_spritesheet = resources::Asset_registry::load_static_spritesheet("background_sunset");
                break;

            default: LOG(utils::Log_lvl::WARN) << "Mersenne Twister failed to generate random number.";
                break;
        }

        //Early return if spritesheet fails to load
        if (!_static_spritesheet) {
            LOG(utils::Log_lvl::WARN) << "Static spritesheet was unable to load from asset registry.";
            return;
        }

        _static_spritesheet->texture().setSmooth(true);

        if (const auto background_region = _static_spritesheet->get_region("background")) {

            _background_img = std::make_unique<sf::Sprite> (_static_spritesheet->texture());
            _background_img->setTextureRect(background_region->rect);
        }
        else {
            LOG(utils::Log_lvl::WARN) << "Unable to find region for background image.";
        }

    }

    void Main_menu::draw() {

        if(_background_img) { _window.draw(*_background_img); }
        if(_title_lbl) {_title_lbl->draw(); }
        if(_play_game_btn) {_play_game_btn->draw(); }
        if(_scoreboard_btn) {_scoreboard_btn->draw(); }
        if (_options_btn) {_options_btn->draw(); }
        if (_profile_btn) {_profile_btn->draw(); }
        if (_exit_btn) {_exit_btn->draw(); }
    }

    void Main_menu::update(float const dt) { static_cast<void> (dt); /*Not implemented*/ };


    //--------------------------
    // Getters
    //--------------------------
    sf::Vector2f Main_menu::get_window_mp() const {

        const sf::Vector2u window_size = _window.getSize();
        const sf::Vector2f window_mp {window_size.x / 2.0f, window_size.y / 2.0f };
        return window_mp;
    }

}