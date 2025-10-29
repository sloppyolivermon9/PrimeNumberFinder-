#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <cmath>
#include <thread>
#include <mutex>

using std::cout;
using std::cin;
using std::string;
using std::endl;

using std::ofstream;
using std::ifstream;
using std::ios_base;
using std::to_string;

using std::thread;
using std::mutex;

//Global Variables
unsigned long long int primePool[20000];
unsigned short int primePoolNumber; //What index to add prime number?

unsigned long long int nextNum;

thread primeCalculators[65535];
bool killThreads = false;
unsigned short int threadNum;

//Ensures threads don't edit variables at same time
mutex mtx;

bool isPrime(unsigned long long int num) {
    for (unsigned long long int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return false; // Found a factor, not prime
        }
    }

    return true;
}

void findPrime() {
    //cout << endl << "Prepping to lock!" << endl;
    mtx.lock();
    //cout << "Locked" << endl;
    unsigned long long int numToWorkOn = nextNum;
    nextNum++;
    //cout << "Prepping to unlock!" << endl;
    mtx.unlock();

    while (!killThreads) {
        if (isPrime(numToWorkOn)) {
            mtx.lock();
            /*if (primePoolNumber > 15000) {
                cout << "Out of range!" << endl;
            }
            else {
                cout << primePoolNumber << endl;
            }*/
            primePool[primePoolNumber] = numToWorkOn;
            cout << primePool[primePoolNumber] << " is a prime number!" << endl;
            primePoolNumber++;
            mtx.unlock();
        }

        mtx.lock();
        numToWorkOn = nextNum;
        nextNum++;
        mtx.unlock();
    }
}

void storeLatestNum(string num) { //stores number to work on when program is launched next
    const string file = "currentNum.txt";
    ofstream f(file);

    f << num;
    f.flush();
    f.close();
}

void storeNewPrimes() { //stores primes found into file, if it doesn't exist it creates a new file
    const string file = "primesFound.txt";
    ofstream f(file, ios_base::app);

    for (unsigned long int prime = 0; prime < 20000; prime++) {
        f << to_string(primePool[prime]) << "\n";
    }
    f.flush();
    f.close();
    primePoolNumber = 0;
}

void saveWork() {
    killThreads = true;
    for (unsigned short int i = 0; i < threadNum; i++) {
        primeCalculators[i].join();
    }

    mtx.lock();
    storeLatestNum(to_string(nextNum));
    storeNewPrimes();
    mtx.unlock();
}

void saveWorkThread() {
    while (true) {
        if (primePoolNumber > 10000) {
            mtx.lock();
            storeLatestNum(to_string(nextNum));
            storeNewPrimes();
            mtx.unlock();
        }
    }
}

int getLastNum() { //gets number from .txt so it can resume progress
    const string file = "currentNum.txt";
    string output = "";
    ifstream f(file);

    if (!f.is_open()) {
        ofstream f(file);
        f << "2";
        f.close();
        ifstream f2(file);
        getline(f2, output);
        f2.close();
        return stoi(output);
    }

    getline(f, output);
    f.close();
    if (output == "") {
        return 2;
    }
    return stoi(output);
}

//saves work when program is closed
BOOL CtrlHandler(DWORD ctrlType) {
    switch (ctrlType) {
    case CTRL_C_EVENT:
        saveWork();
        break;
    case CTRL_BREAK_EVENT:
        saveWork();
        break;
    case CTRL_CLOSE_EVENT:
        saveWork();
        break;
    }
    return TRUE;
}

int main()
{
    SetConsoleCtrlHandler((PHANDLER_ROUTINE)CtrlHandler, TRUE); //sets up event for when program is closed
    
    nextNum = getLastNum();

    cout << "How many threads would you like to work on this?: ";
    cin >> threadNum;

    thread saveThread(saveWorkThread);

    for (unsigned short int i = 0; i < threadNum; i++) {
        cout << "thread created!" << endl;
        primeCalculators[i] = thread(findPrime);
    }

    return 0;
}
