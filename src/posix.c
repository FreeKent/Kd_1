
#include "kd_1.h"
#include <stdio.h>
#include <signal.h>
#include <errno.h>

int infosCount = 0;
siginfo_t *receivedInfos;

void posixHandler(int signo, siginfo_t *siginfo, void *context){
  if (signo >= SIGRTMIN && signo < SIGRTMAX) {
    receivedInfos[infosCount++] = *siginfo;
  } else {
    errSignal = signo;
  }
}

void launchPosix(int amount) {
  receivedInfos = (siginfo_t*)malloc(amount*sizeof(siginfo_t));
  
  struct sigaction sa;
  sa.sa_sigaction = &posixHandler;
  sa.sa_flags = SA_SIGINFO;
  sigset_t mask;
  
  sigfillset(&mask);
  //sigaddset(&mask, SIGPIPE);
  sa.sa_mask = mask;
  int i;
  for (i=SIGRTMIN; i<SIGRTMAX; i++) {
    if (sigaction(i, &sa, NULL) == -1) {
      fprintf(stderr, "Can't block signal - %d", i);
    }
  }
  sigprocmask(SIG_BLOCK, &mask, NULL);
  int parentPid = getpgid();
  int childPid = fork();

  if (childPid == -1) {
    fprintf(stderr, "Can't fork, errno - %d", errno);
  } else if (childPid == 0){
    printf("Sent:\n");
    for (i=0; i<amount; i++) {
      int signo = rand()%(SIGRTMAX-SIGRTMIN) + SIGRTMIN;
      union sigval val;
      val.sival_int = rand();
      sigqueue(parentPid, signo, val);
      printf("%d | %d | %d | %d | %d\n",i,parentPid,getpid(),signo,val.sival_int);
    }
  } else {
    int status;
    waitpid(childPid, &status, 0);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);
    printf("Got:\n");
    for (i=0; i<infosCount; i++){
      siginfo_t *info = &receivedInfos[i];
      printf("%d | %d | %d | %d | %d\n",i,parentPid,info->si_pid,info->si_signo,info->si_value);
    }
  }
}