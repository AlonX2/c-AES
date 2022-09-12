#if !defined(AES_h)
#define AES_h
unsigned char* Encrypt(unsigned char* data, unsigned char* key, size_t dataSize, bool showSteps);
unsigned char* Decrypt(unsigned char* data, unsigned char* key, size_t dataSize, bool showSteps);

#endif