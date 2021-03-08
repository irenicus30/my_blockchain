#include <transaction.h>

bool transaction_t::verify_data() {
    return true;
}

bool transaction_t::verify_signature() {
    std::vector<byte> v = serialize();

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, v.data(), 2+data_size))
        return false;

    unsigned char md[SHA256_DIGEST_LENGTH];    
    if(!SHA256_Final(md, &context))
        return false;

    bool status = true;
    
    EC_KEY *eckey = EC_KEY_new();
    if (eckey == nullptr) {
        status = false;
    } else {
        EC_GROUP *ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (ecgroup == nullptr) {
            return false;
        } else {
            int set_group_status = EC_KEY_set_group(eckey, ecgroup);
            const int set_group_success = 1;
            if (set_group_success != set_group_status) {
                status = false;
            } else {
                BIGNUM *private_bn = nullptr;
                char private_key_c_str[private_key.size()];
                private_key.copy(private_key_c_str, private_key.size());
                unsigned char *private_key_unsigned_char = static_cast<unsigned char*>(static_cast<void *>(private_key_c_str));
                BN_bin2bn(private_key_unsigned_char, private_key.size(), private_bn);
                EC_KEY_set_private_key(eckey, private_bn);

                char signature_c_str[signature.size()];
                signature.copy(signature_c_str, signature.size());
                unsigned char *signature_unsigned_char = static_cast<unsigned char*>(static_cast<void *>(signature_c_str));

                int verify_status = ECDSA_verify(0, md, SHA256_DIGEST_LENGTH, signature_unsigned_char, signature.size(), eckey);
                const int verify_success = 1;
                if (verify_success != verify_status)
                    status = false;

                BN_free(private_bn);
            }
        }
        EC_GROUP_free(ecgroup);
    }
    EC_KEY_free(eckey);

    return status;
}

std::vector<byte> transaction_t::serialize() const {
    std::vector<byte> v;
    v.resize(size);
    byte *begin = &v[0];
    byte *data_begin = &v[2+2];
    byte *signature_begin = &v[2+2+data.size()+2];
    byte *private_key_begin = &v[2+2+data.size()+2+signature.size()+2];

    uint16_t *ptr = reinterpret_cast<uint16_t*>(begin);
    *ptr = size;

    ptr = reinterpret_cast<uint16_t*>(data_begin-2);
    *ptr = data_size;
    char *char_ptr = reinterpret_cast<char*>(data_begin);
    data.copy(char_ptr, data.size());

    ptr = reinterpret_cast<uint16_t*>(signature_begin-2);
    *ptr = signature_size;
    char_ptr = reinterpret_cast<char*>(signature_begin);
    signature.copy(char_ptr, signature.size());

    ptr = reinterpret_cast<uint16_t*>(private_key_begin-2);
    *ptr = private_key_size;
    char_ptr = reinterpret_cast<char*>(private_key_begin);
    private_key.copy(char_ptr, private_key.size());

    return v;
}

bool transaction_t::deserialize(std::vector<byte> v) {
    byte *begin = &v[0];
    uint16_t *ptr = reinterpret_cast<uint16_t*>(begin);
    size = *ptr;

    byte *data_begin = &v[2+2];
    ptr = reinterpret_cast<uint16_t*>(data_begin-2);
    data_size = *ptr;
    char *char_ptr = reinterpret_cast<char*>(data_begin);
    data.assign(char_ptr, data_size);

    byte *signature_begin = &v[2+2+data.size()+2];
    ptr = reinterpret_cast<uint16_t*>(signature_begin-2);
    signature_size = *ptr;
    char_ptr = reinterpret_cast<char*>(signature_begin);
    signature.assign(char_ptr, signature_size);

    byte *private_key_begin = &v[2+2+data.size()+2+signature.size()+2];
    ptr = reinterpret_cast<uint16_t*>(private_key_begin-2);
    private_key_size = *ptr;
    char_ptr = reinterpret_cast<char*>(private_key_begin);
    signature.assign(char_ptr, private_key_size);

    return true;
}