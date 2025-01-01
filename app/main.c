#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char input[100];

  while (1) {
    printf("$ ");
    fflush(stdout);

    fgets(input, 100, stdin);
    input[strcspn(input, "\r\n")] = 0;

    if (!strcmp(input, "exit 0")) {
      return 0;
    }
    if (strncmp(input, "echo", strlen("echo")) == 0) {
      printf("%s\n", input + 5);
      continue;
    }
    printf("%s: command not found\n", input);
  }
}
