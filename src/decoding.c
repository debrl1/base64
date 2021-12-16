//
// Created by Thijs van Esch on 16/12/2021.
//

#include "../include/base64.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "digits.h"

#define PADDING 0x40

/*
 * Derives the length of the original (unencoded) string from the length of
 * the encoded string.
 */
uint32_t base64_dlen(const uint8_t *input) {
    uint8_t padding = 0;
    size_t length = strlen((char *) input);
    padding = input[length - 1] == '=' ? padding + 1 : padding;
    padding = input[length - 2] == '=' ? padding + 1 : padding;
    return ((length / 4) * 3) - padding;
}

/*
 * Decodes a base64 encoded group (containing exactly four characters) into
 * the original bytes. The resulting group contains exactly three characters.
 */
void decode_group(const uint8_t *input, uint8_t *output) {
    output[0] = (input[0] << 2) | (input[1] >> 4);
    output[1] = (input[1] << 4) | (input[2] >> 2);
    output[2] = (input[2] << 6) | input[3];
}

uint8_t *base64decode(const uint8_t *input, uint32_t length) {

    /*
     * Copying the encoded array and setting all base64 digits equal to their
     * 6-bit values;
     */
    uint8_t *copy = (uint8_t *) malloc(sizeof(uint8_t) * length);
    for (int i = 0; i < length; ++i) {
        copy[i] = input[i];
        from_digit(&copy[i]);
        if (copy[i] == PADDING)
            copy[i] = 0x00;
    }

    /*
     * Calculating the length of the decoded string and allocating memory
     * for it;
     */
    uint32_t decoded_length = base64_dlen(input);
    uint8_t *decoded = (uint8_t *) malloc(sizeof(uint8_t) * (decoded_length + 1));
    uint32_t decoded_index = 0;

    /*
     * Allocating memory for the arrays that will hold each group;
     */
    uint8_t *encoded_group = (uint8_t *) malloc(sizeof(uint8_t) * 4);
    uint8_t *decoded_group = (uint8_t *) malloc(sizeof(uint8_t) * 3);

    for (int i = 0; i < length; i += 4) {

        // Copying the input values (from the copy)
        for (int j = 0; j < 4; ++j)
            encoded_group[j] = copy[i + j];

        /*
         * Decoding the group and writing these decoded values to the array
         * that will be returned;
         */
        decode_group(encoded_group, decoded_group);
        for(int j = 0; j < 3; ++j)
            decoded[decoded_index++] = decoded_group[j];
    }

    /*
     * Freeing allocated memory;
     */
    free(copy);
    free(encoded_group);
    free(decoded_group);

    return decoded;
}
