#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sysctl.h>

#define hostnameSZ 100
FILE *fp = NULL;

typedef struct thread_info_ {
  int tid;
  char thr_name[20];
} thread_info_t;

void *my_thread_func(void *arg)
{
  int rc;
  thread_info_t *p_thread_info = (thread_info_t *)arg;

  fprintf(fp, "Thread Id: %d name: %s \n",
  p_thread_info->tid, p_thread_info->thr_name);

  char hostname[hostnameSZ];
  size_t hostname_l = sizeof(hostname);

  //int
  //sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp,
  //       void *newp, size_t newlen);
  //sysctlbyname("net.inet.tcp.stats", &tcpstat, &stat_len, NULL, 0);
  if (p_thread_info->tid == 1) {
    rc = sysctlbyname("kern.hostname", hostname, &hostname_l, NULL, 0);
    if (rc == -1) {
      perror("sysctlbyname");
      exit(1);
    }

    fprintf(fp, "old hostname: %s \n", hostname);
  } else if (p_thread_info->tid == 2) {
    strncpy(hostname, "test-name", sizeof(hostname));
    rc = sysctlbyname("kern.hostname", NULL, 0, hostname, hostname_l);
    if (rc == -1) {
      perror("sysctlbyname");
      exit(1);
    }
  }

  rc = sysctlbyname("kern.hostname", hostname, &hostname_l, NULL, 0);
  if (rc == -1) {
    perror("sysctlbyname");
    exit(1);
  }

  fprintf(fp, "old hostname: %s \n", hostname);

  sleep(1);

  pthread_exit(NULL);
}

int main()
{
  int rc;
  void *status;
  pthread_t mythread1, mythread2;
  pthread_attr_t attr;

  //fopen("const char *restrict", const char *restrict)
  fp = fopen("sysctl_thread.txt", "w");
  if (fp == NULL) {
    perror("fille error");
    exit(1);
  }

  pthread_attr_init(&attr);

  //pthread_create(pthread_t *restrict, const pthread_attr_t *restrict,
  //               void *(*)(void *), void *restrict)
  thread_info_t thread_info;

  thread_info.tid = 1;
  strncpy(thread_info.thr_name, "thread-1", sizeof(thread_info.thr_name));
  rc = pthread_create(&mythread1, &attr, my_thread_func, (void *)&thread_info);
  if (rc) {
    perror("pthread1_create error");
    exit(1);
  }

  fprintf(fp, "Main Thread: waiting on thread1 \n");
  pthread_join(mythread1, &status);
  fprintf(fp, "Main Thread: thread1 done \n\n\n");

  pthread_attr_init(&attr);
  thread_info.tid = 2;
  strncpy(thread_info.thr_name, "thread-2", sizeof(thread_info.thr_name));
  rc = pthread_create(&mythread2, &attr, my_thread_func, (void *)&thread_info);
  if (rc) {
    perror("pthread2_create error");
    exit(1);
  }

  // Join this thread
  fprintf(fp, "Main Thread: waiting on thread2 \n");
  pthread_join(mythread2, &status);
  fprintf(fp, "Main Thread: thread2 done \n\n\n");
  pthread_exit(NULL);

  fclose(fp);

  return 0;
}
