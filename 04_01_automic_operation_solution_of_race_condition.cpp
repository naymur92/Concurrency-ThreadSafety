#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
using namespace std;

atomic<int> sharedCounter(0);

void incrementCounter(int numIncrements)
{
    for (int i = 0; i < numIncrements; ++i)
    {
        ++sharedCounter;
    }
}

int main()
{
    cout << "Main thread ID: " << this_thread::get_id() << "\n";

    const int numThreads = 4;
    const int incrementsPerThread = 100000;
    vector<thread> threads;

    // Launch multiple threads to increment the shared counter
    for (int i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(incrementCounter, incrementsPerThread);
    }

    // Wait for all threads to finish
    for (auto &t : threads)
    {
        t.join();
    }

    cout << "Expected counter value: " << numThreads * incrementsPerThread << "\n";
    cout << "Actual counter value:   " << sharedCounter << "\n";
    return 0;
}