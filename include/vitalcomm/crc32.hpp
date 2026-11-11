// VitalComm — SIL-minded message bus. MIT License.
#ifndef VITALCOMM_CRC32_HPP
#define VITALCOMM_CRC32_HPP

#include <cstddef>
#include <cstdint>

namespace vitalcomm {

/// Compute CRC-32 (polynomial 0xEDB88320, reflected, init 0xFFFFFFFF,
/// final XOR 0xFFFFFFFF) — the variant used by Ethernet, PNG and RFC 1952.
///
/// Deterministic, no allocation, no exceptions, no dependency on the
/// heap. Suitable for safety-adjacent use in bounded time.
///
/// @param data   pointer to the buffer; may be nullptr iff @p size is 0
/// @param size   number of bytes to hash
/// @return       the CRC-32 value
[[nodiscard]] std::uint32_t crc32(const std::uint8_t* data,
                                  std::size_t size) noexcept;

/// Convenience overload for continuing a running CRC across chunks.
[[nodiscard]] std::uint32_t crc32_update(std::uint32_t seed,
                                         const std::uint8_t* data,
                                         std::size_t size) noexcept;

}  // namespace vitalcomm

#endif  // VITALCOMM_CRC32_HPP
