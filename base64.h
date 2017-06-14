#ifndef _BASE64_H_
#define _BASE64_H_

typedef unsigned char uint8_t;

void base64_encode(const uint8_t* data, int data_len, uint8_t* out);

void base64_decode(const uint8_t* data, int data_len, uint8_t* out)

#endif