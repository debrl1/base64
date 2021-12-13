
#include <stdio.h>
#include <stdlib.h>

#include "base64.h"

#define PADDING_CHAR 0x40

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


/**
 * Encodes an unencoded group containing one or two characters into base64.
 * The resulting group will still only contain four characters. For all of the
 * characters that can not be derived from the original bytes a padding
 * character will be inserted.
 */
void encoderemainder(const unsigned char *input, unsigned int inputlen, unsigned char *output) {
    output[0] = input[0] >> 0x02;
    output[1] = (input[0] & 0x03) << 0x04;
    if (inputlen == 1) {
        output[2] = PADDING_CHAR;
        output[3] = PADDING_CHAR;
    } else {
        output[1] |= (input[1] & 0xf0) >> 0x04;
        output[2] = (input[1] << 0x02) & 0x3f;
        output[3] = PADDING_CHAR;
    }
}


unsigned char *base64encode(const unsigned char *input, unsigned int length) {
    unsigned int newlen = encodedlen(length);
    unsigned char *base64 = (unsigned char *) malloc(sizeof(unsigned char) * (newlen + 1));

    unsigned char *original[3];
    unsigned char *encoded = (unsigned char *) malloc(sizeof(unsigned char) * 4);

    int k = 0;
    for (int i = 0; i < length; i += 3) {

        /*
         * Determining the amount of values left to encode;
         */
        unsigned int lim = 3;
        if (i + 3 > length)
            lim = length - i;

        /*
         * Copying the input values;
         */
        for (int j = 0; j < lim; ++j)
            original[j] = input[i + j];

        /*
         * Encoding the copied values and writing these values to the
         * resulting array;
         */
        if (lim == 3)
            encodegroup(original, encoded);
        else
            encoderemainder(original, lim, encoded);
        for (int j = 0; j < 4; ++j)
            base64[k++] = encoded[j];
    }
    base64[newlen] = 0x00;
    return base64;
}

void testS(unsigned char * string, int len) {
    unsigned int s = encodedlen(len);
    unsigned char *p = base64encode(string, len);
    printf("%sn (len: %d)\n", string, len);
    for (int i = 0; i < s + 1; ++i) {
        if (i == s)
            printf("|\t ");
        printf("0x%02x\t", p[i]);
    }
    printf("\n");
}

void test() {
    testS("ManMan", 6);
    testS("ManMa", 5);
    testS("ManM", 4);
    testS("Man", 3);
    testS("Ma", 2);
    testS("M", 1);
}
