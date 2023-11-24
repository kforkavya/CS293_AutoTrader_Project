// This is your trader. Place your orders from here

#include "active_stock_RBT.cpp"
#include "string.cpp"
#include "hash.cpp"
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
active_stock_RBT* Active_Stock_Orders = new active_stock_RBT[10000];

void parseMessage(string input, active_order_ptr& order_ptr, active_stock_ptr& stock_ptr)
{
    int i = 0; string insertion_time_string = "", trader_name = "";
    while(input[i] != ' ')
        insertion_time_string += input[i++];
    int insertion_time = stoi(insertion_time_string);
    i++;
    while(input[i] != ' ')
        trader_name += input[i++];
    i++;
    string tag_string = "";
    while(input[i] != ' ')
        tag_string += input[i++];
    i++;
    char tag = 's';
    if(tag_string == "BUY") tag = 'b';
    int j = input.size()-1;
    string time_duration_string = "";
    while(input[j]!=' ')
        time_duration_string = input[j--] + time_duration_string;
    int time_duration = stoi(time_duration_string), expiration_time = -1;
    if(time_duration != -1)
        expiration_time = insertion_time + time_duration;
    j--;
    string quantity_string = "";
    while(input[j]!='#')
        quantity_string = input[j--] + quantity_string;
    int quantity = stoi(quantity_string);
    j-=2;
    string price_string = "";
    while(input[j]!='$')
        price_string = input[j--] + price_string;
    int price = stoi(price_string);
    j-=2;
    string actual_stock_structure = input.substr(i, j-i+1), sorted_stock_structure;
    vector<pair<string, int> > tokenised_string;
    if(actual_stock_structure[actual_stock_structure.size()-1] < '0' || actual_stock_structure[actual_stock_structure.size()-1] > '9')
    {
        tokenised_string.push_back({actual_stock_structure, 1});
        sorted_stock_structure = actual_stock_structure + " 1";
    }
    else
        stock_structure_disintegrator(actual_stock_structure, tokenised_string, sorted_stock_structure);
    int hash = hashValue(sorted_stock_structure);
    bool inserted_first_time = false;
    Active_Stock_Orders->access(sorted_stock_structure, stock_ptr, inserted_first_time, actual_stock_structure, tokenised_string, hash);
    order_ptr = new active_order_node(price, quantity, trader_name, expiration_time);
}

int reader(int time) {
    if (!inputFile.is_open()) {
        cerr << "Error opening file for reading!" << endl;
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
            delete[] Active_Stock_Orders;
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
        if(spaceIndex != line.size() && line.substr(spaceIndex, 15) == "22B1053_22B0982")
        {
            cerr << "Hit" << endl;
            continue;
        }
        line2 = "22B1053_22B0982 "+line;
        userTrader(NULL);
        // If the end of the file is reached, wait for new data
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
}

int trader(std::string *message)
{
    *message = line2;
    return 1;
}
