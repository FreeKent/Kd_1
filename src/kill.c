
#include "kd_1.h"
#include <stdio.h>
#include <signal.h>

void launchKill(int signal, int pid) {
  kill(pid, signal);
}