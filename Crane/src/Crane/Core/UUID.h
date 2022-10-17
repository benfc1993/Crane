#pragma once

#include <stdint.h>
#include <functional>

namespace Crane {
    class UUID
    {
    public:
        UUID();
        UUID(uint64_t uuid);
        UUID(const UUID& uuid) = default;

        operator uint64_t() const { return m_UUID; }
    private:
        uint64_t m_UUID;

    };
}

namespace std {

    template<>
    struct hash<Crane::UUID>
    {
        std::size_t operator()(const Crane::UUID& uuid) const
        {
            return hash<uint64_t>()((uint64_t)uuid);
        }
    };

}