#include <iostream>
#include <thread>
using namespace std;

// Function to be executed by the thread
void threadFunction()
{
    cout << "Hello from thread!" << endl;
    cout << "Thread ID: " << this_thread::get_id() << endl;
}

int main()
{
    cout << "Main thread ID: " << this_thread::get_id() << endl;

    int n = thread::hardware_concurrency();
    cout << "Number of concurrent threads supported: " << n << endl;

    // Create a thread that runs threadFunction
    thread t(threadFunction);

    // Wait for the thread to finish
    t.join();

    cout << "Thread has finished execution." << endl;
    return 0;
}