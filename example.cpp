#include <iostream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/ec.h>
#include <openssl/ecdsa.h>
#include <openssl/obj_mac.h>
#include <openssl/pem.h>
#include <cstdlib>


int create_signature(unsigned char* hash)
{
    int function_status = -1;
    EC_KEY *eckey = EC_KEY_new();
    BN_CTX *ctx = BN_CTX_new();
    if (NULL == eckey)
    {
        std::cout << "Failed to create new EC Key\n";
        function_status = -1;
    }
    else
    {
        EC_GROUP *ecgroup = EC_GROUP_new_by_curve_name(NID_secp256k1);
        if (NULL == ecgroup)
        {
            std::cout << "Failed to create new EC Group\n";
            function_status = -1;
        }
        else
        {
            int set_group_status = EC_KEY_set_group(eckey, ecgroup);
            const int set_group_success = 1;
            if (set_group_success != set_group_status)
            {
                std::cout << "Failed to set group for EC Key\n";
                function_status = -1;
            }
            else
            {
                const int gen_success = 1;
                int gen_status = EC_KEY_generate_key(eckey);
                if (gen_success != gen_status)
                {
                    std::cout << "Failed to generate EC Key\n";
                    function_status = -1;
                }
                else
                {
                    BIO* outbio_priv = BIO_new(BIO_s_mem());
                    PEM_write_bio_ECPrivateKey(outbio_priv, eckey, NULL, NULL, 0, 0, NULL);
                    std::string keyStr_priv;
                    int priKeyLen = BIO_pending(outbio_priv);
                    keyStr_priv.resize(priKeyLen);
                    BIO_read(outbio_priv, (void*)&(keyStr_priv.front()), priKeyLen);
                    std::cout << "ec priv key:\n" << keyStr_priv << std::endl;

                    BIO* outbio_pub = BIO_new(BIO_s_mem());
                    PEM_write_bio_EC_PUBKEY(outbio_pub, eckey);
                    std::string keyStr_pub;
                    int pubKeyLen = BIO_pending(outbio_pub);
                    keyStr_pub.resize(pubKeyLen);
                    BIO_read(outbio_pub, (void*)&(keyStr_pub.front()), pubKeyLen);                    
                    std::cout << "ec pub key:\n" << keyStr_pub << std::endl;
                    
                    std::cout << "ec priv key in hex:\n" << BN_bn2hex(EC_KEY_get0_private_key(eckey)) << std::endl;
                    std::cout << "ec pub key in hex:\n" << EC_POINT_point2hex(ecgroup, EC_KEY_get0_public_key(eckey),
                                                        EC_GROUP_get_point_conversion_form(ecgroup), ctx) << std::endl;

                    



                    ECDSA_SIG *signature = ECDSA_do_sign(hash, SHA256_DIGEST_LENGTH, eckey);
                    if (NULL == signature)
                    {
                        std::cout << "Failed to generate EC Signature\n";
                        function_status = -1;
                    }
                    else
                    {
                        std::cout << "signature (sig->r, sig->s):" << BN_bn2hex(ECDSA_SIG_get0_r(signature)) << " " << BN_bn2hex(ECDSA_SIG_get0_s(signature)) << std::endl;

                        int verify_status = ECDSA_do_verify(hash, SHA256_DIGEST_LENGTH, signature, eckey);
                        const int verify_success = 1;
                        if (verify_success != verify_status)
                        {
                            std::cout <<  "Failed to verify EC Signature\n";
                            function_status = -1;
                        }
                        else
                        {
                            std::cout << "Verifed EC Signature\n";

                            function_status = 1;
                        }
                    }
                    BIO_free_all(outbio_priv);
                    BIO_free_all(outbio_pub);
                }
            }
            EC_GROUP_free(ecgroup);
        }
        EC_KEY_free(eckey);
        BN_CTX_free(ctx);
    }

  return function_status;

}

int main() 
{
    std::cout << "starting" << std::endl;

    SHA256_CTX context;
    if(!SHA256_Init(&context))
        return 1;
    if(!SHA256_Update(&context, "asd", 3))
        return 1;

    unsigned char md[SHA256_DIGEST_LENGTH];
    if(!SHA256_Final(md, &context))
        return 1;
    
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        if(static_cast<int>(md[i]) < 16)
            std::cout << "0";
        std::cout << std::hex << static_cast<int>(md[i]);
    }

    std::cout << std::endl;

    int status = create_signature(md);

}