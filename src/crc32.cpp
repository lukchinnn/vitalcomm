// VitalComm — SIL-minded message bus. MIT License.
#include "vitalcomm/crc32.hpp"

#include <array>

namespace vitalcomm {
namespace {

// Pre-computed lookup table, generated at compile time so we pay no
// runtime initialisation cost and the table lives in .rodata.
constexpr std::array<std::uint32_t, 256> make_table() noexcept {
    std::array<std::uint32_t, 256> t{};
    constexpr std::uint32_t kPoly = 0xEDB88320U;
    for (std::uint32_t i = 0; i < 256; ++i) {
        std::uint32_t c = i;
        for (int k = 0; k < 8; ++k) {
            c = (c & 1U) ? (kPoly ^ (c >> 1)) : (c >> 1);
        }
        t[i] = c;
    }
    return t;
}

constexpr auto kTable = make_table();

}  // namespace

std::uint32_t crc32(const std::uint8_t* data, std::size_t size) noexcept {
    return crc32_update(0xFFFFFFFFU, data, size) ^ 0xFFFFFFFFU;
}

std::uint32_t crc32_update(std::uint32_t seed,
                           const std::uint8_t* data,
                           std::size_t size) noexcept {
    std::uint32_t c = seed;
    for (std::size_t i = 0; i < size; ++i) {
        const std::uint8_t idx =
            static_cast<std::uint8_t>((c ^ data[i]) & 0xFFU);
        c = kTable[idx] ^ (c >> 8);
    }
    return c;
}

}  // namespace vitalcomm
