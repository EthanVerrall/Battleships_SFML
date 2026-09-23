// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "include/asset_registry.hpp"
#include "include/resource_manager.hpp"
#include "utils/include/logger.hpp"

#include <algorithm>
#include <array>
#include <filesystem>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::resources {

// ============================================================================
// Using directives
// ----------------------------------------------------------------------------

using namespace battleships::utils;

// ============================================================================
// Data
// ----------------------------------------------------------------------------

namespace {

    // ------------------------------------------------------------------------
    // Static spritesheets
    // ------------------------------------------------------------------------

    inline constexpr std::array BATTLESHIP_HULL_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 31.0f, 209.0f } } },
    };

    inline constexpr std::array BATTLESHIP_GUN_REGIONS = {

        Static_region_entry{ .name = "gun", .rect = { { 0.0f, 0.0f }, { 25.0f, 50.0f } } },
    };

    inline constexpr std::array CARRIER_HULL_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 57.0f, 189.0f } } },
    };

    inline constexpr std::array CRUISER_HULL_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 23.0f, 128.0f } } },
    };

    inline constexpr std::array CRUISER_STSM_REGIONS = {

        Static_region_entry{ .name = "stsm", .rect = { { 0.0f, 0.0f }, { 11.0f, 12.0f } } },
    };

    inline constexpr std::array DESTROYER_HULL_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 20.0f, 100.0f } } },
    };

    inline constexpr std::array DESTROYER_GUN_REGIONS = {

        Static_region_entry{ .name = "gun", .rect = { { 0.0f, 0.0f }, { 15.0f, 26.0f } } },
    };

    inline constexpr std::array PATROL_BOAT_HULL_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 13.0f, 55.0f } } },
    };

    inline constexpr std::array PLANE_MISSILE_REGIONS = {

        Static_region_entry{ .name = "missile", .rect = { { 0.0f, 0.0f }, { 5.0f, 9.0f } } },
    };

    inline constexpr std::array PLANE_F35_REGIONS = {

        Static_region_entry{ .name = "f35", .rect = { { 0.0f, 0.0f }, { 35.0f, 45.0f } } },
    };

    inline constexpr std::array RESCUE_SHIP_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 17.0f, 100.0f } } },
    };

    inline constexpr std::array SUBMARINE_HULL_REGIONS = {

        Static_region_entry{ .name = "hull", .rect = { { 0.0f, 0.0f }, { 35.0f, 142.0f } } },
    };

    inline constexpr std::array SUBMARINE_WEAPON_REGIONS = {

        Static_region_entry{ .name = "torpedo", .rect = { { 0.0f, 0.0f }, { 13.0f, 21.0f } } },
    };

    inline constexpr std::array BACKGROUND_REGIONS = {

        Static_region_entry{ .name = "background", .rect = { { 0.0f, 0.0f }, { 1920.0f, 1080.0f } } },
    };

    inline constexpr std::array STATIC_SHEETS = {

        Static_sheet_entry{ .name = "battleship_hull", .path = "assets/textures/battleship/ship_battleship_hull.png", .regions = BATTLESHIP_HULL_REGIONS },
        Static_sheet_entry{ .name = "battleship_gun", .path = "assets/textures/battleship/weapon_battleship_standard_gun.png", .regions = BATTLESHIP_GUN_REGIONS },
        Static_sheet_entry{ .name = "carrier_hull", .path = "assets/textures/carrier/ship_carrier_hull.png", .regions = CARRIER_HULL_REGIONS },
        Static_sheet_entry{ .name = "cruiser_hull", .path = "assets/textures/cruiser/ship_cruiser_hull.png", .regions = CRUISER_HULL_REGIONS },
        Static_sheet_entry{ .name = "cruiser_stsm", .path = "assets/textures/cruiser/weapon_cruiser_standard_STSM.png", .regions = CRUISER_STSM_REGIONS },
        Static_sheet_entry{ .name = "destroyer_hull", .path = "assets/textures/destroyer/ship_destroyer_hull.png", .regions = DESTROYER_HULL_REGIONS },
        Static_sheet_entry{ .name = "destroyer_gun", .path = "assets/textures/destroyer/weapon_destroyer_standard_gun.png", .regions = DESTROYER_GUN_REGIONS },
        Static_sheet_entry{ .name = "patrol_boat_hull", .path = "assets/textures/patrol_boat/ship_patrol_hull.png", .regions = PATROL_BOAT_HULL_REGIONS },
        Static_sheet_entry{ .name = "plane_missile", .path = "assets/textures/plane/missile.png", .regions = PLANE_MISSILE_REGIONS },
        Static_sheet_entry{ .name = "plane_f35", .path = "assets/textures/plane/planeF35_lightning_2.png", .regions = PLANE_F35_REGIONS },
        Static_sheet_entry{ .name = "rescue_ship_hull", .path = "assets/textures/rescue_ship/ship_rescue.png", .regions = RESCUE_SHIP_REGIONS },
        Static_sheet_entry{ .name = "submarine_hull", .path = "assets/textures/submarine/ship_sub_marine_hull.png", .regions = SUBMARINE_HULL_REGIONS },
        Static_sheet_entry{ .name = "submarine_weapon", .path = "assets/textures/submarine/weapon_submarine_standard.png", .regions = SUBMARINE_WEAPON_REGIONS },

        Static_sheet_entry{ .name = "background_coast", .path = "assets/textures/backgrounds/coast.png", .regions = BACKGROUND_REGIONS },
        Static_sheet_entry{ .name = "background_storm", .path = "assets/textures/backgrounds/storm.png", .regions = BACKGROUND_REGIONS },
        Static_sheet_entry{ .name = "background_sunset", .path = "assets/textures/backgrounds/sunset.png", .regions = BACKGROUND_REGIONS },
    };

    // ------------------------------------------------------------------------
    // Animated spritesheets
    // ------------------------------------------------------------------------

    inline constexpr std::array ANIMATED_SHEETS = std::array<Animated_sheet_entry, 0>{};

    // ------------------------------------------------------------------------
    // Audio
    // ------------------------------------------------------------------------

    inline constexpr std::array AUDIO = {

        Audio_entry{ .name = "engines_of_war", .path = "assets/music/Engines of War.ogg" },
        Audio_entry{ .name = "fate_of_generation", .path = "assets/music/Fate of a Generation.ogg" },
        Audio_entry{ .name = "iron_skies", .path = "assets/music/Iron Skies.ogg" },
        Audio_entry{ .name = "steel_and_resolve", .path = "assets/music/Steel and Resolve.ogg" },
        Audio_entry{ .name = "the_price_of_freedom", .path = "assets/music/The Price of Freedom.ogg" },
        Audio_entry{ .name = "the_waiting_hour", .path = "assets/music/The Waiting Hour.ogg" },
        Audio_entry{ .name = "victory_at_a_cost", .path = "assets/music/Victory at a Cost.ogg" },
        Audio_entry{ .name = "world_at_war", .path = "assets/music/World at War.ogg" }
    };

    // ------------------------------------------------------------------------
    // Sound effects
    // ------------------------------------------------------------------------

    inline constexpr std::array SOUNDS = {

        Sound_entry{ .name = "click", .path = "assets/sfx/click.wav" },
        Sound_entry{ .name = "hover", .path = "assets/sfx/hover.wav" },
        Sound_entry{ .name = "launching_missile", .path = "assets/sfx/launching_missile.mp3" },
        Sound_entry{ .name = "missile_impact", .path = "assets/sfx/missile_impact.mp3" },
        Sound_entry{ .name = "water_splash", .path = "assets/sfx/water_splash.mp3" },
    };

    // ------------------------------------------------------------------------
    // Fonts
    // ------------------------------------------------------------------------

    inline constexpr std::array FONTS = {

        Font_entry{ .name = "pixel_bold", .path = "assets/fonts/Pixel_bold.ttf" }
    };

    // ------------------------------------------------------------------------
    // Lookup helpers
    // ------------------------------------------------------------------------

    template<typename Entries>
    auto const* find_entry(
        Entries const& entries,
        std::string_view const name
        ) {

        auto const it = std::find_if(
            entries.begin(),
            entries.end(),
            [name](auto const& entry) { return entry.name == name; }
            );

        return it != entries.end() ? &*it : nullptr;
    }
}

// ============================================================================
// Class Asset_registry
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
std::shared_ptr<Static_spritesheet> Asset_registry::load_static_spritesheet(
    std::string_view const name
    ) {

    auto const* entry = find_entry(STATIC_SHEETS, name);
    if (!entry) {

        LOG(Log_lvl::ERR) << "Static spritesheet not registered: " << name;
        return {};
    }

    auto sheet = Resource_manager::instance().load<Static_spritesheet>(entry->name, std::filesystem::path{ entry->path });
    if (!sheet) {

        return {};
    }

    for (auto const& region : entry->regions) {

        sheet->add_region(region);
    }

    return sheet;
}

// ----------------------------------------------------------------------------
std::shared_ptr<Animated_spritesheet> Asset_registry::load_animated_spritesheet(
    std::string_view const name
    ) {

    auto const* entry = find_entry(ANIMATED_SHEETS, name);
    if (!entry) {

        LOG(Log_lvl::ERR) << "Animated spritesheet not registered: " << name;
        return {};
    }

    auto sheet = Resource_manager::instance().load<Animated_spritesheet>(entry->name, std::filesystem::path{ entry->path });
    if (!sheet) {

        return {};
    }

    for (auto const& animation : entry->animations) {

        sheet->add_animation(animation);
    }

    return sheet;
}

// ----------------------------------------------------------------------------
std::shared_ptr<sf::Music> Asset_registry::load_music(
    std::string_view const name
    ) {

    auto const* entry = find_entry(AUDIO, name);
    if (!entry) {

        LOG(Log_lvl::ERR) << "Music not registered: " << name;
        return {};
    }

    return Resource_manager::instance().load<sf::Music>(entry->name, std::filesystem::path{ entry->path });
}

// ----------------------------------------------------------------------------
std::shared_ptr<sf::SoundBuffer> Asset_registry::load_sound(
    std::string_view const name
    ) {

    auto const* entry = find_entry(SOUNDS, name);
    if (!entry) {

        LOG(Log_lvl::ERR) << "Sound not registered: " << name;
        return {};
    }

    return Resource_manager::instance().load<sf::SoundBuffer>(entry->name, std::filesystem::path{ entry->path });
}

// ----------------------------------------------------------------------------
std::shared_ptr<sf::Font> Asset_registry::load_font(
    std::string_view const name
    ) {

    auto const* entry = find_entry(FONTS, name);
    if (!entry) {

        LOG(Log_lvl::ERR) << "Font not registered: " << name;
        return {};
    }

    return Resource_manager::instance().load<sf::Font>(entry->name, std::filesystem::path{ entry->path });
}

}
