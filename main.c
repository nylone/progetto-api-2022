#include "libraries/string-map.h"
#include "libraries/string-hasher.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  StringMap *allowed = create_map(4096);

  // reads words len
  size_t words_len;
  scanf("%zu", &words_len);
  
  // reads words and populates the map
  char *word;
  while (scanf(" %m[0-9a-zA-Z_-]", &word) != 0) {
    set_map_value(allowed, word, (void *)1);
  }

  // cleans stdin of the start command
  scanf(" +nuova_partita");

  // reads the "reference" word
  char *ref_word = malloc(sizeof(char) * words_len + 1);
  scanf(" %[0-9a-zA-Z_-]", ref_word);

  // count occurrences of chars from the reference
  size_t ref_counts[63];
  for (size_t i = 0; i < words_len; i++) {
    char c = filter(ref_word[i]);
    ref_counts[c]++;
  }

  // reads max playable words
  size_t max_words;
  scanf(" %zu", &max_words);

  // read a word and compute the distance from the reference
  word = malloc(sizeof(char) * words_len + 1);
  char* correctness = malloc(sizeof(char) * words_len + 1);
  correctness[words_len] = '\0';
  for (size_t i = max_words; i > 0; i--) {
    // reads a word
    scanf(" %[0-9a-zA-Z_-]", word);

    // transforms the read word into the correctness info
    if (get_map_value(allowed, word) == (void*) 1) {

      // counts the overlapping occurrences
      // (correct and possibly misplaced) of chars from the reference
      size_t ref_hits[63];

      // first cycle maps all correct and clearly incorrect chars
      for (size_t j = 0; j < words_len; j++) {
        char c = word[j];
        char fc = filter(c);
        correctness[j] = 0 + (c == ref_word[j] && ++ref_hits[fc]) * '+' + (ref_counts[fc] == 0) * '/';
      }

      // second cycle maps possibly misplaced values and marks them as incorrect if they are too many
      for (size_t j = 0; j < words_len; j++) {
        if (correctness[j] == 0) {
          char fc = filter(word[j]);
          correctness[j] = '/' + (ref_counts[fc] > ref_hits[fc]++) * ('|' - '/');
        }
      }

      // print the correctness info
      printf("%s\n", correctness);
    } else {

      // print if the word is not allowed
      printf("not exists\n");
    }
  }

  printf("done\n");
}