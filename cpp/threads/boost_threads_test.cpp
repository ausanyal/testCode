#include <iostream>
#include <pthread.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/date_time.hpp>

using namespace std;
#define NUM_THREADS 10

typedef struct thread_data_ {
    int id;
    string message;
} thread_data_t;

class Worker {
public:
  Worker(unsigned n)
  {
      n_ = n;
  }

  void processQ(unsigned m) {

    float ms = m * n_ * 1e3;

    boost::posix_time::milliseconds workTime(ms);
    boost::this_thread::sleep(workTime);

    cout << "Worker: " << boost::this_thread::get_id() 
         << " : Done sleeping for (ms): " << ms << endl;

    //pthread_exit(NULL);
    return;

  }

private:
  unsigned n_;
};

void *thread_fn(void *thread_arg)
{
  thread_data_t *td = (thread_data_t *)thread_arg;

  cout << "\t\t " << td->id;
  sleep(20 - td->id);
  //cout << "In thread: " << td->id << " Text: " << td->message << endl;

  //pthread_exit(NULL);
  return NULL;
}

void boostTestFn(int n)
{
  boost::posix_time::seconds testSleep(2);

  boost::this_thread::sleep(testSleep);
  cout << __func__ << ": n = " << n << endl;
  pthread_exit(NULL);
  return;
}

int main(int argc, char const *argv[])
{
  int rc;
  void *status;

  // pthread
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  thread_data_t td[NUM_THREADS];

  /****************** bost *********************/
  Worker w(3);
  boost::thread boostThread1(&Worker::processQ, &w, 2);
  boost::thread boostThread2(boostTestFn, 2);

#if 0
  boost::thread::attributes attrs;
  attrs.set_stack_size(2048*10);
  boost::thread boostThread3(attrs, boostTestFn, 2);
#endif

  cout << "Main: Waiting for initial boost threads" << endl;
  // join intial boost threads
  boostThread1.join();
  cout << "Thread1 done" << endl;
  boostThread2.join();
  cout << "Main: Done waiting for initial boost threads" << endl;

  std::vector<boost::thread *> boostThreadPool;

  for (int i = 0; i < NUM_THREADS; i++) {
    td[i].id = i;
    td[i].message = "boost-thread";
    boostThreadPool.push_back(new boost::thread(thread_fn, &td));
  }

  for (int i = 0; i < NUM_THREADS; i++) {
    cout << "Main: Waiting for boost thread: " << i << endl;
    boostThreadPool[i]->join();
    //delete(boostThreadPool[NUM_THREADS - i - 1]);
  }
  cout << "Main: Done waiting for boost threads" << endl;

  /****************** pthread ******************/
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (int i = 0; i < NUM_THREADS; i++) {

    td[i].id = i;
    td[i].message = "pthread";
    //pthread_create(pthread_t *, const pthread_attr_t *,
    //               void *(*)(void *), void *);
    rc = pthread_create(&threads[i], &attr, thread_fn, (void *)&td[i]);
    if (rc) {
      cout << "Error creating thread: " << i << endl;
      exit(1);
    }
  }

  // Join pthreads
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
