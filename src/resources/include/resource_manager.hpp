#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "spritesheet.hpp"
#include "utils/include/logger.hpp"

#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include <memory>
#include <string_view>
#include <unordered_map>
#include <filesystem>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Class Resource_manager
// ----------------------------------------------------------------------------

class Resource_manager final {

public:
    Resource_manager(Resource_manager const&) = delete;
    Resource_manager& operator=(Resource_manager const&) = delete;
    Resource_manager(Resource_manager&&) = delete;
    Resource_manager& operator=(Resource_manager&&) = delete;

    static Resource_manager& instance();

    template<typename T>
    std::shared_ptr<T> load(
        std::string_view const name,
        std::filesystem::path const& path
        );

    template<typename T>
    std::shared_ptr<T> get(std::string_view const name);

    void unload(std::string_view const name);

    void flush_unused();

    void clear();

private:
    Resource_manager() = default;

    ~Resource_manager() = default;

    std::unordered_map<std::string_view, std::shared_ptr<void>> _resources;
    std::unordered_map<std::string_view, std::shared_ptr<sf::Music>> _music;
    std::unordered_map<std::string_view, std::shared_ptr<sf::SoundBuffer>> _sound_buffers;
};

// ============================================================================
// Template implementations
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
template<typename T>
std::shared_ptr<T> Resource_manager::load(
    std::string_view const name,
    std::filesystem::path const& path
    ) {

    using namespace battleships::utils;

    auto it = _resources.find(name);
    if (it != _resources.end()) {

        return std::static_pointer_cast<T>(it->second);
    }

    auto resource = std::make_shared<T>();

    if (!resource->loadFromFile(path.string())) {

        LOG(Log_lvl::ERR) << "Failed to load resource: " << path.string();
        return {};
    }

    _resources[name] = resource;
    return resource;
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<sf::Font> Resource_manager::load<sf::Font>(
    std::string_view const name,
    std::filesystem::path const& path
    ) {

    using namespace battleships::utils;

    auto it = _resources.find(name);
    if (it != _resources.end()) {

        return std::static_pointer_cast<sf::Font>(it->second);
    }

    auto resource = std::make_shared<sf::Font>();

    if (!resource->openFromFile(path.string())) {

        LOG(Log_lvl::ERR) << "Failed to open font: " << path.string();
        return {};
    }

    _resources[name] = resource;
    return resource;
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<Static_spritesheet> Resource_manager::load<Static_spritesheet>(
    std::string_view const name,
    std::filesystem::path const& path
    ) {

    using namespace battleships::utils;

    auto it = _resources.find(name);
    if (it != _resources.end()) {

        return std::static_pointer_cast<Static_spritesheet>(it->second);
    }

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path.string())) {

        LOG(Log_lvl::ERR) << "Failed to load spritesheet texture: " << path.string();
        return {};
    }

    auto spritesheet = std::make_shared<Static_spritesheet>(std::move(texture));
    _resources[name] = spritesheet;
    return spritesheet;
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<Animated_spritesheet> Resource_manager::load<Animated_spritesheet>(
    std::string_view const name,
    std::filesystem::path const& path
    ) {

    using namespace battleships::utils;

    auto it = _resources.find(name);
    if (it != _resources.end()) {

        return std::static_pointer_cast<Animated_spritesheet>(it->second);
    }

    auto texture = std::make_shared<sf::Texture>();
    if (!texture->loadFromFile(path.string())) {

        LOG(Log_lvl::ERR) << "Failed to load spritesheet texture: " << path.string();
        return {};
    }

    auto spritesheet = std::make_shared<Animated_spritesheet>(std::move(texture));
    _resources[name] = spritesheet;
    return spritesheet;
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<sf::Music> Resource_manager::load<sf::Music>(
    std::string_view const name,
    std::filesystem::path const& path
    ) {

    using namespace battleships::utils;

    auto it = _music.find(name);
    if (it != _music.end()) {

        return it->second;
    }

    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile(path.string())) {

        LOG(Log_lvl::ERR) << "Failed to open music: " << path.string();
        return {};
    }

    _music[name] = music;
    return music;
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<sf::SoundBuffer> Resource_manager::load<sf::SoundBuffer>(
    std::string_view const name,
    std::filesystem::path const& path
    ) {

    using namespace battleships::utils;

    auto it = _sound_buffers.find(name);
    if (it != _sound_buffers.end()) {

        return it->second;
    }

    auto buffer = std::make_shared<sf::SoundBuffer>();
    if (!buffer->loadFromFile(path.string())) {

        LOG(Log_lvl::ERR) << "Failed to load sound buffer: " << path.string();
        return {};
    }

    _sound_buffers[name] = buffer;
    return buffer;
}

// ----------------------------------------------------------------------------
template<typename T>
std::shared_ptr<T> Resource_manager::get(
    std::string_view const name
    ) {

    using namespace battleships::utils;

    auto it = _resources.find(name);
    if (it != _resources.end()) {

        return std::static_pointer_cast<T>(it->second);
    }

    LOG(Log_lvl::ERR) << "Resource not found: " << name;
    return {};
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<sf::Music> Resource_manager::get<sf::Music>(
    std::string_view const name
    ) {

    using namespace battleships::utils;

    auto it = _music.find(name);
    if (it != _music.end()) {

        return it->second;
    }

    LOG(Log_lvl::ERR) << "Music not found: " << name;
    return {};
}

// ----------------------------------------------------------------------------
template<>
inline std::shared_ptr<sf::SoundBuffer> Resource_manager::get<sf::SoundBuffer>(
    std::string_view const name
    ) {

    using namespace battleships::utils;

    auto it = _sound_buffers.find(name);
    if (it != _sound_buffers.end()) {

        return it->second;
    }

    LOG(Log_lvl::ERR) << "Sound buffer not found: " << name;
    return {};
}

}
