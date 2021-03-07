#include <iostream>
#include <iomanip>
#include <openssl/sha.h>


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

}