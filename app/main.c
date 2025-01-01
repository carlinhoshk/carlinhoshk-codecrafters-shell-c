
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
  char input[100];

  while (1) {
    printf("$ ");
    fflush(stdout);

    if (!fgets(input, sizeof(input), stdin)) {
      break;
    }

    // Remove newline do final da entrada
    input[strcspn(input, "\r\n")] = 0;

    if (strcmp(input, "exit") == 0) {
      return 0;
    }

    // Comando `type`
    if (strncmp(input, "type ", 5) == 0) {
      char *command = input + 5; // Comando após "type "

      // Verificar comandos internos
      if (strcmp(command, "echo") == 0 || strcmp(command, "exit") == 0 ||
          strcmp(command, "type") == 0) {
        printf("%s is a shell builtin\n", command);
      } else {
        // Verificar comandos externos no PATH
        char *path = getenv("PATH");
        if (path) {
          char path_copy[1024];
          strncpy(path_copy, path, sizeof(path_copy) - 1);
          path_copy[sizeof(path_copy) - 1] = '\0';

          char *dir = strtok(path_copy, ":");
          int found = 0;
          while (dir) {
            char full_path[200];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
            if (access(full_path, X_OK) == 0) {
              printf("%s is %s\n", command, full_path);
              found = 1;
              break;
            }
            dir = strtok(NULL, ":");
          }
          if (!found) {
            printf("%s: not found\n", command);
          }
        }
      }
      continue;
    }

    // Executar comandos externos com argumentos
    pid_t pid = fork();
    if (pid == 0) {
      // Processo filho: executar o comando
      char *args[10];
      int arg_count = 0;

      // Dividir o input em argumentos
      char *token = strtok(input, " ");
      while (token && arg_count < 9) {
        args[arg_count++] = token;
        token = strtok(NULL, " ");
      }
      args[arg_count] = NULL;

      // Executar o comando
      execvp(args[0], args);
      // Se execvp falhar
      perror("Error");
      exit(1);
    } else if (pid > 0) {
      // Processo pai: esperar o filho terminar
      int status;
      waitpid(pid, &status, 0);
    } else {
      perror("fork failed");
    }
  }

  return 0;
}
