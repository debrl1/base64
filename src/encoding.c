//
// Created by Thijs van Esch on 16/12/2021.
//

#include "../include/base64.h"

#include <stdint.h>
#include <stdlib.h>

#include "digits.h"

#define PADDING 0x40

/*
 * Derives the length of the encoded base64 string from the length of the
 * original string.
 */
uint32_t base64_elen(uint32_t len) {
    uint32_t newlen = (len / 3) * 4;
    if (len % 3 != 0)
        newlen += 4;
    return newlen;
}

/*
 * Encodes a group of values into base 64.
 *
 * The group that is to be encoded may contain either one, two or three
 * characters. The amount of characters in the group also has to be passed.
 * The encoded characters will be placed in the three-character array passed
 * as the third and last argument.
 */
void encode_group(const uint8_t *input, uint32_t input_length, uint8_t *output) {
    output[0] = input[0] >> 2;
    output[1] = (input[0] & 0x03) << 4;
    if (input_length == 1) {
        output[2] = PADDING;
        output[3] = PADDING;
    } else if (input_length == 2) {
        output[1] |= (input[1] & 0xf0) >> 4;
        output[2] = (input[1] << 2) & 0x3f;
        output[3] = PADDING;
    } else {
        output[1] |= (input[1] & 0xf0) >> 4;
        output[2] = ((input[1] << 2) & 0x3f) | (input[2] >> 6);
        output[3] = input[2] & 0x3f;
    }
}

uint8_t *base64_encode(const uint8_t *input, uint32_t length) {

    /*
     * Calculating the length of the array that will be returned and
     * allocating memory for it;
     */
    uint32_t newlen = base64_elen(length);
    uint32_t k = 0;
    uint8_t *base64 = (uint8_t *) malloc(sizeof(uint8_t) * (newlen + 1));

    /*
     * Allocating memory for the arrays that will hold the individual groups;
     */
    uint8_t *original = (uint8_t *) malloc(sizeof(uint8_t) * 3);
    uint8_t *encoded = (uint8_t *) malloc(sizeof(uint8_t) * 4);

    for (int i = 0; i < length; i += 3) {

        /*
         * Determining the amount of values left to encode;
         */
        uint32_t lim = 3;
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
        encode_group(original, lim, encoded);
        for (int j = 0; j < 4; ++j)
            base64[k++] = encoded[j];
    }

    /*
     * Freeing allocated memory;
     */
    free(original);
    free(encoded);

    /*
     * Turning the encoded values into printable characters and delimiting the
     * base64 string;
     */
    for (int i = 0; i < newlen; ++i)
        to_digit(&base64[i]);
    base64[newlen] = '\0';

    return base64;
}
