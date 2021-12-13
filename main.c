//
// Created by Thijs van Esch on 11/12/2021.
//

#include <stdio.h>
#include <string.h>

#include "base64.h"

int main() {
    uint8_t s[] = "Het is tijd om mij aandacht te gevën :)";
    uint8_t *sp = s;
    uint32_t l = encodedlen(strlen(sp));
    uint8_t *k = base64encode(s,strlen(sp));
    for (int i = 0; i < l; ++i) {
        printf("%c", k[i]);
    }
}
