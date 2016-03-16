
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
  printf("Аргументы:\n");
  
  curOpt = getopt_long( argc, argv, "", opts, &index );
  while (curOpt != -1) {
    if (opts[index].val == 1) {
      mode = optarg;
      printf("Режим - %s\n", mode );
    } else if (opts[index].val == 2) {
      posixAmount = atoi(optarg);
      printf("Количество сигналов - %d\n", posixAmount );
    } else if (opts[index].val == 3) {
      killSignal = atoi(optarg);
      printf("Номер сигнала - %i\n", killSignal );
    } else if (opts[index].val == 4) {
      killPid = atoi(optarg);
      printf("PID - %i\n", killPid );
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
      printf("Количество сигналов должно быть больше 0\n");
      exit( EXIT_FAILURE );
    }
  } else if ( strcmp( mode, "kill" ) == 0 ) {
    if (!killPid) {
      fprintf(stderr, "Укажите PID, с помощью --pid=value\n");
      exit( EXIT_FAILURE );
    }
    
    if (!killSignal) {
      fprintf(stderr, "Укажите сигнал, с помощью --signal=value\n");
      exit( EXIT_FAILURE );
    }
    
    launchKill(killSignal, killPid);
  } else if ( strcmp( mode, "pipe" ) == 0 ) {
    launchPipe();
  } else {
    fprintf(stderr, "Режим работы должен соответствовать одному из указаных:\nstd, child, posix, kill, pipe\n");
    exit( EXIT_FAILURE );
  }
  return 0;
}
