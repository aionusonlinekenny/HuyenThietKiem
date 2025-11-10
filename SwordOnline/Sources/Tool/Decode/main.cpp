// main.cpp - VC6 Console Tool: ipenc
// Usage:
//   ipenc 45.130.165.13           -> in "ENC:<HEX>"
//   ipenc -d ENC:4A0F...          -> out "45.130.165.13"
//   ipenc -d 4A0F...              -> same (accept without "ENC:")

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "EDOneTimePadStandalone.h"

// ---- HEX helpers ----
static int HexToNibble(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}

static int HexToBytes(const char* src, unsigned char* dst, int maxDst) {
    int n = 0, i = 0;
    while (src[i]) {
        char c1 = src[i++];
        if (c1==' ' || c1=='\t' || c1=='\r' || c1=='\n') continue;
        if (!src[i]) return -1;
        char c2 = src[i++];
        while (c2==' ' || c2=='\t' || c2=='\r' || c2=='\n') {
            if (!src[i]) return -1;
            c2 = src[i++];
        }
        int hi = HexToNibble(c1), lo = HexToNibble(c2);
        if (hi < 0 || lo < 0) return -1;
        if (n >= maxDst) return -1;
        dst[n++] = (unsigned char)((hi << 4) | lo);
    }
    return n;
}

static void BytesToHex(const unsigned char* src, int len, char* dst, int dstSize) {
    static const char* HEX = "0123456789ABCDEF";
    int j = 0;
    for (int i = 0; i < len; ++i) {
        unsigned char b = src[i];
        if (j + 2 >= dstSize) break;
        dst[j++] = HEX[(b >> 4) & 0xF];
        dst[j++] = HEX[b & 0xF];
    }
    if (j < dstSize) dst[j] = '\0';
}

static int EncodeAddressForIni(const char* szIp, char* szOut, int outSize) {
    if (!szIp || !szOut || outSize <= 0) return 0;
    char tmp[64];
    int L = (int)strlen(szIp);
    if (L <= 0 || L >= (int)sizeof(tmp)) return 0;
    strcpy(tmp, szIp);

    EDOneTimePad_Encipher(tmp, L);

    if (outSize < 4 + L * 2 + 1) return 0;
    strcpy(szOut, "ENC:");
    BytesToHex((const unsigned char*)tmp, L, szOut + 4, outSize - 4);
    return 1;
}

static int DecodeAddressFromIni(const char* szField, char* szPlainOut, int outSize) {
    if (!szField || !szPlainOut || outSize <= 0) return 0;
    const char* pHex = szField;
    if (pHex[0]=='E' && pHex[1]=='N' && pHex[2]=='C' && pHex[3]==':') pHex += 4;

    unsigned char buf[128];
    int n = HexToBytes(pHex, buf, sizeof(buf));
    if (n <= 0) return 0;

    EDOneTimePad_Decipher((char*)buf, n);
    if (n >= (int)sizeof(buf)) return 0;
    buf[n] = '\0';

    strncpy(szPlainOut, (const char*)buf, outSize - 1);
    szPlainOut[outSize - 1] = '\0';
    return 1;
}

static void PrintUsage() {
    puts("ipenc - VC6 console tool (EDOneTimePad)");
    puts("  Encode: ipenc 45.130.165.13");
    puts("          -> ENC:<HEX>");
    puts("  Decode: ipenc -d ENC:<HEX>");
    puts("       or ipenc -d <HEX>");
}

int main(int argc, char* argv[]) {
    if (argc < 2) { PrintUsage(); return 0; }

    if (argc == 2) {
        const char* ip = argv[1];
        int dots = 0; for (const char* p = ip; *p; ++p) if (*p == '.') ++dots;
        if (dots != 3) { fprintf(stderr, "Input khong phai IPv4: %s\n", ip); return 1; }
        char out[512];
        if (!EncodeAddressForIni(ip, out, sizeof(out))) { fprintf(stderr, "Encode that bai.\n"); return 2; }
        puts(out);
        return 0;
    }

    if (argc == 3 && (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "/d") == 0)) {
        char plain[128];
        if (!DecodeAddressFromIni(argv[2], plain, sizeof(plain))) { fprintf(stderr, "Decode that bai.\n"); return 3; }
        puts(plain);
        return 0;
    }

    PrintUsage();
    return 0;
}
