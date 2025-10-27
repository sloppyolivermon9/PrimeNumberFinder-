
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

    for (int prime = 0; prime < 100000; prime++) {
        f << to_string(prime) << "\n";
    }
    f.flush();
    f.close();
    currentPrimes = 0;
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
            primesFound[currentPrimes] = currentNum;
            currentPrimes++;
        }
        if (currentPrimes == 100000) {
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
