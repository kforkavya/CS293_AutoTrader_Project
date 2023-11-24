// This is your trader. Place your orders from here


#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <atomic>
#include <mutex>
#include <unistd.h>
#include <fstream>
using namespace std;

ifstream inputFile("output.txt");
extern mutex printMutex;
mutex fileMutex;
extern void* userTrader(void* arg);
string line2 = "";

std::condition_variable dataReady; // Condition variable for signaling new data
bool newDataAvailable = false; // Flag to indicate new data
std::streampos filePosition;

void writeToOutput(const std::string& message) {
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << message << std::endl;
    printMutex.unlock();
}

int reader(int time) {
    if (!inputFile.is_open()) {
        writeToOutput("Error opening file for reading!");
        return 1; // Indicate failure
    }

    std::string line;

    while (true) {
        {
            std::unique_lock<std::mutex> lock(fileMutex);
            inputFile.seekg(0, std::ios::end);
            std::streampos endPosition = inputFile.tellg();

            if (endPosition > filePosition) {
                inputFile.seekg(filePosition);
                while (std::getline(inputFile, line)) {
                    if (line.find("!@") != std::string::npos) {
                        filePosition = inputFile.tellg();
                        newDataAvailable = true;
                        dataReady.notify_one();
                        writeToOutput("Read new data from file");
                        break;
                    }
                }
            }
        }

        // If the end of the file is reached, wait for new data
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int trader(std::string *message)
{
    return 1;
}
