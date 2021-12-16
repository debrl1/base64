//
// Created by Thijs van Esch on 16/12/2021.
//

#include <stdio.h>
#include <string.h>

#include "../include/base64.h"

int main(int argc, char *argv[]) {
    unsigned char *encoded;
    for (int i = 1; i < argc; ++i) {
        encoded = base64_encode((unsigned char *) argv[i], strlen(argv[i]));
        printf("%s\n", encoded);
    }
    return 0;
}
