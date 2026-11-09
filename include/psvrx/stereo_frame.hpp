#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace psvrx {

struct ImageView {
    const std::uint8_t* data{};
    int width{};
    int height{};
    int channels{};
    std::size_t stride_bytes{};
};

struct StereoFrame {
    ImageView left;
    ImageView right;
};

// Splits a packed side-by-side image into non-owning left/right views.
// The input must have an even width and tightly interleaved pixels per row.
// Returns false for invalid dimensions, channels, stride, or null data.
bool split_side_by_side(const ImageView& packed, StereoFrame& output) noexcept;

} // namespace psvrx
