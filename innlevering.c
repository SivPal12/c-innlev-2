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
  printf("Formatted key:\n%s\n\n", pFormattedKey);

  // TODO Read msg file
  char *msgToEncode = "There is a concert.......";

  int msgIndex = -1, keyIndex = 0, encodedStreamSize = 0;
  while (msgToEncode[++msgIndex] != '\0') {
    // TODO better logic
    if (encodedStreamSize <= msgIndex +10) {
      encodedStreamSize += 4098;
      //TODO Handle failed
      encodedStream = realloc(encodedStream, sizeof(char)*encodedStreamSize);
    }
    // if [a-zA-Z]
    if (msgToEncode[msgIndex] >= 'a' &&
        msgToEncode[msgIndex] <= 'z' ||
        msgToEncode[msgIndex] >= 'A' &&
        msgToEncode[msgIndex] <= 'Z') {
      encodedStream[keyIndex++] = 'n';
      encodedStream[keyIndex++] = 'a';
    } else {
      encodedStream[keyIndex++] = msgToEncode[msgIndex];
    }
  }

  // TODO                                                        v -- Why??
  encodedStream = realloc(encodedStream, keyIndex*sizeof(char) + 1);

  printf("Encoded msg:\n%s\n\n", encodedStream);

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
