//
// Created by Thijs van Esch on 16/12/2021.
//

#include "digits.h"

#include <stdint.h>

static const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

void to_digit(uint8_t *digit) {
    *digit = alphabet[*digit];
}

void from_digit(uint8_t *digit) {
    for (int i = 0; i < 65; ++i) {
        if (*digit == alphabet[i]) {
            *digit = i;
            return;
        }
    }
}
