#include "transaction.h"


bool transaction_t::add_signature(byte_vector_t key) {
    private_key_size = key.size();
    private_key = key;

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, reinterpret_cast<byte_t*>(&size), 2))
        return false;
    if(!SHA256_Update(&context, reinterpret_cast<byte_t*>(&data_size), 2))
        return false;
    if(!SHA256_Update(&context, data.data(), data_size))
        return false;

    byte_t md[SHA256_DIGEST_LENGTH];
    if(!SHA256_Final(md, &context))
        return false;

    bool status = true;
    
    EC_KEY *eckey = EC_KEY_new();
    if (eckey == nullptr) {
        status = false;
    } else {
        EC_GROUP *ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (ecgroup == nullptr) {
            status = false;
        } else {
            int set_group_status = EC_KEY_set_group(eckey, ecgroup);
            const int set_group_success = 1;
            if (set_group_success != set_group_status) {
                status = false;
            } else {
                BIGNUM *private_bn = nullptr;
                BN_bin2bn(private_key.data(), private_key.size(), private_bn);
                EC_KEY_set_private_key(eckey, private_bn);

                signature.resize(ECDSA_size(eckey));
                unsigned int len;
                int result = ECDSA_sign(0, md, SHA256_DIGEST_LENGTH, signature.data(), &len, eckey);
                signature_size = static_cast<uint16_t>(len);
                if(signature_size == 0) {
                    status = false;
                } else {
                    signature.resize(signature_size);
                }
                BN_free(private_bn);
            }
        }
        EC_GROUP_free(ecgroup);
    }
    EC_KEY_free(eckey);

    return status;

    return true;
}

bool transaction_t::verify_data() const {
    return true;
}

bool transaction_t::verify_signature() const {
    byte_vector_t v = serialize();

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return false;

    if(!SHA256_Update(&context, v.data(), 4+data_size))
        return false;

    byte_t md[SHA256_DIGEST_LENGTH];
    if(!SHA256_Final(md, &context))
        return false;

    bool status = true;
    
    EC_KEY *eckey = EC_KEY_new();
    if (eckey == nullptr) {
        status = false;
    } else {
        EC_GROUP *ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (ecgroup == nullptr) {
            status = false;
        } else {
            int set_group_status = EC_KEY_set_group(eckey, ecgroup);
            const int set_group_success = 1;
            if (set_group_success != set_group_status) {
                status = false;
            } else {
                BIGNUM *private_bn = nullptr;
                BN_bin2bn(private_key.data(), private_key.size(), private_bn);
                EC_KEY_set_private_key(eckey, private_bn);

                int verify_status = ECDSA_verify(0, md, SHA256_DIGEST_LENGTH, signature.data(), signature.size(), eckey);
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

byte_vector_t transaction_t::serialize() const {
    byte_vector_t v;
    v.resize(size);

    byte_t *begin = &v[0];
    byte_t *data_begin = &v[2+2];
    byte_t *signature_begin = &v[2+2+data.size()+2];
    byte_t *private_key_begin = &v[2+2+data.size()+2+signature.size()+2];

    uint16_t *ptr = reinterpret_cast<uint16_t*>(begin);
    *ptr = size;

    ptr = reinterpret_cast<uint16_t*>(data_begin-2);
    *ptr = data_size;
    std::memcpy(data_begin, data.data(), data.size());

    ptr = reinterpret_cast<uint16_t*>(signature_begin-2);
    *ptr = signature_size;
    std::memcpy(signature_begin, signature.data(), signature.size());

    ptr = reinterpret_cast<uint16_t*>(private_key_begin-2);
    *ptr = private_key_size;
    std::memcpy(private_key_begin, private_key.data(), private_key.size());

    return v;
}

bool transaction_t::deserialize(byte_vector_t v) {
    byte_t *begin = &v[0];
    uint16_t *ptr = reinterpret_cast<uint16_t*>(begin);
    size = *ptr;

    byte_t *data_begin = &v[2+2];
    ptr = reinterpret_cast<uint16_t*>(data_begin-2);
    data_size = *ptr;
    data.resize(data_size);
    std::memcpy(data.data(), data_begin, data_size);

    byte_t *signature_begin = &v[2+2+data.size()+2];
    ptr = reinterpret_cast<uint16_t*>(signature_begin-2);
    signature_size = *ptr;
    signature.resize(signature_size);
    std::memcpy(signature.data(), signature_begin, signature_size);

    byte_t *private_key_begin = &v[2+2+data.size()+2+signature.size()+2];
    ptr = reinterpret_cast<uint16_t*>(private_key_begin-2);
    private_key_size = *ptr;
    private_key.resize(private_key_size);
    std::memcpy(private_key.data(), private_key_begin, private_key_size);

    return true;
}