// VitalComm — SIL-minded message bus. MIT License.
#include "vitalcomm/crc32.hpp"

#include <cstdint>
#include <cstdio>
#include <string_view>

int main() {
    constexpr std::string_view msg{"VitalComm hello, world"};
    const auto c = vitalcomm::crc32(
        reinterpret_cast<const std::uint8_t*>(msg.data()), msg.size());
    std::printf("crc32(\"%.*s\") = 0x%08X\n",
                static_cast<int>(msg.size()), msg.data(), c);
    return 0;
}
