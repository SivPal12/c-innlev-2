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
  char *pFormattedKey = malloc(0);
  // TODO Insert correct values
  formatKey("ABCABCABC", pFormattedKey);
  printf("Formatted key:\t%s\n\n", pFormattedKey);

  free(pFormattedKey);
}

int formatKey(const char *key, char *formattedKey) {
  int numChars = 4098;

  int i;
  for (i = 0; key[i]; i++ ) {
    if (numChars < i) {
      numChars += 4098;
      formattedKey = realloc(formattedKey, sizeof(char)*numChars);
    }
    char currentChar = key[i];
    // If uppercase
    if (currentChar >= 'A' && currentChar <= 'Z') {
      // Switch casing
      currentChar ^= ' ';
    }
    formattedKey[i] = currentChar;
    printf("New char: %c\n", currentChar);
  }
  // Resize formattedKey
  formattedKey = realloc(formattedKey, sizeof(char)*i);
  printf("formatted: %s\n", formattedKey);
  return 0;
}
