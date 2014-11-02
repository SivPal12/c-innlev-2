/* TODO
 * Fix - Formatted key is sometimes not the right length (abc.. key, 'hello' msg)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream, int minSpacing);

int formatKey(const char *keyFile, char *formattedKey);

char *readFile(const char *file);

int encodeChar(char charToEncode, char *formattedKey, char *encodedChar, int keyIndex);

/* return codes:
 * 1 - mem alloc failed
 * 2 - Could not open file
 * 3 - Unable to satisfy min spacing
 */
int main (int argc, char *argv[]){
  char *toEncode = "msg/msg.txt";
  char *pEncoded;
  char *pKeyFile = "songLibrary/sweetChildGR.txt";
  int minSpacing = 2;
  int status = encode(toEncode, pKeyFile, pEncoded, minSpacing);
  if (status != 0) {
    return status;
  }

  printf("%s", pEncoded);

  return 1;
}

int encode(const char *inputMessageFile, const char *keyFile,
    char *encodedStream, int minSpacing) {
  char *rawKey = readFile(keyFile);
  printf("Raw key:\n%s\n", rawKey);

  char *pFormattedKey = malloc(0);
  if (pFormattedKey == NULL) {
    printf("Malloc failed\n");
    return 1;
  }
  // TODO Insert correct values
  formatKey(rawKey, pFormattedKey);
  printf("Formatted key:\n%s\n\n", pFormattedKey);

  char *msgToEncode = readFile(inputMessageFile);
  if (msgToEncode == NULL) {
    return 2;
  }

  int msgIndex = 0, encodedSize = 0, keyIndex = strlen(pFormattedKey) + minSpacing + 1, bufferSize = 0;
  while (msgToEncode[msgIndex] != '\0') {
    char *encodedChar = malloc(0);
    if (encodedChar == NULL) {
      printf("Malloc failed\n");
      return 1;
    }

    int tmp = encodeChar(msgToEncode[msgIndex], pFormattedKey, encodedChar, keyIndex);
    if (tmp < 0) {
      return tmp;
    }

    printf("fabs: %d   keyIndex: %d   minSpacing: %d   tmp: %d   Keylen: %d\n",
        (int)(fabs((int)(tmp % strlen(pFormattedKey)))),2,3,4,5);//, keyIndex, minSpacing, tmp, (int)strlen(pFormattedKey));*/
    if (fabs((int)(tmp % strlen(pFormattedKey)) - (keyIndex - minSpacing)) < minSpacing) {
      printf("Unable to satisfy min spacing\n");
      return 3;
    }
    keyIndex = ((tmp + minSpacing) % (int)strlen(pFormattedKey));

    int encodedCharLength = strlen(encodedChar);
    encodedSize += encodedCharLength;
    if (bufferSize <= encodedSize) {
      bufferSize += 4098;

      encodedStream = realloc(encodedStream, sizeof(char)*bufferSize);
      if (encodedStream == NULL) {
        printf("Realloc failed\n");
        return 1;
      }
    }
    strcat(encodedStream, encodedChar);
    free(encodedChar);
    msgIndex++;
  }

  encodedStream = realloc(encodedStream, sizeof(char)*encodedSize);
  if (encodedStream == NULL) {
    printf("Realloc failed\n");
    return 1;
  }

  printf("Encoded:\n%s\n", encodedStream);

  free(pFormattedKey);
  free(rawKey);
  return 0;
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

  int numLoops = 0;
  while (tolower(charToEncode) != key[keyIndex]) {
    if (++keyIndex >= keyLength) {
      // TODO Avoid infinity loop
      if (++numLoops > 1) {
        printf("Unable to find char '%c' in key", charToEncode);
        return -1;
      }
      keyIndex %= keyLength;
    }
  }

  encodedChar = realloc(encodedChar, sizeof(int)+4);
  char *buffer;

  if (islower(charToEncode)) {
    buffer = "[%d]";
  } else {
    buffer = "[-%d]";
  }

  sprintf(encodedChar, buffer, keyIndex+1);

  return keyIndex;
}
