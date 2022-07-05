#include <stddef.h>

#ifndef P
    #define P 61 // a "reasonable" prime number (roughly equal to the size of the alphabet for the chars) (used for hashing)
#endif

#ifndef M
    #define M 1000000009 // a big prime number (used for hashing)
#endif

// filters every char to only allow the correct ones to be shown
char filter(char c) {
    return 
        (c >= '0' && c<= '9') * (c - '0') + // scale down numbers
        (c >= 'A' && c<= 'Z') * (c - 'A' + 10) + // scale down upper case letters
        (c >= 'a' && c<= 'z') * (c - 'a' + 10 + 26) + // scale down lower case letters
        (c == '-') * (c - '-' + 10 + 26 + 26) + // scale down special char -
        (c == '_') * (c - '_' + 10 + 26 + 26 + 1); // scale down special char _
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