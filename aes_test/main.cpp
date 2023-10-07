#include <iostream>
#include <cstdio>
#include <openssl/pem.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <memory>
#include <openssl/ec.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <openssl/rand.h>

using namespace std;


std::string PAESEncrypt(string input) {
    std::string did = "123123";
    unsigned char key[SHA256_DIGEST_LENGTH];
    // unsigned char* did = reinterpret_cast<unsigned char*>(const_cast<char*>(setup_info->did().c_str()));
    SHA256((unsigned char *)(did.c_str()), 6, key);

    const int keySize = 256;
    const int ivSize = AES_BLOCK_SIZE;

    // Generate random IV
    unsigned char iv[ivSize];
    RAND_bytes(iv, ivSize);

    // Set up encryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);

    // Allocate memory for output buffer
    int outputSize = input.length() + AES_BLOCK_SIZE;
    unsigned char* output = new unsigned char[outputSize];
    int encryptedSize = 0;

    // Encrypt the input data
    EVP_EncryptUpdate(ctx, output, &encryptedSize,
                      reinterpret_cast<const unsigned char*>(input.c_str()), input.length());

    // Finalize the encryption
    int finalEncryptedSize = 0;
    EVP_EncryptFinal_ex(ctx, output + encryptedSize, &finalEncryptedSize);
    encryptedSize += finalEncryptedSize;

    // Construct the result string with IV + ciphertext
    std::string encryptedData(reinterpret_cast<char*>(iv), ivSize);
    encryptedData += std::string(reinterpret_cast<char*>(output), encryptedSize);

    // Clean up
    delete[] output;
    EVP_CIPHER_CTX_free(ctx);

    return encryptedData;
}


std::string AESDecrypt(string encryptedData) {
    std::string did = "123123";
    unsigned char key[SHA256_DIGEST_LENGTH];
    // unsigned char* did = reinterpret_cast<unsigned char*>(const_cast<char*>(setup_info->did().c_str()));
    SHA256((unsigned char *)(did.c_str()), 6, key);
    const int keySize = 256;
    const int ivSize = AES_BLOCK_SIZE;

    // Extract IV and ciphertext from encrypted data
    const unsigned char* iv = reinterpret_cast<const unsigned char*>(encryptedData.c_str());
    const unsigned char* ciphertext = reinterpret_cast<const unsigned char*>(encryptedData.c_str() + ivSize);
    int ciphertextSize = encryptedData.size() - ivSize;

    // Set up decryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, key, iv);
    EVP_CIPHER_CTX_set_padding(ctx, 1);

    // Allocate memory for output buffer
    int outputSize = ciphertextSize + AES_BLOCK_SIZE;
    unsigned char* output = new unsigned char[outputSize];
    int decryptedSize = 0;

    // Decrypt the ciphertext
    EVP_DecryptUpdate(ctx, output, &decryptedSize, ciphertext, ciphertextSize);

    // Finalize the decryption
    int finalDecryptedSize = 0;
    EVP_DecryptFinal_ex(ctx, output + decryptedSize, &finalDecryptedSize);
    decryptedSize += finalDecryptedSize;

    // Construct the result string
    std::string decryptedData(reinterpret_cast<char*>(output), decryptedSize);

    // Clean up
    delete[] output;
    EVP_CIPHER_CTX_free(ctx);

    return decryptedData;

}


int main() {
	std::string cipher = PAESEncrypt("test123");
    for (unsigned char n : cipher) 
        std::cout << "0x" << std::hex << static_cast<unsigned short>(n) << ", " ;
    std::cout << std::endl;
	
	unsigned char cipher1[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x53,0x56,0x3a,0x1b,0x6a,0xd7,0x92,0x02,0x2b,0x54,0xc9,0x47,0x97,0x44,0x26,0xce,0xb3,0x06,0x66,0x52,0x11,0x1a,0x1d,
    0x81,0x79,0xae,0x33,0xf6,0x74,0x8c,0x38,0xa1,0xd5,0x43,0x0c,0x65,0xca,0x4f,0x09,0x4c,0xe7,0xe1,0x61,0x72,0x41,0x4f,0x1a,0xdc,};

    std::string str(cipher1, cipher1+sizeof(cipher1));
	
	std::string plain = AESDecrypt(cipher);
	std::cout << "plain = " << plain << std::endl;
    for (unsigned char n:plain)
        std::cout << "0x" << std::hex << static_cast<unsigned short>(n) << ", " ;

    std::cout << std::endl;
	
	return 0;	
}
