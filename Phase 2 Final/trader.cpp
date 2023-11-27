// This is your trader. Place your orders from here


#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <atomic>
#include <mutex>
#include <unistd.h>
#include <fstream>
#include "my_trader.cpp"
using namespace std;

ifstream inputFile("output.txt");
extern mutex printMutex;
extern std::atomic<int> commonTimer;
string line2 = "";

std::streampos filePosition;

int reader(int time);
int trader(string* message);

void* userThread(void* arg)
{
    int thread_id = *(int*)arg;
    while(true)
    {
        int currentTime;
        {
            currentTime = commonTimer.load();
        }
        int end = reader(currentTime);
        if (end) break;
    }
    return NULL;
}

void* userTrader(void* arg)
{
    //int thread_id = *(int*)arg;
    std::string message;
    int valid = trader(&message);
    int currentTime;
    {
        currentTime = commonTimer.load();
    }
    {
        std::lock_guard<std::mutex> lock(printMutex);
        if (valid) std::cout << currentTime << " " << message << std::endl;
    }
    return NULL;
}

int reader(int time) {
    std::string line;

    while (true) 
    {
        if(!getline(inputFile, line))
        {
            //cerr << "Not reading a line" << endl;
            inputFile.close();
            inputFile.open("output.txt");
            inputFile.seekg(filePosition);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            return 0;
        }
        filePosition = inputFile.tellg();
        if(line == "!@")
        {
            //cerr << "Input Reading Complete." << endl;
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
        if(spaceIndex != line.size() && line.substr(spaceIndex, 9) == "kforkavya")
        {
            //cerr << "Found my line" << endl;
            continue;
        }
        //cerr << line << endl;
        string_process_2(line, line2);
        //cerr << "line2 = " << line2 << endl;
        if(line2 != "")
        {
            //cerr << "Now printing" << endl;
            userTrader(NULL);
        }
    }
}

int trader(std::string *message)
{
    *message = line2;
    return 1;
}