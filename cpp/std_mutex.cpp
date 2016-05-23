// thread example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>
#include <chrono>

using namespace std;
typedef void (*fn_ptr)(int x, char c);

static inline void print_thread_id()
{
  std::cout << std::endl << std::this_thread::get_id() << endl;
  return;
}

class thr_lib {
private:
  std::mutex mtx;
public:
  thr_lib() {
    cout << "Object contructed\n";
  }
  ~thr_lib() {
    cout << "Object destructed\n";
  }

  std::thread crt_thr(fn_ptr p_fn, int x, char c);

  //void foo(int x, char c);
  //void bar(int x, char c);
  //void foobar(int x, char c);
};

std::thread
thr_lib::crt_thr(fn_ptr p_fn, int x, char c)
{

  return;
}

// try_lock
void
bar(int x, char c)
{
  if (mtx.try_lock()) {

    for (int i = 0; i < x; i++) {
      std::cout << c << endl;
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    print_thread_id();
    cout << __func__ << ": Releasing lock \n";
    mtx.unlock();

  } else {
    //cout << "Didnt get lock to print " << c << std::endl;
  }
}

//lock
void
foo(int x, char c)
{

  mtx.lock();
  for (int i = 0; i < x; i++) {
    std::cout << c << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  print_thread_id();
  cout << __func__ << ": Releasing lock \n";
  mtx.unlock();

  return;
}


//lock_guard
void
foobar(int x, char c)
{
  std::lock_guard<std::mutex> lock(mtx);
  for (int i = 0; i < x; i++) {

    std::cout << c << endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  print_thread_id();
  cout << __func__ << ": Releasing lock \n";
  return;
}

#define NUM_THREADS_PER_FN 2

int main()
{
  thr_lib mythr;
  std::thread *arr_thr = NULL;
  std::thread t3(bar, 2, '#');     // spawn new thread that calls foo()


  std::thread t5(foobar, 2, '%');     // spawn new thread that calls foo()

  std::thread t4(bar, 2, '$');  // spawn new thread that calls bar(0)

  std::thread t6(foobar, 2, '^');  // spawn new thread that calls bar(0)

  std::thread t1(foo, 2, '!');     // spawn new thread that calls foo()
  std::thread t2(foo, 2, '@');  // spawn new thread that calls bar(0)

  // synchronize threads:
  t1.join();
  t2.join();
  t3.join();
  t4.join();
  t5.join();
  t6.join();


  if (arr_thr) {
    free(arr_thr);
  }
  std::cout << "\ncompleted.\n";

  return 0;
}



#if 0
fn_ptr fn_ptr_arr[] = {
  foo,
  bar,
  foobar
};
  int num_fns = sizeof(fn_ptr_arr)/sizeof(fn_ptr_arr[0]);
  int id;

  for (int i = 1, id = 0; i <= num_fns; i++) {
    for (int j = 1; j <= NUM_THREADS_PER_FN; j++, id++) {

      if ((i == 0) && (j == 0)) {
        arr_thr = (std::thread *)malloc(NUM_THREADS_PER_FN * num_fns *
                                        sizeof(std::thread));
      }
      cout << "Generating thread: " << i << " " << j << " " << id << endl;
      cout << "addr: " << fn_ptr_arr[id] << endl;
      //fn_ptr_arr[id](50, '$');
      arr_thr[id] = std::thread(fn_ptr_arr[id], 50, '$');

    }
  }

  for (int j = 0; j < NUM_THREADS_PER_FN * num_fns; j++) {
    //arr_thr[j].join();
  }

#endif
