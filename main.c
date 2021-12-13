//
// Created by Thijs van Esch on 11/12/2021.
//

#include <stdio.h>

#include "base64.h"

int main() {
    uint8_t s[] = "Man";
    uint32_t l = encodedlen(3);
    uint8_t *k = base64encode(s, l);
    for (int i = 0; i < l; ++i) {
        printf("0x%02x\t", k[i]);
    }
}
