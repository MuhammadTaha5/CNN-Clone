#include <iostream>
#include <cstdlib>      // for rand(), srand()
#include <ctime>        // for time()
#include <unistd.h>     // for fork(), getpid(), getppid()
#include <sys/wait.h>   // for waitpid()

using namespace std;

bool isPrime(int n)
{
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i)
    {
        if (n % i == 0)
            return false;
    }
    return true;
}

int main()
{
    int arr[1000];

    // Seed and fill array with random numbers between 1 and 100
    srand(time(NULL));
    for (int i = 0; i < 1000; i++)
    {
        arr[i] = rand() % 100 + 1;
    }

    pid_t a = fork();

    if (a < 0)
    {
        cerr << "Fork failed!" << endl;
        return 1;
    }

    // First child process
    if (a == 0)
    {
        int totalPrime = 0;
        cout << "First Child Executing first 500 indexes" << endl;

        for (int i = 0; i < 500; i++)
        {
            if (isPrime(arr[i]))
                totalPrime++;
        }

        cout << "Total Prime Numbers in First Child Process (PID: "
             << getpid() << ", PPID: " << getppid()
             << ") = " << totalPrime << endl;

        return 0; // Child exits here
    }

    // Parent creates second child
    pid_t b = fork();

    if (b < 0)
    {
        cerr << "Second Fork failed!" << endl;
        return 1;
    }

    // Second child process
    if (b == 0)
    {
        int totalPrime = 0;
        cout << "Second Child Executing last 500 indexes" << endl;

        for (int i = 500; i < 1000; i++)
        {
            if (isPrime(arr[i]))
                totalPrime++;
        }

        cout << "Total Prime Numbers in Second Child Process (PID: "
             << getpid() << ", PPID: " << getppid()
             << ") = " << totalPrime << endl;

        return 0; // Child exits here
    }

    // Parent waits for both children
    waitpid(a, NULL, 0);
    waitpid(b, NULL, 0);

    cout << "Parent Process (PID: " << getpid() << ") finished waiting for children." << endl;

    return 0;
}
