
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// Verifica se o arquivo no caminho especificado é executável
int is_executable(const char *path) { return access(path, X_OK) == 0; }

// Busca o comando no PATH
char *find_in_path(const char *cmd) {
  char *path = getenv("PATH");
  if (!path)
    return NULL;

  static char full_path[1024];
  char *dir = strtok(path, ":");
  while (dir) {
    snprintf(full_path, sizeof(full_path), "%s/%s", dir, cmd);
    if (is_executable(full_path)) {
      return full_path;
    }
    dir = strtok(NULL, ":");
  }
  return NULL;
}

// Cria um processo filho para executar o comando
void fork_and_exec_cmd(char *full_path, int argc, char **argv) {
  pid_t pid = fork();
  if (pid == 0) { // Processo filho
    execv(full_path, argv);
    perror("execv");
    exit(1);
  } else if (pid > 0) { // Processo pai
    int status;
    waitpid(pid, &status, 0);
  } else {
    perror("fork");
  }
}

// Limpa e processa a entrada do usuário
void clean_input(char *input, int buffer_size) {
  if (fgets(input, buffer_size, stdin) != NULL) {
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
    }
  }
}

// Implementação do comando "echo"
void echo(char *str) {
  printf("%s\n", str + 5); // Pula a palavra "echo " para imprimir o restante
}

int main() {
  int exit_REPL = 1;

  while (exit_REPL) {
    printf("$ ");
    fflush(stdout);

    char input[100];
    clean_input(input, 100);

    if (strcmp(input, "exit 0") == 0) {
      exit_REPL = 0;
    } else if (strncmp(input, "echo", 4) == 0) {
      echo(input);
    } else if (strncmp(input, "type", 4) == 0) {
      char *cmd = input + 5; // Comando após "type "
      if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "echo") == 0 ||
          strcmp(cmd, "type") == 0) {
        printf("%s is a shell builtin\n", cmd);
      } else {
        char *path = find_in_path(cmd);
        if (path)
          printf("%s is %s\n", cmd, path);
        else
          printf("%s: not found\n", cmd);
      }
    } else {
      // Processar comando externo com argumentos
      char *argv[10];
      int argc = 0;

      char *token = strtok(input, " ");
      while (token && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
      }
      argv[argc] = NULL;

      char *cmd_path = find_in_path(argv[0]);
      if (cmd_path) {
        fork_and_exec_cmd(cmd_path, argc, argv);
      } else {
        printf("%s: command not found\n", argv[0]);
      }
    }

    fflush(stdout);
  }

  return 0;
}
