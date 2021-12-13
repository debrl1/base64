
#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

/**
 * Derives the length of the encoded base64 string from the length of the
 * original string.
 */
unsigned int encodedlen(unsigned int len) {
    unsigned int newlen = (len / 3) * 4;
    if (len % 3 != 0)
        newlen += 4;
    return newlen;
}

/**
 * Encodes an unencoded group (containing exactly three characters) into
 * base64. The resulting group contains exactly four characters.
 */
void encodegroup(const unsigned char *input, unsigned char *output) {
    output[0] = input[0] >> 0x02;
    output[1] = ((input[0] & 0x03) << 0x04) | ((input[1] & 0xf0) >> 0x04);
    output[2] = ((input[1] << 0x02) & 0x3f) | (input[2] >> 0x06);
    output[3] = input[2] & 0x3f;
}


void encoderemainder(const unsigned char *input, unsigned int inputlen, unsigned char *output) {
    output[0] = input[0] >> 0x02;
    output[1] = (input[0] & 0x03) << 0x04;
    if (inputlen == 1) {
        output[2] = output[3] = 0x40;
    } else {
        output[1] |= (input[1] & 0xf0) >> 0x04;
        output[2] = (input[1] << 0x02) & 0x3f;
        output[3] = 0x40;
    }
}


unsigned char *base64encode(const unsigned char *original, unsigned int length) {
    unsigned int newlen = encodedlen(length);
    unsigned char *base64 = (unsigned char *) malloc(sizeof(unsigned char) * (newlen + 1));

    unsigned char originalgroup[3];
    unsigned char *encoded = (unsigned char *) malloc(sizeof(unsigned char) * 4);

    int k = 0;
    for (int i = 0; i < length; i += 4) {

        /*
         * Determining the amount of values left to encode
         */
        unsigned int lim;
        if (i + 3 > length)
            lim = length - i;
        else
            lim = 3;

        /*
         * Copying the original values;
         */
        for (int j = 0; j < lim; ++j)
            originalgroup[j] = original[i + j];

        /*
         * Encoding the copied values and writing these values to the
         * resulting array;
         */
        if (lim == 3)
            encodegroup(originalgroup, encoded);
        else
            encoderemainder(originalgroup, lim, encoded);
        for (int j = 0; j < 4; ++j)
            base64[k++] = encoded[j];
    }
    return base64;
}


void test() {

    unsigned char s[] = "Man";
    unsigned char *k = base64encode(s, 2);
    for (int i = 0; i < 4; ++i) {
        printf("0x%02x\t", k[i]);
    }
}
