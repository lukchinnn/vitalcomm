// VitalComm — SIL-minded message bus. MIT License.
#include "vitalcomm/crc32.hpp"

#include <gtest/gtest.h>

#include <array>
#include <cstdint>
#include <string_view>

namespace {

using vitalcomm::crc32;

std::uint32_t crc_of(std::string_view s) {
    return crc32(reinterpret_cast<const std::uint8_t*>(s.data()), s.size());
}

TEST(Crc32, EmptyInputMatchesReference) {
    EXPECT_EQ(crc32(nullptr, 0U), 0x00000000U);
}

TEST(Crc32, KnownVector_123456789) {
    // Reference value from https://reveng.sourceforge.io/ and RFC 1952.
    EXPECT_EQ(crc_of("123456789"), 0xCBF43926U);
}

TEST(Crc32, KnownVector_Alphabet) {
    EXPECT_EQ(crc_of("abcdefghijklmnopqrstuvwxyz"), 0x4C2750BDU);
}

TEST(Crc32, SingleByteIsDeterministic) {
    const std::uint8_t b = 0x42;
    EXPECT_EQ(crc32(&b, 1U), crc32(&b, 1U));
}

}  // namespace
