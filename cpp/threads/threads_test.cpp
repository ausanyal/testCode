#include <iostream>
#include <pthread.h>
#include <string>

using namespace std;

#define NUM_THREADS 10

typedef struct thread_data_ {
    int id;
    string message;
} thread_data_t;


void *thread_fn(void *thread_arg)
{
  thread_data_t *td = (thread_data_t *)thread_arg;

  sleep(10);
  cout << "In thread: " << td->id << " Text: " << td->message << endl;

  pthread_exit(NULL);
}


int main(int argc, char const *argv[])
{

  int rc;
  void *status;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  thread_data_t td[NUM_THREADS];

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (int i = 0; i < NUM_THREADS; i++) {

    td[i].id = i;
    td[i].message = "hello";
    //pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void *);
    rc = pthread_create(&threads[i], &attr, thread_fn, (void *)&td[i]);
    if (rc) {
      cout << "Error creating thread: " << i << endl;
      exit(1);
    }
  }

  // Join threads
  for(int i = 0; i < NUM_THREADS; i++) {
    //pthread_join(pthread_t, void **)
    //int j = NUM_THREADS - i - 1;
    int j = i;

    pthread_join(threads[j], &status);
    if (rc) {
      cout << "Main: Error joining thread: " << j << endl;
    }
    cout << "Main: completed thread: " << j;
    cout << " Exiting with status: " << status << endl;
  }
  cout << "Main program exiting "<< endl;
  pthread_exit(NULL);

  return 0;
}
