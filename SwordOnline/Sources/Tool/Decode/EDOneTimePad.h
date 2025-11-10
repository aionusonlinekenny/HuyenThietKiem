// EDOneTimePadStandalone.h
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Mã hoá/giải mã tại chỗ (in-place), dữ liệu là ASCII bytes.
// Tham số len là số byte thực tế (không tính '\0').
void EDOneTimePad_Encipher(char* pPlaintext, int nPlainLen);
void EDOneTimePad_Decipher(char* pCiphertext, int nCiphertextLen);

#ifdef __cplusplus
}
#endif
