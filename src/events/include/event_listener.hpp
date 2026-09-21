#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include <cstdint>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::events {

// ============================================================================
// Class Event_listener
// ----------------------------------------------------------------------------

class Event_listener {

public:
    virtual ~Event_listener() = default;

protected:
    Event_listener()
        : _listener_id(_generate_id())
    {}

    std::uint64_t listener_id() const { return _listener_id; }

private:
    static std::uint64_t _generate_id() {

        static std::uint64_t id = 0u;
        return id++;
    }

    std::uint64_t _listener_id;
};

}
