
#include <stdlib.h>

#include "base64.h"

#define PADDING 0x40

/*
 * Derives the length of the encoded base64 string from the length of the
 * original string.
 */
uint32_t encodedlen(uint32_t len) {
    uint32_t newlen = (len / 3) * 4;
    if (len % 3 != 0)
        newlen += 4;
    return newlen;
}

/*
 * Derives the length of the original (unencoded) string from the length of
 * the encoded string.
 */
uint32_t decodedlen(const uint8_t *input, uint32_t len) {
    uint8_t padding = 0;
    for (uint32_t i = len - 1;; --i) {
        if (input[i] == PADDING)
            ++padding;
        else
            break;
    }
    return ((len / 4) * 3) - padding;
}

/*
 * Sets the 6-bit value equal to its base64 equivalent.
 */
void to_digit(uint8_t *digit) {
    if (*digit < 26)
        *digit += 'A';
    else if (*digit < 52)
        *digit += 'a' - 26;
    else if (*digit < 62)
        *digit += '0' - 52;
    else if (*digit == 62)
        *digit = '+';
    else if (*digit == 63)
        *digit = '/';
    else
        *digit = '=';
}

/*
 * Sets the base64 value equal to its 6-bit index.
 */
void from_digit(uint8_t *digit) {
    if (('A' <= *digit) && (*digit <= 'Z'))
        *digit -= 'A';
    else if (('a' <= *digit) && (*digit <= 'z'))
        *digit = *digit - 'a' + 26;
    else if (('0' <= *digit) && (*digit <= '9'))
        *digit = *digit - '0' + 52;
    else if (*digit == '+')
        *digit = 62;
    else if (*digit == '/')
        *digit = 63;
    else
        *digit = PADDING;
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

/*
 * Decodes a base64 encoded group (containing exactly four characters) into
 * the original bytes. The resulting group contains exactly three characters.
 */
void decode_group(const uint8_t *input, uint8_t *output) {
    output[0] = (input[0] << 2) | (input[1] >> 4);
    output[1] = (input[1] << 4) | (input[2] >> 2);
    output[2] = (input[2] << 6) | input[3];
}

uint8_t *base64encode(const uint8_t *input, uint32_t length) {
    
    /*
     * Calculating the length of the array that will be returned and 
     * allocating memory for it; 
     */
    uint32_t newlen = encodedlen(length);
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
    uint32_t decoded_length = decodedlen(copy,  length);
    uint8_t *decoded = (uint8_t *) malloc(sizeof(uint8_t) * (decoded_length + 1));
    uint32_t decoded_index = 0;

    /*
     * Allocating memory for the arrays that will hold each group;
     */
    uint8_t *encoded_group = (uint8_t *) malloc(sizeof(uint8_t) * 4);
    uint8_t *decoded_group = (uint8_t *) malloc(sizeof(uint8_t) * 3);

    for (int i = 0; i < length; i += 4) {

        /*
         * Copying the input values (from the copy);
         */
        for (int j = 0; j < 4; ++j)
            encoded_group[j] = copy[i + j];

        /*
         * Decoding the group and writing these decoded values to the array
         * that will be returned;
         */
        decode_group(encoded_group, decoded_group);
        for (int j = 0; j < 3; ++j)
            decoded[decoded_index++] = decoded_group[j];
    }

    /*
     * Freeing allocated memory;
     */
    free(copy);
    free(encoded_group);
    free(decoded_group);

    /*
     * Delimiting the string;
     */
    decoded[decoded_length] = '\0';

    return decoded;
}
