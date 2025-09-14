#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
using namespace std;

class ThreadPool {
   public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] { this->worker(); });
        }
    }

    ~ThreadPool() {
        {
            lock_guard<mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (thread &worker : workers) worker.join();
    }

    void enqueue(function<void()> task) {
        {
            lock_guard<mutex> lock(queueMutex);
            tasks.push(move(task));
        }
        condition.notify_one();
    }

   private:
    vector<thread> workers;
    queue<function<void()>> tasks;

    mutex queueMutex;
    condition_variable condition;
    bool stop;

    void worker() {
        while (true) {
            function<void()> task;
            {
                unique_lock<mutex> lock(queueMutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty()) return;
                task = move(tasks.front());
                tasks.pop();
            }
            task();
        }
    }
};

int main() {
    ThreadPool pool(4);

    for (int i = 0; i < 8; ++i) {
        pool.enqueue([i] {
            cout << "Task " << i << " is running on thread " << this_thread::get_id() << endl;
            this_thread::sleep_for(chrono::milliseconds(200));
        });
    }

    // Give time for all tasks to finish before main exits
    this_thread::sleep_for(chrono::seconds(2));
    return 0;
}