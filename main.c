#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INIT_SET_SIZE 32768 // words are contained in a set, the set grows as it fills up.
#define HASH_A_SIZE 64 // size of the alphabet used by the hasher
#define HASH_P1 29791 // prime number used in rolling hashset_pos

typedef struct {
    bool d; // dirty flag
    char **arr; // array of words
    size_t np; // next empty position
    size_t s; // size of array
} WA; // words array

struct limit {
    bool lr; // limit reached
    size_t lg; // limit guess
};

typedef struct {
    char *cc; // correct chars
    bool *ic; // incorrect chars
    struct limit *l; // limits for every symbol
    WA vw;
} RWI; // reference word info

static size_t get_char_id(char c);

static void load_words();

static bool is_viable(const char *w);

static void filter_guesses();

static int comparator(const void *p1, const void *p2);

static size_t hashset_pos(size_t hash, size_t i);

static bool is_in_set(const char *key);

static void put_in_set(char *key);

bool ng; // new game flag
RWI guesser; // words guesser info
size_t wl; // words len
char **set; // set of added words
size_t set_size = INIT_SET_SIZE; // max amount of words in set
size_t set_used = 0; // tracks amount of added words
char *input;

int main() {
    set = calloc(INIT_SET_SIZE, sizeof(char *));

    // reads words len
    if (scanf(" %zu", &wl) == EOF) return 0;

    input = malloc(sizeof(char) * (wl + 1));

    guesser.cc = calloc(wl, sizeof(char));
    guesser.ic = calloc(wl * HASH_A_SIZE, sizeof(bool));
    guesser.l = calloc(HASH_A_SIZE, sizeof(struct limit));
    guesser.vw.s = 0;
    guesser.vw.np = 0;
    guesser.vw.arr = NULL;

    // reads words and populates the set and the words in mixed order
    ng = true;
    load_words();

    // cleans stdin of the start command
    if (scanf(" %*s") == EOF) return 0;

    // reads the "reference" w
    char reference[wl + 1]; // word to win the game
    if (scanf(" %[0-9a-zA-Z_-]", reference) == EOF) return 0;

    // count occurrences of chars from the reference
    size_t rwcc[HASH_A_SIZE] = {0}; // reference word char counts
    for (size_t i = 0; i < wl; i++) {
        size_t fc = get_char_id(reference[i]);
        rwcc[fc]++;
    }

    // reads max playable words
    size_t mw; // max words
    if (scanf(" %zu", &mw) == EOF) return 0;

    // read accepted w and compute the distance from the reference
    char w[wl + 1];
    char correctness[wl + 1];
    correctness[wl] = '\0';

    // read at max mw words from stdin or stops at the first word not matching pattern
    size_t iters = 0;

    while (true) {
        while (iters < mw && scanf(" %[0-9a-zA-Z_-]", w)) {
            // transforms the read w into the correctness info
            if (is_in_set(w)) {
                iters++;

                // flag to unset as soon as a non correct match is found
                bool all_correct = true;

                // counts the overlapping occurrences
                // (correct and possibly misplaced) of chars from the reference
                size_t rwh[HASH_A_SIZE] = {0}; // reference word hits

                // first cycle maps all correct and clearly incorrect chars
                for (size_t j = 0; j < wl; j++) {
                    char c = w[j];
                    size_t fc = get_char_id(c);
                    if (c == reference[j]) {
                        rwh[fc]++; // save accepted positive hit
                        guesser.cc[j] = c; // save the correct char position
                        correctness[j] = '+';
                    } else if (rwcc[fc] == 0) {
                        all_correct = false;
                        guesser.l[fc].lr = true; // save the limit of an incorrect char to 0
                        correctness[j] = '/';
                    } else {
                        all_correct = false;
                        correctness[j] = '|';
                    }
                }

                // if the correct word was found end the game and print ok
                if (all_correct) {
                    printf("ok\n");
                    break;
                }

                // second cycle maps possibly misplaced values and marks them as incorrect if they are too many
                for (size_t j = 0; j < wl; j++) {
                    char c = w[j];
                    size_t fc = get_char_id(c);
                    if (correctness[j] == '|') {
                        if (rwcc[fc] > rwh[fc]) {
                            rwh[fc]++; // add accepted positive hit
                        } else {
                            guesser.l[fc].lr = true;
                            correctness[j] = '/';
                        }
                        guesser.ic[j * HASH_A_SIZE + fc] = true;
                    }
                    if (guesser.l[fc].lg < rwh[fc]) guesser.l[fc].lg = rwh[fc];
                }

                // use the gained information to filter the viable words
                filter_guesses();

                // print the correctness info
                // print the amount of viable words left
                printf("%s\n%zu\n", correctness, guesser.vw.np);

                if (iters == mw) { // if the correct word is not found by the time the last word is inputted
                    // end the game and print ko.
                    printf("ko\n");
                    break;
                }
            } else {
                // print if the w is not accepted
                printf("not_exists\n");
            }
        }

        char command;
        if (scanf(" +%c%*s", &command) == EOF) return 0;
        switch (command) {
            case 's': {
                if (ng) {
                    filter_guesses();
                }
                if (guesser.vw.d) {
                    qsort(guesser.vw.arr, guesser.vw.np, sizeof(char *), comparator);
                    guesser.vw.d = false;
                }
                for (int i = 0; i < guesser.vw.np; ++i) {
                    printf("%s\n", guesser.vw.arr[i]);
                }
                break;
            }
            case 'i': {
                load_words();
                if (scanf(" %*s") == EOF) return 0;
                break;
            }
            case 'n': {
                ng = true;
                // reset the guess list
                memset(guesser.cc, 0, wl * sizeof(char));
                memset(guesser.ic, 0, wl * HASH_A_SIZE * sizeof(bool));
                memset(guesser.l, 0, HASH_A_SIZE * sizeof(struct limit));

                // reads the "reference" w
                if (scanf(" %[0-9a-zA-Z_-]", reference) == EOF) return 0;

                // count occurrences of chars from the reference
                memset(rwcc, 0, HASH_A_SIZE * sizeof(size_t));
                for (size_t i = 0; i < wl; i++) {
                    size_t fc = get_char_id(reference[i]);
                    rwcc[fc]++;
                }

                // reads max playable words
                if (scanf(" %zu", &mw) == EOF) return 0;

                // read at max mw words from stdin or stops at the first word not matching pattern
                iters = 0;
                break;
            }
        }
    }
}

static size_t get_char_id(const char c) {
    if (c == '_') return 63;
    else if (c == '-') return 62;
    else if (c >= 'a') return c - 'a' + 36;
    else if (c >= 'A') return c - 'A' + 10;
    else return c - '0';
}

static size_t hash(const char *s) {
    size_t h1 = 0;
    size_t p1 = 1;
    for (size_t j = 0; s[j]; j++) {
        size_t c = get_char_id(s[j]);
        h1 += (c * p1);
        p1 *= HASH_P1;

    }

    return h1;
}

static size_t hashset_pos(size_t hash, size_t i) {
    size_t h1 = hash;
    size_t h2 = hash % (set_size - 1);
    h2 = h2 == 0 ? 1 : h2;

    return (h1 + i * h2) & (set_size - 1);
}

static bool is_in_set(const char *key) {
    size_t i = 0;
    size_t h = hash(key);

    while (i < set_size &&
           (i == 0 || hashset_pos(h, i) != hashset_pos(h, 0)) &&
           set[hashset_pos(h, i)] != NULL) {
        if (strcmp(key, set[hashset_pos(h, i++)]) == 0) {
            return true;
        }
    }

    return false;
}

static void expand_set() {
    char **old_set = set;
    size_t old_set_size = set_size;
    set_size = set_size << 1;
    set = calloc(set_size, sizeof(char *));

    for (int j = 0; j < old_set_size; ++j) {
        if (old_set[j] != NULL) put_in_set(old_set[j]);
    }

    free(old_set);
}

static void put_in_set(char *key) {
    size_t i = 0;
    size_t h = hash(key);

    while (i < set_size &&
           (i == 0 || hashset_pos(h, i) != hashset_pos(h, 0))) {
        if (set[hashset_pos(h, i)] == NULL) {
            set[hashset_pos(h, i)] = key;
            set_used++;
            return;
        }
        i++;
    }
    expand_set();
    put_in_set(key);
}

static void load_words() {
    while (scanf(" %[0-9a-zA-Z_-]", input) != 0) {
        put_in_set(input);

        if (!ng && is_viable(input)) {
            guesser.vw.d = true;
            if (guesser.vw.np == guesser.vw.s) {
                guesser.vw.s += guesser.vw.s / 2;
                guesser.vw.arr = realloc(guesser.vw.arr, guesser.vw.s * sizeof(char *));
            }
            guesser.vw.arr[guesser.vw.np++] = input;
        }
        input = malloc(sizeof(char) * (wl + 1));
    }
}

static bool is_viable(const char *w) {
    size_t counts[HASH_A_SIZE] = {0}; // counts occurrences of chars in the string
    for (size_t i = 0; i < wl; i++) {
        char c = w[i]; // char at the evaluated position
        size_t fc = get_char_id(c);
        counts[fc]++;
        char cc = guesser.cc[i]; // a correct char is found for the position (or 0 if absent)
        if (cc) { // if a correct char was found
            if (cc != c) { // if the evaluated char is not correct
                return false;
            }
        }
        if (guesser.ic[i * HASH_A_SIZE + get_char_id(c)]) return false;
        if (guesser.l[fc].lr) {
            if (counts[fc] > guesser.l[fc].lg) {
                return false;
            }
        }
    }
    // evaluate minimum count requirements
    for (size_t i = 0; i < HASH_A_SIZE; i++) {
        if (counts[i] < guesser.l[i].lg) {
            return false;
        }
    }
    return true;
}

static void filter_guesses() {
    size_t vwnp = 0;

    if (ng) {
        ng = false;
        guesser.vw.d = true;
        if (guesser.vw.s < set_used) {
            guesser.vw.arr = realloc(guesser.vw.arr, set_used * sizeof(char *));
            guesser.vw.s = set_used;
        }
        for (size_t i = 0; i < set_size; ++i) {
            if (set[i] != NULL) {
                char *w = set[i];
                if (is_viable(w)) guesser.vw.arr[vwnp++] = w;
            }
        }
    } else {
        for (size_t i = 0; i < guesser.vw.np; ++i) {
            char *w = guesser.vw.arr[i];
            if (is_viable(w)) guesser.vw.arr[vwnp++] = w;
        }
    }
    guesser.vw.np = vwnp;
}

static int comparator(const void *p1, const void *p2) {
    // taken from qsort man page
    return strcmp(*(const char **) p1, *(const char **) p2);
}