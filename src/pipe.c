
#include "kd_1.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>

int receivedPipeSignal = 0;

void pipeHandler(int signo, siginfo_t *siginfo, void *context){
  receivedPipeSignal = 1;
}

void launchPipe() {
  struct sigaction sa;
  sa.sa_sigaction = &pipeHandler;
  sa.sa_flags = SA_SIGINFO;
  sigset_t mask;
  
  sigemptyset(&mask);
  sigaddset(&mask, SIGPIPE);
  sa.sa_mask = mask;
  if (sigaction(SIGPIPE, &sa, NULL) == -1) {
    perror("Ошибка: не удается обработать сигнал SIGCHLD");
  }
  
  while (!receivedPipeSignal) {
    sleep(10);
  }
  if (receivedPipeSignal) {
    int fd[2];
    if (pipe(fd) == 0) {
      int childPid = fork();
      if (childPid == 0) {
        if (close(fd[0]) == -1) {
          fprintf(stderr, "Can't close descriptor, errno - %d",errno);
        }
      } else if (childPid == -1){
        fprintf(stderr, "Can't fork, errno - %d",errno);
      } else {
        int returnStatus;
        waitpid(childPid, &returnStatus, 0);
        printf("---child process ended---\n");
      }
    } else {
      fprintf(stderr, "Can't open pipe, errno - %d",errno);
    }
  }
}