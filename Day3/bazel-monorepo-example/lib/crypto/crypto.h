#pragma once
// lib/crypto/crypto.h
#ifdef _WIN32
  #ifdef CRYPTO_BUILDING_DLL
    #define CRYPTO_EXPORT __declspec(dllexport)
  #else
    #define CRYPTO_EXPORT __declspec(dllimport)
  #endif
#else
  #define CRYPTO_EXPORT __attribute__((visibility("default")))
#endif

#include <string>
#include "lib/core/result.h"

namespace crypto {

CRYPTO_EXPORT core::Result<std::string> Sha256Hex(const std::string& input);
CRYPTO_EXPORT std::string               Base64Encode(const std::string& input);
CRYPTO_EXPORT core::Result<std::string> Base64Decode(const std::string& input);

}  // namespace crypto
