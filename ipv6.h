
#include <iostream>
#include <string>
#include <array>

class IPv6 final
{
public:
    IPv6(uint16_t v1 = 0, uint16_t v2 = 0, uint16_t v3 = 0, uint16_t v4 = 0, uint16_t v5 = 0, uint16_t v6 = 0, uint16_t v7 = 0, uint16_t v8 = 0):
        mAddr{v1, v2, v3, v4, v5, v6, v7, v8} {}
    ~IPv6() = default;

    IPv6(IPv6 const&) = default;
    IPv6(IPv6&&) = default;
    IPv6& operator=(IPv6 const&) = default;
    IPv6& operator=(IPv6&&) = default;

    bool empty() const; // all parts == 0

    bool operator==(IPv6 const& rhv) const { return mAddr == rhv.mAddr; }
    bool operator!=(IPv6 const& rhv) const { return !operator==(rhv); }

    static IPv6 from_string(std::string const& str_ip); // in case of error throw runtime_error
    std::string to_string() const;

    static std::string to_string(uint16_t a_num);

    std::array<uint16_t, 8> mAddr = {0};
};

std::ostream& operator<<(std::ostream& os, IPv6 const& addr);


