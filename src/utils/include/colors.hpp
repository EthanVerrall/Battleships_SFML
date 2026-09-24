#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::utils {

namespace colors {

//Defaults
inline constexpr sf::Color DEFAULT_BACKGROUND(sf::Color::Black);
inline constexpr sf::Color DEFAULT_BORDER(sf::Color::Black);
inline constexpr sf::Color DEFAULT_TEXT(sf::Color::White);

//Headings
inline constexpr sf::Color HEADING_BORDER(sf::Color::Yellow);
inline constexpr sf::Color HEADING_TEXT(sf::Color::Red);

//Buttons
inline constexpr sf::Color BUTTON_TEXT_HOVER(sf::Color::Yellow);

}

}