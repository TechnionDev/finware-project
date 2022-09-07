#include <Arduino.h>
#include "utils.h"
#include <stdlib.h>
#include <string.h>

void reverseheb(char* string) {
  int i, j;
  char* temp;
  //
  j = strlen(string);
  temp = (char*)malloc(j + 1);
  temp[j] = 0;
  for (i = 0; string[i]; i++) {
    if (string[i] == (char)0xD7) {
      temp[j - i - 1] = string[i + 1];
      temp[j - i - 2] = (char)0xD7;
      i++;
    } else {
      if ((string[i] < '0' || string[i] > '9') &&
          (string[i] < 'A' || string[i] > 'Z') &&
          (string[i] < 'a' || string[i] > 'z'))
        temp[j - i - 1] = string[i];
      else {
        int m, k;
        k = i;
        while ((string[i] >= '0' && string[i] <= '9') ||
               (string[i] >= 'A' && string[i] <= 'Z') ||
               (string[i] >= 'a' && string[i] <= 'z')) {
          i++;
        }
        i--;
        for (m = 0; m < (i - k + 1); m++) {
          temp[j - (k + m) - 1] = string[i - m];
        }
      }
    }
  }
  strcpy(string, temp);
  free(temp);
}

void blockUntilPress() {
  int last = 1;
  int now = digitalRead(GPIO_NUM_39);
  while (now == 1) {
    if (now != last) {
      Serial.printf("Now: %d, Last:%d\n", now, last);
    }
    last = now;
    delay(1);
    now = digitalRead(GPIO_NUM_39);
  }
}