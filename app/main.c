#include <stdio.h>
#include <string.h>

int main() {
  char input[100];
  fgets(input, 100, stdin);
  // Flush after every printf
  setbuf(stdout, NULL);

  // Uncomment this block to pass the first stage
  printf("$ ");

  input[strlen(input) - 1] = '\0';
  printf("%s: command not found\n", input);
  // Wait for user input
  // fgets(input, 100, stdin);
  return 0;
}
