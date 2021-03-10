#pragma once

#include "blockchain.h"


class loader_t {

    public:
        loader_t(std::string config_file_name = "");

    private:
        bool store_line(std::string key, std::string value);
};

