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

    while (true) 
    {
        if(!getline(inputFile, line))
        {
            cerr << "Not reading a line" << endl;
            inputFile.close();
            inputFile.open("output.txt");
            inputFile.seekg(filePosition);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return 0;
        }
        filePosition = inputFile.tellg();
        if(line == "!@")
        {
            cerr << "Input Reading Complete." << endl;
            return 1;
        }
        if(line == "TL")
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }
        int spaceIndex = 0;
        while(spaceIndex < line.size())
        {
            if(line[spaceIndex++] == ' ')
                break;
        }
        if(spaceIndex != line.size() && line.substr(spaceIndex, 7) == "22B1053")
            continue;
        line2 = "22B1053 " + line;
        userTrader(NULL);
        // If the end of the file is reached, wait for new data
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int trader(std::string *message)
{
    *message = line2;
    return 1;
}
