#include <iostream>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

std::atomic_bool ready(false);
std::mutex g_m;

void count(const uint32_t id)
{
  while (!ready) { // wait until main() sets ready...
    //若线程还有没创建的，将当前线程分配的cpu时间片，让调度器安排给其他线程，
    //由于使用了yield函数，在 not Ready 情况下，避免了空循环，在一定程度上，可以提高cpu的利用率
    std::this_thread::yield();
  }
  
  for (uint32_t i = 0; i < 1000; i++) {}

  std::lock_guard<std::mutex> l(g_m);
  cout << "id:" << id  << endl;
}

int main()
{
  std::thread t[10];

  for (uint32_t i = 0; i < 10; i++) {
    t[i] = std::thread(count, i);
  }

  ready = true;

  for (uint32_t i = 0; i < 10; i++) {
    t[i].join();
  }

  return 0;
}