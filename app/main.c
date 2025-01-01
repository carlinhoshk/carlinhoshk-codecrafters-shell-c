
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> // Para access()

int main() {
  char input[100];

  while (1) {
    printf("$ ");
    fflush(stdout);

    // Lê a entrada e remove o '\n' ou '\r\n'
    if (fgets(input, sizeof(input), stdin) == NULL) {
      perror("Erro ao ler entrada");
      continue;
    }
    input[strcspn(input, "\r\n")] = 0;

    // Verifica se o comando é "exit 0" para encerrar o programa
    if (strcmp(input, "exit 0") == 0) {
      return 0;
    }

    // Implementa o comando "type"
    if (strncmp(input, "type ", 5) == 0) {
      char *command = input + 5; // Pega o comando após "type "

      // Verifica se é um shell builtin
      if (strcmp(command, "echo") == 0 || strcmp(command, "exit") == 0 ||
          strcmp(command, "type") == 0) {
        printf("%s is a shell builtin\n", command);
        continue;
      }

      // Procura o comando no PATH
      char *path = getenv("PATH"); // Recupera a variável de ambiente PATH
      if (path == NULL) {
        printf("Erro: PATH não definido\n");
        continue;
      }

      char *path_copy = strdup(path); // Faz uma cópia para tokenização
      if (path_copy == NULL) {
        perror("Erro de memória");
        continue;
      }

      char *dir = strtok(path_copy, ":"); // Divide os diretórios no PATH
      int found = 0;
      while (dir != NULL) {
        char full_path[256];
        snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);

        // Verifica se o comando é executável
        if (access(full_path, X_OK) == 0) {
          printf("%s is %s\n", command, full_path);
          found = 1;
          break;
        }

        dir = strtok(NULL, ":");
      }

      free(path_copy); // Libera a memória alocada

      if (!found) {
        printf("%s: not found\n", command);
      }

      continue;
    }

    // Implementa o comando "echo"
    if (strncmp(input, "echo ", 5) == 0) {
      printf("%s\n", input + 5);
      continue;
    }

    // Comando não reconhecido
    printf("%s: command not found\n", input);
  }
}
