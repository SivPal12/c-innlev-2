#include <stdio.h>
#include <stdlib.h>

int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream);

int formatKey(const char *keyFile, char *formattedKey);

int main (int argc, char *argv[]){
  char *toEncode = "abc";
  char *pEncoded;
  char *pKeyFile = "";
  encode(toEncode, pKeyFile, pEncoded);

  printf("%s", pEncoded);
}

int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream) {
  // TODO Handle realloc failed
  char *pFormattedKey = malloc(0);
  // TODO Insert correct values
  formatKey("ABCABCABC. ,", pFormattedKey);
  printf("Formatted key:\t%s\n\n", pFormattedKey);

  free(pFormattedKey);
}

int formatKey(const char *key, char *formattedKey) {
  int numChars = 4098;

  int currentIndexInFormattedKey = 0;
  for (int i = 0; key[i]; i++ ) {
    if (numChars < i) {
      numChars += 4098;
      // Handle realloc failed
      formattedKey = realloc(formattedKey, sizeof(char)*numChars);
    }
    char currentChar = key[i];
    // TODO Use string lib
    // If uppercase
    if (currentChar >= 'A' && currentChar <= 'Z') {
      // Switch casing
      currentChar ^= ' ';
    }
    if (currentChar >= 'a' && currentChar <= 'z') {
      formattedKey[currentIndexInFormattedKey++] = currentChar;
    }
    printf("New char: %c\n", currentChar);
  }
  // Resize formattedKey
  // TODO Handle realloc failed
  formattedKey = realloc(formattedKey, sizeof(char)*currentIndexInFormattedKey);
  printf("formatted: %s\n", formattedKey);
  return 0;
}
