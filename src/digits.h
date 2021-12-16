//
// Created by Thijs van Esch on 16/12/2021.
//

#ifndef B64_DIGITS_H
#define B64_DIGITS_H

#include <stdint.h>

/*
 * Sets the 6-bit value equal to its base64 equivalent.
 */
void to_digit(uint8_t *digit);

/*
 * Sets the base64 value equal to its 6-bit index.
 */
void from_digit(uint8_t *digit);

#endif // B64_DIGITS_H
