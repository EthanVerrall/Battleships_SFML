#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "defs.hpp"
#include "utils/include/logger.hpp"

#include "SFML/Graphics.hpp"

#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::settings {

// ============================================================================
// Class Settings_manager
// ----------------------------------------------------------------------------

class Settings_manager final {

    //--------------------------
    // Constructor / Destructor
    //--------------------------
public:

    Settings_manager(Settings_manager const&) = delete;
    Settings_manager& operator=(Settings_manager const&) = delete;
    Settings_manager(Settings_manager&&) = delete;
    Settings_manager& operator=(Settings_manager&&) = delete;

private:
    Settings_manager() = default;

    //--------------------------
    // Class specific functions
    //--------------------------
public:

    static Settings_manager& instance();

    void load();

    void save();

    void clear();

    bool has(std::string_view const key) const;

private:

    void _emit_event___settings_changed(std::string_view const key);

    void _emit_event___settings_saved();

    Setting_key_entry const* _resolve_key(std::string_view const key) const;

    //--------------------------
    // Getters
    //--------------------------
public:

    template<typename T>
    T get(std::string_view const key) const;

    //--------------------------
    // Setters
    //--------------------------
public:

    template<typename T>
    void set(std::string_view const key, T const value);

    //--------------------------
    // Attributes
    //--------------------------
private:

    std::unordered_map<std::string_view, Setting_value> _settings;
    std::unordered_set<std::string_view> _changed_keys;
};

// ============================================================================
// Template implementations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
template<typename T>
T Settings_manager::get(
    std::string_view const key
    ) const {

    using namespace battleships::utils;

    auto const* entry = _resolve_key(key);

    if (!entry) {

        LOG(Log_lvl::ERR) << "Setting not registered: " << key;
        return {};
    }

    if (!std::holds_alternative<T>(entry->type)) {

        LOG(Log_lvl::ERR) << "Setting '" << key
            << "' was declared with a different type";
        return {};
    }

    auto const it = _settings.find(entry->name);

    if (it == _settings.cend()) {

        LOG(Log_lvl::WARN) << "Could not get setting '" <<
            key << "', not found in map";
        return {};
    }

    return std::get<T>(it->second);
}

// ----------------------------------------------------------------------------
template<typename T>
void Settings_manager::set(
    std::string_view const key,
    T const value
    ) {

    auto const* entry = _resolve_key(key);

    if (!entry) {

        LOG(battleships::utils::Log_lvl::ERR) << "Setting not registered: " << key;
        return;
    }

    if (!std::holds_alternative<T>(entry->type)) {

        LOG(battleships::utils::Log_lvl::ERR) << "Setting '"
            << key << "' was declared with a different type";
        return;
    }

    auto const it = _settings.find(entry->name);

    if (it == _settings.end()) {

        LOG(battleships::utils::Log_lvl::ERR) << "Could not set setting '"
            << key << "', not found in map";
        return;
    }

    if (std::get<T>(it->second) == value) {

        // Trying to set a setting to the same value
        return;
    }

    it->second = Setting_value(value);

    _changed_keys.insert(entry->name);

    _emit_event___settings_changed(entry->name);
}

}
