#pragma once

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

std::string to_string(const byte_vector_t& bytes);

class peer_t;
typedef std::shared_ptr<peer_t> peer_ptr;

