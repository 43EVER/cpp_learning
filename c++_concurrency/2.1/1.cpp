#include <iostream>
#include <thread>
#include <exception>

class background_task {
public:
  background_task() = default;

  background_task(const background_task&) {
    std::cout << this << "(background_task): copy constructor" << std::endl;
  }

  void operator() () const {
    std::cout << "doing something" << std::endl;
  }
};

int main()
{
  // std::thread th{background_task()};
  // std::thread th2([]{
  //   std::cout << "lambda" << std::endl;
  // });
  // th.join();
  // th2.join();

  // 异常
  int i = 0;
  try {
    std::thread t([&]() {
      while (i < 1e9)
        i++;
        if (i == 5e8)
          throw std::runtime_error("114514 error");
    });
  } catch (std::runtime_error ex) {
    // t.join();
    std::cout << ex.what() << std::endl;
    throw ex;
  }
  std::cout << "complete" << std::endl;
  // t.join();
}