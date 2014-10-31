#include <stdio.h>
#include <stdlib.h>

const char *outputFile = "encodedText.txt";

int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream);

int formatKey(const char *keyFile, char *formattedKey);

char *readFile(const char *file);

int main (int argc, char *argv[]){
  char *toEncode = "abc";
  char *pEncoded;
  char *pKeyFile = "songLibrary/sweetChildGR.txt";
  encode(toEncode, pKeyFile, pEncoded);

  printf("%s", pEncoded);
}

int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream) {
  char *rawKey = readFile(keyFile);
  printf("Raw key:\n%s\n", rawKey);
  // TODO Handle realloc failed
  char *pFormattedKey = malloc(0);
  // TODO Insert correct values
  formatKey(rawKey, pFormattedKey);
  printf("Formatted key:\t%s\n\n", pFormattedKey);

  FILE *pFile;
  pFile = fopenf(outputFile, "a");
  if (pFile == NULL) {
    printf("Could not open file: '%s'", outputFile);
    return 1;
  }
  
  fprintf(pFile, "[%d]", 
  fclose(pFile);

  free(pFormattedKey);
  free(rawKey);
}

int formatKey(const char *key, char *formattedKey) {
  int numChars = 0;
  int currentIndexInFormattedKey = 0;
  for (int i = 0; key[i]; i++ ) {
    if (numChars < i) {
      numChars += 4098;
      char *pTmp = realloc(formattedKey, sizeof(char)*numChars);
      if (pTmp == NULL) {
        printf("Realloc failed\n");
        return 1;
      }
      formattedKey = pTmp;
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
  }
  // Resize formattedKey
  char *pTmp = realloc(formattedKey, sizeof(char)*currentIndexInFormattedKey);
  if (pTmp == NULL) {
    printf("Realloc failed\n");
    return 1;
  }
  formattedKey = pTmp;
  return 0;
}

char *readFile(const char *file) {
  int bufferSize = 4098;
  char *buffer = malloc(sizeof(char)*bufferSize);

  FILE *pFile = fopen(file, "r");
  if (!pFile) {
    printf("Cound not read file '%s'\n", file);
    return NULL;
  }
  int fileSize = 0;
  while (!feof(pFile)) {
    if (++fileSize > bufferSize) {
      bufferSize += 4098;
      buffer = realloc(buffer, sizeof(char)*bufferSize);
    }
    buffer[fileSize-1] = fgetc(pFile);
  }
  fclose(pFile);
  buffer[fileSize-1] = '\0';
  return realloc(buffer, fileSize - 1);
}
