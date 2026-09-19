// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/resource_manager.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::utils;

// ============================================================================
// Class Resource_manager
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Resource_manager& Resource_manager::instance() {

    static Resource_manager instance;
    return instance;
}

// ----------------------------------------------------------------------------
void Resource_manager::unload(
    std::string_view const name
    ) {

    _resources.erase(name);
    _music.erase(name);
    _sound_buffers.erase(name);
}

// ----------------------------------------------------------------------------
void Resource_manager::flush_unused() {

    for (auto it = _resources.begin(); it != _resources.end();) {

        if (it->second.use_count() == 1) {

            it = _resources.erase(it);
        } else {

            ++it;
        }
    }

    for (auto it = _music.begin(); it != _music.end();) {

        if (it->second.use_count() == 1) {

            it = _music.erase(it);
        } else {

            ++it;
        }
    }

    for (auto it = _sound_buffers.begin(); it != _sound_buffers.end();) {

        if (it->second.use_count() == 1) {

            it = _sound_buffers.erase(it);
        } else {

            ++it;
        }
    }
}

// ----------------------------------------------------------------------------
void Resource_manager::clear() {

    _resources.clear();
    _music.clear();
    _sound_buffers.clear();
}

}
