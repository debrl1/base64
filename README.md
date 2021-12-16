
## base64 encoding/decoding library for C

This repository holds a base64 library I wrote in order to learn more about programming in C.

Features:

 * Ability to encode any array of bytes into base64
 * Ability to decode any base64 encoded strings back into their original bytes

#### Sample usage:

    #include <stdio.h>

    #include "base64.h"

    int main() {
        char *str = "This is an unencoded string!";
    
        // Encoding
        uint8_t *encoded = base64_encode((uint8_t *) str, 28);
        uint32_t encoded_length = base64_elen(28);
        
        // Decoding
        uint8_t *decoded = base64decode(encoded, encoded_length);
    
        printf("Original: %s\n", str);
        printf("Encoded:  %s\n", encoded);
        printf("Decoded:  %s\n", decoded);
    }

