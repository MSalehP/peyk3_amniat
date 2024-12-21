#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <openssl/aes.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/rand.h>
#include <fstream>

// Random message generator
std::vector<unsigned char> generateRandomMessage(size_t size) {
    std::vector<unsigned char> message(size);
    RAND_bytes(message.data(), size);
    return message;
}

// AES time measure
double measureAesTime(const std::vector<unsigned char>& message) {
    AES_KEY aesKey;
    unsigned char key[16]; // AES 128-bit key
    RAND_bytes(key, sizeof(key));

    unsigned char iv[16]; // Initialization Vector
    RAND_bytes(iv, sizeof(iv));

    std::vector<unsigned char> encrypted(message.size());
    auto start = std::chrono::high_resolution_clock::now();
    AES_set_encrypt_key(key, 128, &aesKey);
    AES_cbc_encrypt(message.data(), encrypted.data(), message.size(), &aesKey, iv, AES_ENCRYPT);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

// RSA time measure
double measureRsaTime(const std::vector<unsigned char>& message) {
    RSA* rsaKey = RSA_new();
    BIGNUM* bne = BN_new();
    BN_set_word(bne, RSA_F4);
    RSA_generate_key_ex(rsaKey, 3072, bne, NULL);

    std::vector<unsigned char> encrypted(RSA_size(rsaKey));
    auto start = std::chrono::high_resolution_clock::now();
    RSA_public_encrypt(message.size(), message.data(), encrypted.data(), rsaKey, RSA_PKCS1_OAEP_PADDING);
    auto end = std::chrono::high_resolution_clock::now();
    RSA_free(rsaKey);
    BN_free(bne);
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    const size_t messageSize = 256;
    const int numMessages = 1000;

    std::vector<double> aesTimes, rsaTimes;

    for (int i = 0; i < numMessages; ++i) {
        auto message = generateRandomMessage(messageSize);
        aesTimes.push_back(measureAesTime(message));
        rsaTimes.push_back(measureRsaTime(message));
    }

    std::ofstream file("times.csv");
    file << "AES_Time,RSA_Time\n";
    for (int i = 0; i < numMessages; ++i) {
        file << aesTimes[i] << "," << rsaTimes[i] << "\n";
    }
    file.close();

    std::cout << "result saved!" << std::endl;
    return 0;
}
