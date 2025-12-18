// GuiIpEnc.cpp - Win32 GUI app (VC6) Encode/Decode IP <-> ENC:<hex>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "EDOneTimePadStandalone.h"

// ---- IDs ----
#define IDC_EDIT_INPUT   1001
#define IDC_BTN_ENC      1002
#define IDC_BTN_DEC      1003
#define IDC_EDIT_OUTPUT  1004
#define IDC_STATIC_IN    1005
#define IDC_STATIC_OUT   1006

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
        if (c1==' '||c1=='\t'||c1=='\r'||c1=='\n') continue;
        if (!src[i]) return -1;
        char c2 = src[i++];
        while (c2==' '||c2=='\t'||c2=='\r'||c2=='\n') {
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
    int L = (int)lstrlenA(szIp);
    if (L <= 0 || L >= (int)sizeof(tmp)) return 0;
    lstrcpyA(tmp, szIp);

    // encode in-place
    EDOneTimePad_Encipher(tmp, L);

    if (outSize < 4 + L*2 + 1) return 0;
    lstrcpyA(szOut, "ENC:");
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

    lstrcpynA(szPlainOut, (const char*)buf, outSize);
    return 1;
}

static int IsLikelyIPv4(const char* s) {
    if (!s || !*s) return 0;
    int dots = 0;
    for (const char* p = s; *p; ++p) if (*p == '.') ++dots;
    return (dots == 3);
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CREATE:
    {
        HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);

        CreateWindowExA(0, "STATIC", "Input (IP or ENC:<hex>):",
            WS_CHILD | WS_VISIBLE, 10, 10, 360, 18, hWnd, (HMENU)IDC_STATIC_IN, GetModuleHandle(NULL), 0);

        HWND hIn = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
            10, 30, 460, 24, hWnd, (HMENU)IDC_EDIT_INPUT, GetModuleHandle(NULL), 0);

        CreateWindowExA(0, "BUTTON", "Encode -> ENC:<hex>",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            10, 60, 180, 28, hWnd, (HMENU)IDC_BTN_ENC, GetModuleHandle(NULL), 0);

        CreateWindowExA(0, "BUTTON", "Decode -> IP",
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            200, 60, 130, 28, hWnd, (HMENU)IDC_BTN_DEC, GetModuleHandle(NULL), 0);

        CreateWindowExA(0, "STATIC", "Output:",
            WS_CHILD | WS_VISIBLE, 10, 95, 360, 18, hWnd, (HMENU)IDC_STATIC_OUT, GetModuleHandle(NULL), 0);

        HWND hOut = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
            WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_READONLY,
            10, 115, 460, 24, hWnd, (HMENU)IDC_EDIT_OUTPUT, GetModuleHandle(NULL), 0);

        SendMessage(hIn, WM_SETFONT, (WPARAM)hFont, TRUE);
        SendMessage(hOut, WM_SETFONT, (WPARAM)hFont, TRUE);
        break;
    }
    case WM_COMMAND:
        if (HIWORD(wParam) == BN_CLICKED)
        {
            char in[512]; in[0] = 0;
            GetWindowTextA(GetDlgItem(hWnd, IDC_EDIT_INPUT), in, sizeof(in));

            if (LOWORD(wParam) == IDC_BTN_ENC)
            {
                if (!IsLikelyIPv4(in)) {
                    MessageBoxA(hWnd, "Chuoi nhap khong phai IPv4 (vd: 45.130.165.13).", "Encode", MB_ICONWARNING);
                    break;
                }
                char out[1024];
                if (!EncodeAddressForIni(in, out, sizeof(out))) {
                    MessageBoxA(hWnd, "Encode that bai.", "Encode", MB_ICONERROR);
                    break;
                }
                SetWindowTextA(GetDlgItem(hWnd, IDC_EDIT_OUTPUT), out);
            }
            else if (LOWORD(wParam) == IDC_BTN_DEC)
            {
                if (!in[0]) {
                    MessageBoxA(hWnd, "Nhap ENC:<hex> hoac chuoi <hex> de giai ma.", "Decode", MB_ICONWARNING);
                    break;
                }
                char plain[256];
                if (!DecodeAddressFromIni(in, plain, sizeof(plain))) {
                    MessageBoxA(hWnd, "Decode that bai. Kiem tra chuoi hex/ENC:...", "Decode", MB_ICONERROR);
                    break;
                }
                SetWindowTextA(GetDlgItem(hWnd, IDC_EDIT_OUTPUT), plain);
            }
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_GETMINMAXINFO:
    {
        // lock size fixed
        MINMAXINFO* p = (MINMAXINFO*)lParam;
        p->ptMinTrackSize.x = 490;
        p->ptMinTrackSize.y = 180;
        p->ptMaxTrackSize.x = 490;
        p->ptMaxTrackSize.y = 180;
        return 0;
    }
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    const char* CLASS_NAME = "IPENC_MAIN_WND";

    WNDCLASSA wc = {0};
    wc.style         = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
    wc.lpszClassName = CLASS_NAME;

    if (!RegisterClassA(&wc))
        return 0;

    HWND hWnd = CreateWindowExA(0, CLASS_NAME, "IP Encoder (EDOneTimePad) - VC6",
                    WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                    CW_USEDEFAULT, CW_USEDEFAULT, 500, 210,
                    NULL, NULL, hInstance, NULL);
    if (!hWnd) return 0;

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}
