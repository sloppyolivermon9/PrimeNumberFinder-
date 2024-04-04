// PrimeNumberFinder.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <Windows.h>
#include <cmath>
using namespace std;

//Global Variables
list<int> primesFound;
int currentNum;

bool isPrime(int num) {
    if (num < 2) {
        return false; // Numbers less than 2 are not prime
    }

    // Check divisibility from 2 up to the square root of num
    for (int i = 2; i <= sqrt(num); ++i) {
        if (num % i == 0) {
            return false; // Found a factor, not prime
        }
    }

    return true; // No factors found, prime
}

/* OLD FUNCTION TO FIND PRIMES
bool isPrime(int num) { //determines if num is prime
    for (int i = 2; i < num; i++) {
        if ((num % i) == 0) {
            return false;
        }
    }
    return true;
}
*/

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

void storeNewPrimes() { //stores primes found into file, if it doesn't exist it creates a new file
    const string file = "primesFound.txt";
    ofstream f(file, ios_base::app);

    for (const int& prime : primesFound) {
        f << to_string(prime) << "\n";
    }
    f.flush();
    f.close();
    primesFound.clear();
}

void storeLatestNum(string num) { //stores number to work on when program is launched next
    const string file = "currentNum.txt";
    ofstream f(file);

    f << num;
    f.flush();
    f.close();
}

void findPrimes() { //function to pull together main parts of program
    currentNum = getLastNum();
    while (true) {
        string numToStore = to_string(currentNum);
        if (isPrime(currentNum)) {
            cout << currentNum << " is a prime number!\n";
            primesFound.push_back(currentNum);
        }
        if (primesFound.size() == 100000) {
            storeNewPrimes();
        }
        currentNum++;
    }
}

void saveWork() { //called to save stuff to files
    storeLatestNum(to_string(currentNum));
    storeNewPrimes();
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
    findPrimes();
    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
