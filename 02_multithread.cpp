#include <iostream>
#include <thread>
#include <vector>
using namespace std;

// Function to be executed by the thread
void threadFunction(int threadNum)
{
    cout << "Hello from thread " << threadNum << "!" << "\n";
    cout << "Thread ID: " << this_thread::get_id() << "\n";
}

int main()
{
    cout << "Main thread ID: " << this_thread::get_id() << "\n";

    const int numThreads = 4; // Number of threads to launch
    vector<thread> threads;

    // Launch multiple threads
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(threadFunction, i + 1);
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    cout << "All threads have finished execution." << "\n";
    return 0;
}