// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/settings_manager.hpp"
#include "include/toml_codec.hpp"
#include "events/include/game_event_manager.hpp"
#include "events/include/event_type.hpp"
#include "utils/include/logger.hpp"

#include <toml++/toml.hpp>

#include <algorithm>
#include <fstream>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::settings {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::utils;
using namespace battleships::events;

// ============================================================================
// Class Settings_manager
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
Settings_manager& Settings_manager::instance() {

    static Settings_manager instance;
    return instance;
}

// ----------------------------------------------------------------------------
void Settings_manager::load() {

    toml::table table;

    try {

        table = toml::parse_file(SETTINGS_FILE_PATH);

    } catch (toml::parse_error const& err) {

        LOG(Log_lvl::WARN) << "Could not load settings file '" << SETTINGS_FILE_PATH
            << "': " << err.what();
        return;
    }

    for (auto const& entry : SETTING_KEYS) {

        auto const node = table.at_path(entry.name);

        if (!node) { continue; }

        auto const decode_and_store = [&](auto type_tag) {

            using T = decltype(type_tag);

            _settings[entry.name] = Setting_value(Toml_codec<T>::decode(node));
            };

        std::visit(decode_and_store, entry.type);
    }
}

// ----------------------------------------------------------------------------
void Settings_manager::save() const {

    toml::table table;

    for (auto const& [key, value] : _settings) {

        auto const encode_and_insert = [&](auto const& held) {

            using T = std::decay_t<decltype(held)>;

            table.insert_or_assign(key, Toml_codec<T>::encode(held));
            };

        std::visit(encode_and_insert, value);
    }

    std::ofstream file(SETTINGS_FILE_PATH, std::ios::trunc);

    if (!file) {

        LOG(Log_lvl::ERR) << "Could not open '" << SETTINGS_FILE_PATH << "' for writing";
        return;
    }

    file << table;
}

// ----------------------------------------------------------------------------
void Settings_manager::clear() {

    _settings.clear();
}

// ----------------------------------------------------------------------------
bool Settings_manager::has(
    std::string_view const key
    ) const {

    return _settings.contains(key);
}

// ----------------------------------------------------------------------------
Setting_key_entry const* Settings_manager::_resolve_key(
    std::string_view const key
    ) const {

    auto const it = std::find_if(
        std::begin(SETTING_KEYS),
        std::end(SETTING_KEYS),
        [key](Setting_key_entry const& entry) { return entry.name == key; }
        );

    if (it == std::end(SETTING_KEYS)) {

        return nullptr;
    }

    return &*it;
}

// ----------------------------------------------------------------------------
void Settings_manager::_emit_event___settings_changed(
    std::string_view const key
    ) {

    Game_event_type const event = Game_event_type::SETTINGS_CHANGED;

    LOG(Log_lvl::TRACE) << "Emitting event: " << event;

    auto& game_event_manager = Game_event_manager::instance();

    game_event_manager.emit(
        event,
        Game_event_data{ .payload = key }
        );
}

}
