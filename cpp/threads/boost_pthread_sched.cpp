#include <iostream>
#include <boost/thread.hpp>
#include <sched.h>
#include <unistd.h>
//#include <cstdio.h>
using namespace std;
#define HAVE_SCHED_GETSCHEDULER 1
int main(int argc, char const *argv[])
{
  int policy, res;
  struct sched_param param;

  if ((policy = sched_getscheduler(getpid()) == -1 )) {
    perror("sched_getscheduler");
    exit(EXIT_FAILURE);
  }
  if ((res = sched_getparam(getpid(), &param)) == -1 ) {
    perror("sched_getparam");
    exit(EXIT_FAILURE);
  }

  cout << "First:" << endl;
  cout << "policy = " << policy << endl;

  return 0;
}
