#include "ecdsa.h"
#include "hash.h"


byte_vector_t get_signature(byte_ptr ptr, int len, byte_vector_t& private_key)
{
    bool status = true;
    byte_vector_t signature;

    hash_t md = get_hash(ptr, len);

    EC_KEY *eckey = EC_KEY_new();
    if (eckey == nullptr)
    {
        status = false;
    }
    else
    {
        EC_GROUP *ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (ecgroup == nullptr)
        {
            status = false;
        }
        else
        {
            int set_group_status = EC_KEY_set_group(eckey, ecgroup);
            const int set_group_success = 1;
            if (set_group_success != set_group_status)
            {
                status = false;
            }
            else
            {
                BIGNUM *private_bn = nullptr;
                BN_bin2bn(private_key.data(), private_key.size(), private_bn);
                EC_KEY_set_private_key(eckey, private_bn);

                signature.resize(ECDSA_size(eckey));
                unsigned int len;
                int result = ECDSA_sign(0, md.data(), sizeof(hash_t), signature.data(), &len, eckey);
                if(len == 0)
                {
                    signature.resize(0);
                    status = false;
                }
                else
                {
                    signature.resize(len);
                }
                BN_free(private_bn);
            }
        }
        EC_GROUP_free(ecgroup);
    }
    EC_KEY_free(eckey);

    return signature;
}

bool verify_signature(byte_ptr ptr, int len, byte_vector_t& public_key, byte_vector_t& signature)
{
    bool status = true;

    hash_t md = get_hash(ptr, len);

    EC_KEY *eckey = EC_KEY_new();
    if (eckey == nullptr) {
        status = false;
    }
    else
    {
        EC_GROUP *ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (ecgroup == nullptr)
        {
            status = false;
        }
        else
        {
            int set_group_status = EC_KEY_set_group(eckey, ecgroup);
            const int set_group_success = 1;
            if (set_group_success != set_group_status)
            {
                status = false;
            }
            else
            {
                BIGNUM *public_bn = nullptr;
                BN_bin2bn(public_key.data(), public_key.size(), public_bn);
                EC_POINT *pub = nullptr;
                EC_POINT_bn2point(EC_KEY_get0_group(eckey), public_bn, pub, NULL);
                EC_KEY_set_public_key(eckey, pub);

                int verify_status = ECDSA_verify(0, md.data(), sizeof(hash_t), signature.data(), signature.size(), eckey);
                const int verify_success = 1;
                if (verify_success != verify_status)
                {
                    status = false;
                }

                BN_free(public_bn);
                EC_POINT_free(pub);
            }
        }
        EC_GROUP_free(ecgroup);
    }
    EC_KEY_free(eckey);
    return status;
}