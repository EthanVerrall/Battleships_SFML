#pragma once

// ============================================================================
// Includes
// ----------------------------------------------------------------------------

#include <algorithm>
#include <concepts>

// ============================================================================
// Namespaces
// ----------------------------------------------------------------------------

namespace battleships::utils {

namespace math {

// ============================================================================
// Functions
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Linearly interpolates between min and max by t, where t is typically in
// [0, 1] (t outside that range extrapolates past min/max instead of clamping)
// ----------------------------------------------------------------------------
template <std::floating_point Type>
inline constexpr Type lerp(
    Type const min,
    Type const max,
    Type const t
    ) {

    return min + ((max - min) * t);
}

// ----------------------------------------------------------------------------
// Returns how far value lies between min and max, clamped to [0, 1].
// The inverse of lerp
// ----------------------------------------------------------------------------
template <std::floating_point Type>
inline constexpr Type inverse_lerp(
    Type const min,
    Type const max,
    Type const value
    ) {

    if (min == max) {

        return Type{0};
    }

    return std::clamp((value - min) / (max - min), Type{0}, Type{1});
}

}

}
