#pragma once
#include <string>
void RSA_encrypt(char* plaintext);
void RSA_decrypt(char* encrypted_data);
bool validate(const std::string& name, std::string regcode);
// UltraISOValidator_h__