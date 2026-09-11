// lib/crypto/crypto.cc — minimal SHA-256 + Base64 (no OpenSSL dependency)
#include "lib/crypto/crypto.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <cstdint>

// ── SHA-256 ───────────────────────────────────────────────────────────────────
namespace {

static const uint32_t K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

inline uint32_t rotr(uint32_t x, int n) { return (x >> n) | (x << (32 - n)); }
inline uint32_t ch (uint32_t x,uint32_t y,uint32_t z){ return (x&y)^(~x&z); }
inline uint32_t maj(uint32_t x,uint32_t y,uint32_t z){ return (x&y)^(x&z)^(y&z); }
inline uint32_t S0(uint32_t x){ return rotr(x,2)^rotr(x,13)^rotr(x,22); }
inline uint32_t S1(uint32_t x){ return rotr(x,6)^rotr(x,11)^rotr(x,25); }
inline uint32_t s0(uint32_t x){ return rotr(x,7)^rotr(x,18)^(x>>3); }
inline uint32_t s1(uint32_t x){ return rotr(x,17)^rotr(x,19)^(x>>10); }

std::string sha256_raw(const std::string& msg) {
    uint32_t h[8] = {
        0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
        0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
    };
    std::string m = msg;
    uint64_t bit_len = (uint64_t)m.size() * 8;
    m += (char)0x80;
    while (m.size() % 64 != 56) m += (char)0x00;
    for (int i = 7; i >= 0; --i)
        m += (char)((bit_len >> (8 * i)) & 0xff);

    for (size_t i = 0; i < m.size(); i += 64) {
        uint32_t w[64];
        for (int j = 0; j < 16; ++j)
            w[j] = ((uint8_t)m[i+4*j]<<24)|((uint8_t)m[i+4*j+1]<<16)|
                   ((uint8_t)m[i+4*j+2]<<8)|(uint8_t)m[i+4*j+3];
        for (int j = 16; j < 64; ++j)
            w[j] = s1(w[j-2])+w[j-7]+s0(w[j-15])+w[j-16];
        uint32_t a=h[0],b=h[1],c=h[2],d=h[3],e=h[4],f=h[5],g=h[6],hh=h[7];
        for (int j = 0; j < 64; ++j) {
            uint32_t t1=hh+S1(e)+ch(e,f,g)+K[j]+w[j];
            uint32_t t2=S0(a)+maj(a,b,c);
            hh=g;g=f;f=e;e=d+t1;d=c;c=b;b=a;a=t1+t2;
        }
        h[0]+=a;h[1]+=b;h[2]+=c;h[3]+=d;
        h[4]+=e;h[5]+=f;h[6]+=g;h[7]+=hh;
    }
    std::string out(32, 0);
    for (int i = 0; i < 8; ++i)
        for (int j = 3; j >= 0; --j)
            out[i*4+(3-j)] = (h[i] >> (8*j)) & 0xff;
    return out;
}

static const char B64[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

}  // namespace

namespace crypto {

core::Result<std::string> Sha256Hex(const std::string& input) {
    if (input.empty())
        return core::Result<std::string>::Err("input must not be empty");
    std::string raw = sha256_raw(input);
    std::ostringstream oss;
    for (unsigned char c : raw)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    return core::Result<std::string>::Ok(oss.str());
}

std::string Base64Encode(const std::string& input) {
    std::string out;
    int val = 0, valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) { out += B64[(val >> valb) & 0x3f]; valb -= 6; }
    }
    if (valb > -6) out += B64[((val << 8) >> (valb + 8)) & 0x3f];
    while (out.size() % 4) out += '=';
    return out;
}

core::Result<std::string> Base64Decode(const std::string& input) {
    std::string out;
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; ++i) T[(uint8_t)B64[i]] = i;
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (T[c] == -1) {
            if (c == '=') break;
            return core::Result<std::string>::Err("invalid base64 character");
        }
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) { out += (char)((val >> valb) & 0xff); valb -= 8; }
    }
    return core::Result<std::string>::Ok(out);
}

}  // namespace crypto
