#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include "SFML/Graphics.hpp"

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::utils {

namespace sizes {

//Headings
inline constexpr unsigned int HEADING_TEXT {160u};
inline constexpr float HEADING_BORDER {6.0f};

//Sections
inline constexpr unsigned int SECTIONS_TEXT {80u};
inline constexpr float SECTIONS_BORDER {3.0f};

//Buttons
inline constexpr unsigned int BUTTON_TEXT {65u};
inline constexpr float BUTTON_BORDER {4.0f};

//Row labels
inline constexpr unsigned int ROW_LABEL_TEXT {32u};
inline constexpr float ROW_LABEL_BORDER {2.0f};

//FPS counter
inline constexpr unsigned int FPS_COUNTER_TEXT {28u};
inline constexpr float FPS_COUNTER_BORDER {2.0f};

}

}