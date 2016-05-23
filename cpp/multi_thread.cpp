/*
multi-thread test
*/

#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

volatile int counter(0);

mutex m;

void foo() {

  for (int i = 0; i < 10; i++) {
    m.try_lock();
    cout << "thread id: " << this_thread::get_id() << endl;
    counter++;
    m.unlock();
  }
}

int main() {
  thread threads[10];

  for (int i = 0; i < 10; i++) {
    threads[i] = thread(foo);
  }
  for (auto &th : threads)
    th.join();

  cout << "Counter: " << counter << endl;

  return 0;
}
