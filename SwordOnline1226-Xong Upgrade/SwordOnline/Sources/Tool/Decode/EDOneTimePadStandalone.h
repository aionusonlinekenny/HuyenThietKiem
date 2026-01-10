// EDOneTimePadStandalone.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void EDOneTimePad_Encipher(char* pPlaintext, int nPlainLen);
void EDOneTimePad_Decipher(char* pCiphertext, int nCiphertextLen);

#ifdef __cplusplus
}
#endif
