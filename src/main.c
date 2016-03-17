
#include <kd_1.h>
#include <getopt.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static const struct option opts[] = {
  {"mode", required_argument, NULL, 1},
  {"amount", required_argument, NULL, 2},
  {"signal", required_argument, NULL, 3},
  {"pid", required_argument, NULL, 4}
};

char *mode;
int posixAmount;
int killSignal;
pid_t killPid;

int main(int argc, char *argv[]){
  int curOpt = 0;
  int index = 0;
  printf("-------=======Kd_1=======-------\n");
  printf("Pid - %i\n", getpid());
  printf("Arguments:\n");
  
  curOpt = getopt_long( argc, argv, "", opts, &index );
  while (curOpt != -1) {
    if (opts[index].val == 1) {
      mode = optarg;
      printf("Mode - %s\n", mode );
    } else if (opts[index].val == 2) {
      posixAmount = atoi(optarg);
      printf("Signals amount - %d\n", posixAmount );
    } else if (opts[index].val == 3) {
      killSignal = atoi(optarg);
      printf("Signal number - %i\n", killSignal );
    } else if (opts[index].val == 4) {
      killPid = atoi(optarg);
      printf("PID for kill - %i\n", killPid );
    }
    curOpt = getopt_long( argc, argv, "", opts, &index);
  }
  printf("------======Launching======------\n");
  
  if ( strcmp( mode, "std" ) == 0){
    launchStd();
  } else if ( strcmp( mode, "child" ) == 0 ) {
    launchChild();
  } else if ( strcmp( mode, "posix" ) == 0 ) {
    if (posixAmount > 0) {
      launchPosix(posixAmount);
    } else {
      printf("Signals amount must be greater 0\n");
      exit( EXIT_FAILURE );
    }
  } else if ( strcmp( mode, "kill" ) == 0 ) {
    if (!killPid) {
      fprintf(stderr, "Need for PID, use --pid=value\n");
      exit( EXIT_FAILURE );
    }
    if (killSignal <=0 ) {
      fprintf(stderr, "Need for signal, use --signal=value\n");
      exit( EXIT_FAILURE );
    }
    launchKill(killSignal, killPid);
  } else if ( strcmp( mode, "pipe" ) == 0 ) {
    launchPipe();
  } else {
    fprintf(stderr, "Mode must be one of these:\nstd, child, posix, kill, pipe\n");
    exit( EXIT_FAILURE );
  }
  return 0;
}
