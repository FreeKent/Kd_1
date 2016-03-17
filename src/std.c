
#include "kd_1.h"
#include <stdio.h>
#include <signal.h>

int errSigno = -1;
int receivedSigno = -1;
siginfo_t *receivedSiginfo = NULL;

void std_Handler(int signo, siginfo_t *siginfo, void *context){
  
  if (signo == SIGUSR1 || signo == SIGUSR2 || signo == SIGHUP) {
    receivedSigno = signo;
    receivedSiginfo = siginfo;
  } else {
    errSigno = signo;
  }
  
  return;
  
}

void launchStd() {
  struct sigaction sa;
  sa.sa_sigaction = &std_Handler;
  sa.sa_flags = SA_SIGINFO;
  sigset_t mask;
  
  sigemptyset(&mask);
  sigaddset(&mask, SIGHUP);
  sigaddset(&mask, SIGUSR1);
  sigaddset(&mask, SIGUSR2);
  sa.sa_mask = mask;
  if (sigaction(SIGHUP, &sa, NULL) == -1) {
    fprintf(stderr, "Can't change action for SIGHUP");
  }
  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    fprintf(stderr, "Can't change action for SIGUSR1");
  }
  if (sigaction(SIGUSR2, &sa, NULL) == -1) {
    fprintf(stderr, "Can't change action for SIGUSR2");
  }
  
  while (1) {
    sleep(10);
    if (errSigno != -1) {
      fprintf(stderr, "Unexpected signal - %d\n", errSigno);
      errSigno = -1;
    } else if (receivedSigno != -1) {
      printf("Got signal %d from PID:%d GID:%d\n",
             receivedSigno,receivedSiginfo->si_pid,getpgid(receivedSiginfo->si_pid));
      receivedSigno = -1;
      receivedSiginfo = NULL;
    }
  }
}
