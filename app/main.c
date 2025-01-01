
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char input[100];

  while (1) {
    printf("$ ");
    fflush(stdout);

    if (fgets(input, sizeof(input), stdin) == NULL) {
      perror("Erro ao ler entrada");
      continue;
    }
    input[strcspn(input, "\r\n")] = 0;

    if (strcmp(input, "exit 0") == 0) {
      return 0;
    }

    if (strncmp(input, "type ", 5) == 0) {
      char *command = input + 5; // Pega o comando após "type "
      if (strcmp(command, "echo") == 0 || strcmp(command, "exit") == 0 ||
          strcmp(command, "type") == 0) {
        printf("%s is a shell builtin\n", command);
      } else {
        printf("%s: not found\n", command);
      }
      continue;
    }

    if (strncmp(input, "echo ", 5) == 0) {
      printf("%s\n", input + 5);
      continue;
    }

    printf("%s: command not found\n", input);
  }
}
