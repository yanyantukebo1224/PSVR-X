#include "psvrx/stereo_frame.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

int main() {
    // 4x2 RGB frame: each row is 4 pixels * 3 bytes.
    std::array<std::uint8_t, 24> pixels{};
    psvrx::ImageView packed{pixels.data(), 4, 2, 3, 12};
    psvrx::StereoFrame stereo{};

    assert(psvrx::split_side_by_side(packed, stereo));
    assert(stereo.left.width == 2);
    assert(stereo.right.width == 2);
    assert(stereo.left.height == 2);
    assert(stereo.right.height == 2);
    assert(stereo.left.data == pixels.data());
    assert(stereo.right.data == pixels.data() + 6);

    // Reject odd widths and null frames.
    packed.width = 3;
    assert(!psvrx::split_side_by_side(packed, stereo));

    packed.width = 4;
    packed.data = nullptr;
    assert(!psvrx::split_side_by_side(packed, stereo));

    // Reject a stride too short to contain a row.
    packed.data = pixels.data();
    packed.stride_bytes = 5;
    assert(!psvrx::split_side_by_side(packed, stereo));

    std::cout << "PSVR-X core tests passed\n";
    return 0;
}
