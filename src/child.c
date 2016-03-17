
#include "kd_1.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>

int receivedChildSignal = 0;
siginfo_t sigchldInfo;
void childHandler(int signo, siginfo_t *siginfo, void *context){
  
  receivedChildSignal = 1;
  sigchldInfo = *siginfo;
}

void launchChild() {
  struct sigaction sa;
  sa.sa_sigaction = &childHandler;
  sa.sa_flags = SA_SIGINFO;
  sigset_t mask;
  
  sigemptyset(&mask);
  sigaddset(&mask, SIGCHLD);
  sa.sa_mask = mask;
  if (sigaction(SIGCHLD, &sa, NULL) == -1) {
    fprintf(stderr, "Can't change action for SIGCHLD");
  }
  
  while (!receivedChildSignal) {
    sleep(10);
  }
  if (receivedChildSignal) {
    pid_t pid = fork();
    int sleepTime = rand()%10;
    if (pid == -1) {
      fprintf(stderr, "Can't fork, errno - %d",errno);
    } else if (pid == 0){
      printf("Sleep %ds\n",sleepTime);
      sleep(sleepTime);
      printf("Child PID: %i\n", sigchldInfo.si_pid);
      printf("User of process: %i\n", sigchldInfo.si_uid);
      printf("Status: %i\n", sigchldInfo.si_status);
      printf("Code: %i\n", sigchldInfo.si_code);
    } else {
      int returnStatus;
      waitpid(pid, &returnStatus, 0);
      printf("---child process ended---\n");
    }
  }
}