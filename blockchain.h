#pragma once

#include <openssl/sha.h>
#include <openssl/ec.h>

#include <boost/log/trivial.hpp>

#include <cstring>
#include <cstdint>
#include <cctype>
#include <cmath>

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <sstream> 
#include <bitset>
#include <array>

#include "config.h"


typedef unsigned char byte_t;
typedef byte_t* byte_ptr;
typedef std::vector<byte_t> byte_vector_t;

typedef std::array<byte_t, SHA256_DIGEST_LENGTH> hash_t;

std::string to_string(const hash_t& bytes);