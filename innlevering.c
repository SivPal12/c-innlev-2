#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream);

int formatKey(const char *keyFile, char *formattedKey);

char *readFile(const char *file);

int encodeChar(char charToEncode, char *formattedKey, char *encodedChar, int keyIndex);

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

  char *msgToEncode = "There is a consert.....";
  int msgIndex = 0, encodedSize = 0, keyIndex = 0, bufferSize = 0;
  while (msgToEncode[msgIndex] != '\0') {
    char *encodedChar = malloc(0);
    keyIndex = encodeChar(msgToEncode[msgIndex], pFormattedKey, encodedChar, keyIndex);
    int encodedCharLength = strlen(encodedChar);
    encodedSize += encodedCharLength;
    if (bufferSize <= encodedSize) {
      bufferSize += 4098;
      // TODO handle failed
      encodedStream = realloc(encodedStream, sizeof(char)*bufferSize);
    }
    strcat(encodedStream, encodedChar);
    free(encodedChar);
    msgIndex++;
  }

  // TODO Handle failed
  encodedStream = realloc(encodedStream, sizeof(char)*encodedSize);

  printf("Encoded:\n%s\n", encodedStream);

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

int encodeChar(char charToEncode, char *key, char *encodedChar, int keyIndex) {
printf("To encode: %c\n", charToEncode);
  if (!isalpha(charToEncode)) {
    encodedChar = realloc(encodedChar, sizeof(char)*2);
    char buffer[2];
    sprintf(buffer, "%c", charToEncode);
    strcpy(encodedChar, buffer);
    return keyIndex;
  }
  int keyLength = strlen(key);

  while (tolower(charToEncode) != key[keyIndex]) {
    if (keyIndex++ >= keyLength) {
      keyIndex = 0;
    }
  }

  encodedChar = realloc(encodedChar, sizeof(int)+4);
  char *buffer;

printf("PreS size: %lu\n", sizeof(encodedChar));
  if (islower(charToEncode)) {
//    strcpy(encodedChar, "[%d]");
    buffer = "[%d]";
  } else {
//    strcpy(encodedChar, "[-%d]");
    buffer = "[-%d]";
  }
printf("Post size: %lu\n", sizeof(encodedChar));

//  sprintf(encodedChar, encodedChar, keyIndex+1);
  sprintf(encodedChar, buffer, keyIndex+1);
printf("Encoded char: %s\n", encodedChar);

  return keyIndex;
}
