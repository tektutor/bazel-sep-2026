// lib/crypto/crypto_test.cc
#include "lib/crypto/crypto.h"
#include <gtest/gtest.h>

TEST(CryptoSha256, StartsWithKnownPrefix) {
    auto r = crypto::Sha256Hex("abc");
    ASSERT_TRUE(r.ok());
    EXPECT_EQ(r.value().substr(0, 8), "ba7816bf");
}

TEST(CryptoSha256, EmptyInputReturnsErr) {
    EXPECT_TRUE(crypto::Sha256Hex("").err());
}

TEST(CryptoBase64, RoundTrip) {
    std::string plain = "Hello, Bazel 9!";
    auto decoded = crypto::Base64Decode(crypto::Base64Encode(plain));
    ASSERT_TRUE(decoded.ok());
    EXPECT_EQ(decoded.value(), plain);
}

TEST(CryptoBase64, InvalidCharsReturnErr) {
    EXPECT_TRUE(crypto::Base64Decode("!!!!").err());
}
