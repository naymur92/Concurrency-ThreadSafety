#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
using namespace std;

class Singleton {
   private:
    static Singleton* instance;
    static mutex mtx;
    Singleton() {}  // private constructor

   public:
    static Singleton* getInstance() {
        if (instance == nullptr) {  // first check (not locked)
            std::lock_guard<std::mutex> lock(mtx);
            if (instance == nullptr)  // double-check
                instance = new Singleton();
        }
        return instance;
    }

    void show() {
        cout << "Singleton Instance at " << this << " (Thread " << this_thread::get_id() << ")\n";
    }
};

Singleton* Singleton::instance = nullptr;
std::mutex Singleton::mtx;

void threadFunc() {
    Singleton::getInstance()->show();
}

int main() {
    const int numThreads = 10;
    vector<thread> threads;

    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(threadFunc);
    }

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}