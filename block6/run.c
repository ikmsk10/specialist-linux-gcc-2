#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

int main(int argc, char * argv[])
{
   int pid, status;
   if (argc < 2) {
     printf("Usage: %s command, [arg1 [arg2]...]\n", argv[0]);
     return EXIT_FAILURE;
   }
   printf("Starting %s...\n", argv[1]);
   pid = fork();
   if (pid == 0) {
     execvp(argv[1], &argv[1]);
     perror("execl");
     return EXIT_FAILURE; // Never get there normally
   }
   else {
     if (wait(&status) == -1) {
       perror("wait");
       return EXIT_FAILURE;
     }
     if (WIFEXITED(status))
       printf("Child terminated normally with exit code %i\n", WEXITSTATUS(status));
     if (WIFSIGNALED(status))
       printf("Child was terminated by a signal #%i\n", WTERMSIG(status));
     if (WCOREDUMP(status))
       printf("Child dumped core\n");
     if (WIFSTOPPED(status))
       printf("Child was stopped by a signal #%i\n", WSTOPSIG(status));
   }
   return EXIT_SUCCESS;
}
