#include <blockchain.h>
#include <transaction>

struct block_t {
    uint32_t size;
    uint64_t nounce;
    std::string previous_block_hash;

    uint32_t number_of_transactions;
    std::vector<transaction_t> transaction;

    uint64_t this_block_number;

    //this is not hashed
    std::string this_block_hash;

    void add_hash();
    bool verify();

    std::vector<byte> serialize();
    void deserialize(std::vector<byte>);
};