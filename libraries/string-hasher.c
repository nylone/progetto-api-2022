#include <stddef.h>
#include <stdio.h>

#define P 67 // a "reasonable" prime number (roughly equal to the size of the alphabet for the chars) (used for hashing)
#define M 1000000009 // a big prime number (used for hashing)

// filters every char to only allow the correct ones to be shown
char filter(const char c) {
    return 
        (c >= '0' && c<= '9') * (c - '0') + // scale down numbers
        (c >= 'A' && c<= 'Z') * (c - 'A' + 10) + // scale down upper case letters
        (c >= 'a' && c<= 'z') * (c - 'a' + 36) + // scale down lower case letters
        (c == '-') * (62) + // scale down special char -
        (c == '_') * (63); // scale down special char _
}

// computes the polynomial rolling hash function
size_t hash(const char* s) {
    size_t hash = 0;
    size_t pow = 1;
    for (size_t i = 0; s[i]; i++) {
        hash = hash + (filter(s[i]) * pow);
        pow = pow * P;
    }
    return hash % M;
}