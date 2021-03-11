#pragma once

#include "blockchain.h"


enum class config_entry {
    p2p_address,
    seed_address,
    api_address,
    not_an_option
};

class loader_t {

    public:
        loader_t(std::string config_file_name = "");
    
        std::string p2p_address;
        std::vector<std::string> seed_address;
        std::string api_address;

    private:
        bool store_line(std::string key, std::string value);

        config_entry resolve_options(std::string);
};