#include "ipv6.h"

#include <vector>

bool IPv6::empty() const
{
    for(uint16_t const a_part: mAddr) {
        if(a_part != 0) {
            return false;
        }
    }
    return true;
}

IPv6 IPv6::from_string(std::string const& str_ip)
{
    if(str_ip.empty()) {
        throw std::runtime_error("Invalid IPv6 string - empty");
    }
    std::vector<char> vec_ip(str_ip.begin(), str_ip.end());
    IPv6 res;
    size_t start_end = 0;
    vec_ip.push_back('\0');
    char* p_next = &vec_ip[0];
    if(*p_next != ':') {
        for(; (*p_next != '\0') && (*p_next != ':') && (start_end < res.mAddr.size()); ++p_next) {
            char const* const p_start = p_next;
            unsigned long const val = strtoul(p_start, &p_next, 16);
            if((errno == ERANGE)) {
                errno = 0;
                throw std::runtime_error("Invalid IPv6 string - found invalid value");
            }
            if(p_next == p_start) {
                throw std::runtime_error("Invalid IPv6 string - found invalid component");
            }
            if(val > UINT16_MAX) {
                throw std::runtime_error("Invalid IPv6 string - found invalid value");
            }
            res.mAddr[start_end++] = val;
            if(*p_next == '\0') {
                break;
            }
            if(*p_next != ':') {
                throw std::runtime_error("Invalid IPv6 string - found invalid character");
            }
        }
    } else {
        ++p_next;
    }
    if(*p_next == '\0') {
        if(start_end != 8) {
            throw std::runtime_error("Invalid IPv6 string - invalid number of component");
        }
        return res;
    }
    if(*p_next != ':') {
        throw std::runtime_error("Invalid IPv6 string - found invalid character");
    }
    ++p_next;
    std::vector<uint16_t> end_res;
    if(*p_next != '\0') {
        for(; ; ++p_next) {
            char const* const p_start = p_next;
            unsigned long const val = strtoul(p_start, &p_next, 16);
            if((errno == ERANGE)) {
                errno = 0;
                throw std::runtime_error("Invalid IPv6 string - found invalid value");
            }
            if(p_next == p_start) {
                throw std::runtime_error("Invalid IPv6 string - found invalid component");
            }
            if(val > UINT16_MAX) {
                throw std::runtime_error("Invalid IPv6 string - found invalid value");
            }
            end_res.push_back(val);
            if(*p_next == '\0') {
                break;
            }
            if(*p_next != ':') {
                throw std::runtime_error("Invalid IPv6 string - found invalid character");
            }
        }
    }
    if((start_end + end_res.size()) > 7) {
        throw std::runtime_error("Invalid IPv6 string - invalid number of component");
    }
    std::copy(std::begin(end_res), std::end(end_res), std::begin(res.mAddr) + (res.mAddr.size() - end_res.size()));
    return res;
}

std::string IPv6::to_string(uint16_t const val)
{
    std::string res(4, '\0');
    for (int i = 0; i < 4; ++i) {
        uint8_t const a_part = ((val >> (12 - i*4)) & 0xf);
        if(a_part > 9) {
            res[i] = a_part - 0xa + 'a';
        } else {
            res[i] = a_part + '0';
        }
    }
    if(res[0] != '0') {
        return res;
    }
    int non_0_pos = 1;
    for(; (non_0_pos < 3) && (res[non_0_pos] == '0'); ++non_0_pos);
    return res.substr(non_0_pos);
}

std::string IPv6::to_string() const
{
    std::string res;
    size_t longest_zeros = 0;
    size_t longest_zeros_pos = 0;
    size_t current_zeros = 0;
    size_t current_zeros_pos = 0;
    for (size_t i = 0; i < mAddr.size(); ++i) { // found part to skip
        if(mAddr[i] == 0) {
            if(current_zeros == 0) {
                current_zeros_pos = i;
            }
            ++current_zeros;
            if(current_zeros > longest_zeros) {
                longest_zeros = current_zeros;
                longest_zeros_pos = current_zeros_pos;
            }
        } else if (current_zeros > 1) {
            current_zeros = 0;
        }
    }
    bool add_delim = false;
    for (size_t i = 0; i < mAddr.size(); ++i) {
        if((i == longest_zeros_pos) && (longest_zeros > 1)) { // do not skip group of single zero
            res.append("::");
            add_delim = false;
            i += (longest_zeros - 1);
            continue;
        }
        if(add_delim) {
            res.append(":");
        }
        add_delim = true;
        std::string const comp = to_string(mAddr[i]);
        res.append(comp);
    }
    return res;
}

std::ostream& operator<<(std::ostream& os, IPv6 const& addr)
{
    os << addr.to_string();
    return os;
}
