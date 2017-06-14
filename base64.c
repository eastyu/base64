#include "base64.h"

void base64_encode(const uint8_t* data, int data_len, uint8_t* out)
{
    static uint8_t BASE64[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/',
    };

    #define data_0(map, a)      ((map)[(a) >> 0x02])
    #define data_1(map, a, b)   ((map)[(((a) & 0x03) << 0x04) | ((b) >> 0x04)])
    #define data_2(map, a, b)   ((map)[(((a) & 0x0F) << 0x02) | ((b) >> 0x06)])
    #define data_3(map, a)      ((map)[(c) & 0x3F])
    #define fill_data(map, buf, counter, a, b, c)       \
    {                                                   \
        buf[(counter)++ ] = data_0((map), (a));         \
        buf[(counter)++ ] = data_1((map), (a), (b));    \
        buf[(counter)++ ] = data_2((map), (b), (c));    \
        buf[(counter)++ ] = data_3((map), (c));         \
    }
    
    int i = 0, j = 0, mod = data_len % 3, data_len_align = data_len - mod;
    uint8_t a = 0, b = 0, c = 0;
    while (i < data_len_align) {
        a = data[i++]; b = data[i++]; c = data[i++];
        fill_data(BASE64, out, j, a, b, c);
    }
    if (mod != 0) {
        a = data[i++]; b = (1 == mod) ? 0 : data[i++]; c = 0;
        fill_data(BASE64, out, j, a, b, c);
        (2 == mod) ? out[--j] = '=' : (out[--j] = '=', out[--j] = '=');
    }
}

void base64_decode(const uint8_t* data, int data_len, uint8_t* out)
{
    static uint8_t DEBASE64[] = {
        0x3E, 0x00, 0x00, 0x00, 0x3F, 0x34, 0x35, 0x36,
        0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11,
        0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x1B,
        0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23,
        0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B,
        0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33
    };
    for (int i = 0, j = 0; i < data_len; i += 4) {
        uint8_t a = data[i], b = data[i + 1], c = data[i + 2], d = data[i + 3];
        out[j++] = (DEBASE64[a - 0x2B] << 2) | ((DEBASE64[b - 0x2B] & 0x30) >> 4);
        out[j++] = (DEBASE64[b - 0x2B] << 4) | ((DEBASE64[c - 0x2B] & 0x3C) >> 2);
        out[j++] = (DEBASE64[c - 0x2B] << 6) | DEBASE64[d - 0x2B];
    }
}