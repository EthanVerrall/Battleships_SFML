#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "event_type.hpp"
#include "event_data.hpp"

#include <functional>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::events {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using Game_callback = std::function<void(Game_event_data const&)>;

// ============================================================================
// Class Game_event_manager
// ----------------------------------------------------------------------------

class Game_event_manager final {

public:
    Game_event_manager(Game_event_manager const&) = delete;
    Game_event_manager& operator=(Game_event_manager const&) = delete;
    Game_event_manager(Game_event_manager&&) = delete;
    Game_event_manager& operator=(Game_event_manager&&) = delete;

    static Game_event_manager& instance();

    void emit(
        Game_event_type const& type,
        Game_event_data const& data = {}
        );

    std::uint64_t register_callback(
        Game_event_type const& type,
        Game_callback const& callback,
        std::uint64_t const listener_id
        );

    void deregister_callback(std::uint64_t const callback_id);

    void deregister_listener(std::uint64_t const listener_id);

    void deregister_type(Game_event_type const& type);

    void clear();

private:
    Game_event_manager();

    ~Game_event_manager() = default;

    struct Callback_entry {

        std::uint64_t callback_id;
        std::uint64_t listener_id;
        Game_callback callback;
    };

    std::unordered_map<Game_event_type, std::vector<Callback_entry>> _callbacks;
    std::uint64_t _next_callback_id;
};

}
