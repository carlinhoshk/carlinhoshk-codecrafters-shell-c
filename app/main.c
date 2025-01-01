#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char input[100];

  while (1) {
    printf("$ ");
    fflush(stdout);

    fgets(input, 100, stdin);

    if (input[strlen(input) - 1] = '\n') {
      printf("%s: command not found\n", input);
      exit(0);
    }
  }
  return 0;
}
