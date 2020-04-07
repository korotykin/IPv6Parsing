#include "ipv6.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("IPv6 ctor")
{
    CHECK(0 == IPv6{}.mAddr[0]);
    CHECK(0 == IPv6{}.mAddr[1]);
    CHECK(0 == IPv6{}.mAddr[2]);
    CHECK(0 == IPv6{}.mAddr[3]);
    CHECK(0 == IPv6{}.mAddr[4]);
    CHECK(0 == IPv6{}.mAddr[5]);
    CHECK(0 == IPv6{}.mAddr[6]);
    CHECK(0 == IPv6{}.mAddr[7]);

    CHECK(IPv6{}.empty());

    CHECK(1 == IPv6{1, 2, 3, 4, 5, 6, 7, 8}.mAddr[0]);
    CHECK(2 == IPv6{1, 2, 3, 4, 5, 6, 7, 8}.mAddr[1]);
    CHECK(3 == IPv6{1, 2, 3, 4, 5, 6, 7, 8}.mAddr[2]);
    CHECK(4 == IPv6{1, 2, 3, 4, 5, 6, 7, 8}.mAddr[3]);
    CHECK(5 == IPv6{1, 2, 3, 4, 5, 6, 7 ,8}.mAddr[4]);
    CHECK(6 == IPv6{1, 2, 3, 4, 5, 6, 7, 8}.mAddr[5]);
    CHECK(7 == IPv6{1, 2, 3, 4, 5, 6, 7 ,8}.mAddr[6]);
    CHECK(8 == IPv6{1, 2, 3, 4, 5, 6, 7, 8}.mAddr[7]);
}

TEST_CASE("uint16_t to string")
{
    CHECK("0" == IPv6::to_string(0));
    CHECK("f" == IPv6::to_string(0xf));
    CHECK("c1" == IPv6::to_string(0xc1));
    CHECK("ab9" == IPv6::to_string(0xab9));
    CHECK("8de7" == IPv6::to_string(0x8de7));
}

TEST_CASE("IPv6 from string")
{
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00, 0x42, 0x8329} == IPv6::from_string("2001:0db8:0000:0000:0000:ff00:0042:8329"));
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00, 0x42, 0x8329} == IPv6::from_string("2001:0DB8:0000:0000:0000:FF00:0042:8329"));
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00, 0x42, 0x8329} == IPv6::from_string("2001:db8::ff00:42:8329"));
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00, 0x42, 0x8329} == IPv6::from_string("2001:DB8::FF00:42:8329"));
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00} == IPv6::from_string("2001:db8::ff00:0:0"));
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00} == IPv6::from_string("2001:DB8::FF00:0:0"));
    CHECK(IPv6{0x2001, 0xdb8, 0, 0, 0xff00} == IPv6::from_string("2001:db8:0:0:ff00::"));
    CHECK(IPv6{0x2001, 0xdb8} == IPv6::from_string("2001:db8::"));
    CHECK(IPv6{0, 0, 0, 0, 0, 0, 0, 1} == IPv6::from_string("0000:0000:0000:0000:0000:0000:0000:0001"));
    CHECK(IPv6{0, 0, 0, 0, 0, 0, 0, 1} == IPv6::from_string("::1"));
}

TEST_CASE("IPv6 invalid string")
{
    CHECK_THROWS_AS(IPv6::from_string(""), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("zero"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string(":::1"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:db8:::"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001::db8::"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:::db8::"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:db8"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:db8:"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:ffdb8:0:0:ff00::"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:0DB8:0000:0000:0000:FF00:0042:8329:1234"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:0DB8:0000:0000:0000:FF00:0042:8329::"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string(":2001:0DB8:0000:0000:0000:FF00:0042:8329"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:0DB8:0000:0000:0000:FF00:0042:"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:0DB8:0000:0000:0000:FF00:0042:  "), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("::2001:0DB8:0000:0000:0000:FF00:0042:8329"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:0DB8:0000:0000:0000:FF00:-42:8329"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("127.0.0.1"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:db8"), std::runtime_error);
    CHECK_THROWS_AS(IPv6::from_string("2001:db8:"), std::runtime_error);
}

TEST_CASE("IPv6 to string")
{
    CHECK("2001:db8::ff00:42:8329" == IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00, 0x42, 0x8329}.to_string());
    CHECK("2001:db8::ff00:0:0" == IPv6{0x2001, 0xdb8, 0, 0, 0, 0xff00}.to_string());
    CHECK("2001:db8:0:0:ff00::" == IPv6{0x2001, 0xdb8, 0, 0, 0xff00}.to_string());
    CHECK("2001:db8::" == IPv6{0x2001, 0xdb8}.to_string());
    CHECK("2001:db8::ff00:0:0:8329" == IPv6{0x2001, 0xdb8, 0, 0, 0xff00, 0, 0, 0x8329}.to_string());
    CHECK ("::1" == IPv6{0, 0, 0, 0, 0, 0, 0, 1}.to_string());
}