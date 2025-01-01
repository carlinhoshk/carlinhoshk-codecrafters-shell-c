#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int is_executable(const char *path) { return access(path, X_OK) == 0; }
char *find_in_path(const char *command) {
  char *path_env = getenv("PATH");
  if (path_env == NULL)
    return NULL;
  char *path_copy = strdup(path_env);
  char *dir = strtok(path_copy, ":");
  static char full_path[1024];
  while (dir != NULL) {
    snprintf(full_path, sizeof(full_path), "%s/%s", dir, command);
    if (is_executable(full_path)) {
      free(path_copy);
      return full_path;
    }
    dir = strtok(NULL, ":");
  }
  free(path_copy);
  return NULL;
}
void fork_and_exec_cmd(char *full_path, int argc, char **argv) {
  pid_t pid = fork();
  if (pid == 0) {
    execv(full_path, argv);
    perror("execv");
    exit(1);
  } else if (pid < 0)
    perror("fork");
  else {
    int status;
    waitpid(pid, &status, 0);
  }
}
void clean_input(char *input, int buffer_size) {
  if (fgets(input, buffer_size, stdin) != NULL) {
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
    }
  }
}
void echo(char *str1) { printf("%s\n", str1 + 5); }
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
      fflush(stdout);
      continue;
    } else if (strncmp(input, "type", 4) == 0) {
      if (strncmp(&input[5], "exit", 4) == 0 ||
          strncmp(&input[5], "echo", 4) == 0 ||
          strncmp(&input[5], "type", 4) == 0) {
        printf("%.4s is a shell builtin\n", &input[5]);
      } else {
        char *cmd = input + 5;
        char *path = find_in_path(cmd);
        if (path)
          printf("%s is %s\n", cmd, path);
        else
          printf("%s: not found\n", cmd);
      }
      fflush(stdout);
    } else {
      char *argv[10];
      int argc = 0;
      char *token = strtok(input, " ");
      while (token != NULL && argc < 10) {
        argv[argc++] = token;
        token = strtok(NULL, " ");
      }
      argv[argc] = NULL;
      char *cmd_path = find_in_path(argv[0]);
      if (cmd_path) {
        fork_and_exec_cmd(cmd_path, argc, argv);
      } else
        printf("%s: command not found\n", argv[0]);
      fflush(stdout);
    }
  }
  return 0;
}
