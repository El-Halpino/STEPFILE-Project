// STEPFILE-Project.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <chrono>
#include "STEP.h"
using namespace std;

int main()
{
    STEP stepDataObj;
    auto start = chrono::steady_clock::now();// Start Clock
    stepDataObj.stepController();
    auto end = chrono::steady_clock::now();// End Clock
    std::cout << "\nTime Taken: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n\n";
    return 0;
}
