#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "event_type.hpp"
#include "event_data.hpp"

#include "SFML/Graphics.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <cstdint>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::event {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using SFML_callback = std::function<void(SFML_event_data const&)>;

// ============================================================================
// Class SFML_event_manager
// ----------------------------------------------------------------------------

class SFML_event_manager final {

public:
    SFML_event_manager(SFML_event_manager const&) = delete;
    SFML_event_manager& operator=(SFML_event_manager const&) = delete;
    SFML_event_manager(SFML_event_manager&&) = delete;
    SFML_event_manager& operator=(SFML_event_manager&&) = delete;

    static SFML_event_manager& instance();

    void process_events(sf::Window& window);

    void process_events(sf::RenderWindow& window);

    std::uint64_t register_callback(
        SFML_event_type const type,
        SFML_callback const& callback,
        std::uint64_t const listener_id
        );

    void deregister_callback(std::uint64_t const callback_id);

    void deregister_listener(std::uint64_t const listener_id);

    void deregister_type(SFML_event_type const type);

    void clear();

private:
    SFML_event_manager();

    ~SFML_event_manager() = default;

    void _dispatch(SFML_event_type const type, SFML_event_data const& data);
    void _process_held_keys();
    void _process_held_mouse_buttons();

    SFML_event_type _key_to_press_event(sf::Keyboard::Key const key);
    SFML_event_type _key_to_held_event(sf::Keyboard::Key const key);
    SFML_event_type _key_to_release_event(sf::Keyboard::Key const key);

    SFML_event_type _mouse_to_press_event(sf::Mouse::Button const button);
    SFML_event_type _mouse_to_held_event(sf::Mouse::Button const button);
    SFML_event_type _mouse_to_release_event(sf::Mouse::Button const button);

    struct Callback_entry {

        std::uint64_t callback_id;
        std::uint64_t listener_id;
        SFML_callback callback;
    };

    std::unordered_map<SFML_event_type, std::vector<Callback_entry>> _callbacks;
    std::unordered_map<sf::Keyboard::Key, bool> _held_keys;
    std::unordered_map<sf::Mouse::Button, bool> _held_mouse_buttons;

    std::uint64_t _next_callback_id;
};

}
