#pragma once

#include <boost/log/trivial.hpp>

#include <openssl/sha.h>
#include <openssl/ec.h>

#include <cstring>
#include <cstdint>
#include <cctype>

#include <string>
#include <vector>
#include <array>
#include <memory>
#include <sstream> 

#include "config.h"

typedef unsigned char byte_t;
typedef std::vector<byte_t> byte_vector_t;

std::string to_string(const byte_vector_t& bytes);

class peer_t;
typedef std::shared_ptr<peer_t> peer_ptr;

