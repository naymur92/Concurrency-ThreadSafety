#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

using namespace std;

queue<int> dataQueue;
mutex mtx;
condition_variable cv;

bool done = false;

void producer() {
    for (int i = 0; i < 10; ++i) {
        {
            lock_guard<mutex> lock(mtx);
            dataQueue.push(i);
            cout << "Produced: " << i << endl;
        }
        cv.notify_one();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
    {
        lock_guard<mutex> lock(mtx);
        done = true;
    }
    cv.notify_all();
}

void consumer() {
    while (true) {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [] { return !dataQueue.empty() || done; });

        while (!dataQueue.empty()) {
            int value = dataQueue.front();
            dataQueue.pop();
            cout << "Consumed: " << value << endl;
        }

        if (done && dataQueue.empty()) break;
    }
}

int main() {
    thread prod(producer);
    thread cons(consumer);

    prod.join();
    cons.join();

    return 0;
}