#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

const int num_threads = 100;
mutex mtx;
condition_variable cv;
int current_thread = 0;

void count(int index) {
  int num = 1000000;
  while (num--) {}

  unique_lock<mutex> lock(mtx);

  // 使用循环等待，直到轮到当前线程执行
  while (index != current_thread) {
    cv.wait(lock);
  }

  // 输出信息
  cout << "I'm thread " << index << ", local count: 1000000\n";

  // 更新全局变量，准备让下一个线程执行
  current_thread++;
  if (current_thread >= num_threads) {
    current_thread = 0;
  }

  // 通知下一个线程
  cv.notify_all();
}

int main(void) {
  thread t[num_threads];

  for (int i = 0; i < num_threads; i++)
    t[i] = thread(count, i);

  // 等待所有线程完成
  for (int i = 0; i < num_threads; i++)
    t[i].join();
  
  return 0;
}
