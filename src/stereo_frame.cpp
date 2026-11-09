#include "psvrx/stereo_frame.hpp"

#include <limits>

namespace psvrx {

bool split_side_by_side(const ImageView& packed, StereoFrame& output) noexcept {
    output = {};

    if (packed.data == nullptr || packed.width <= 0 || packed.height <= 0 ||
        packed.channels <= 0 || (packed.width % 2) != 0) {
        return false;
    }

    const auto min_stride =
        static_cast<std::size_t>(packed.width) *
        static_cast<std::size_t>(packed.channels);

    if (packed.stride_bytes < min_stride ||
        packed.stride_bytes > std::numeric_limits<std::size_t>::max() /
                                  static_cast<std::size_t>(packed.height)) {
        return false;
    }

    const int eye_width = packed.width / 2;
    const auto eye_stride =
        static_cast<std::size_t>(eye_width) *
        static_cast<std::size_t>(packed.channels);

    output.left = {
        packed.data,
        eye_width,
        packed.height,
        packed.channels,
        packed.stride_bytes
    };

    output.right = {
        packed.data + eye_stride,
        eye_width,
        packed.height,
        packed.channels,
        packed.stride_bytes
    };

    return true;
}

} // namespace psvrx
