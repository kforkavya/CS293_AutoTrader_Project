#include "market.h"
#include "stock_rbt.cpp"
#include "client_rbt.cpp"
#include "simple_rbt.cpp"
#include "trade_heap.cpp"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>
#include <math.h>
#include <vector>
#include <string>
#include <limits> // Include for numeric_limits
#include <sstream> // Include for std::istringstream
//#include "trade_heap.cpp"

using namespace std;

vector<Stock_RBT> table(5000);
int Money_Transfered = 0;
int Completed_Trade = 0;
int Shares_Traded = 0;
vector<Client_RBT> table_client(5000);

C_RBT client_store;

int hash_key(string stock){
    
    int sum = 0;
    unsigned int p = 61;

    for(int i = 0 ; i < stock.length() ; i++){
        sum += ((int)stock[i]*p ) % 1000000;
        p *=61;
        p %= 1000000;
    }
    sum = sum % 5000;
    return sum;
}

void cmp (string stock1 , int key , int start , int late_price){

    bool b1 = false;
    node* p = table[key].access(stock1 , b1);

    //cout<<"a";
    //Trade_* buy = p->buy_heap.top(start);
    //cout<<p->company_name<<endl;
    while(true){
    Trade_* sell = p->sell_heap.top(start);
    Trade_* buy = p->buy_heap.top(start);

    //sell->print_data();

    //if(buy.quantity == 0 || sell.quantity == 0) return;
    if(sell == nullptr)return;
    if(buy == nullptr)return;

    //cout<<"c";

    if(buy->price >= sell->price){
        Completed_Trade +=1;
        if(buy->start < sell->start){
            cout<<buy->broker<<" purchased "<<std::min(buy->quantity , sell->quantity)<<" share of "<<stock1<<" from "<<sell->broker<<" for $"<<buy->price<<"/share"<<endl;
            Money_Transfered += std::min(buy->quantity , sell->quantity)*(buy->price);
            Shares_Traded += std::min(buy->quantity , sell->quantity);
            bool b2 = false;
            node_c* p1 = table_client[hash_key(buy->broker)].access(buy->broker,b2); // buyer
            node_c* p2 = table_client[hash_key(sell->broker)].access(sell->broker,b2); // seller
            node_S* p3 = p1 ->RBT.access(stock1 , b2); // buyer
            node_S* p4 = p2 ->RBT.access(stock1 , b2); // seller
            p3 ->buy += std::min(buy->quantity , sell->quantity)*(buy->price);
            p3 ->count_buy += std::min(buy->quantity , sell->quantity);
            p4 ->sell += std::min(buy->quantity , sell->quantity)*(buy->price);
            p4 ->count_sell += std::min(buy->quantity , sell->quantity);
            //p->store.push_back(make_pair(buy->price , std::min(buy->quantity , sell->quantity)));
        }
        else if(buy->start == sell->start){
            if(buy->price == late_price){
                cout<<buy->broker<<" purchased "<<std::min(buy->quantity , sell->quantity)<<" share of "<<stock1<<" from "<<sell->broker<<" for $"<<sell->price<<"/share"<<endl;
                Money_Transfered += std::min(buy->quantity , sell->quantity)*(sell->price);
                Shares_Traded += std::min(buy->quantity , sell->quantity);
                bool b2 = false;
                node_c* p1 = table_client[hash_key(buy->broker)].access(buy->broker,b2);
                node_c* p2 = table_client[hash_key(sell->broker)].access(sell->broker,b2);
                node_S* p3 = p1 ->RBT.access(stock1 , b2);
                node_S* p4 = p2 ->RBT.access(stock1 , b2);
                p3 ->buy += std::min(buy->quantity , sell->quantity)*(sell->price);
                p3 ->count_buy += std::min(buy->quantity , sell->quantity);
                p4 ->sell += std::min(buy->quantity , sell->quantity)*(sell->price);
                p4 ->count_sell += std::min(buy->quantity , sell->quantity);
                //p->store.push_back(make_pair(sell->price , std::min(buy->quantity , sell->quantity)));
            }
            else{
                cout<<buy->broker<<" purchased "<<std::min(buy->quantity , sell->quantity)<<" share of "<<stock1<<" from "<<sell->broker<<" for $"<<buy->price<<"/share"<<endl;
                Money_Transfered += std::min(buy->quantity , sell->quantity)*(buy->price);
                Shares_Traded += std::min(buy->quantity , sell->quantity);
                bool b2 = false;
                node_c* p1 = table_client[hash_key(buy->broker)].access(buy->broker,b2);
                node_c* p2 = table_client[hash_key(sell->broker)].access(sell->broker,b2);
                node_S* p3 = p1 ->RBT.access(stock1 , b2);
                node_S* p4 = p2 ->RBT.access(stock1 , b2);
                p3 ->buy += std::min(buy->quantity , sell->quantity)*(buy->price);
                p3 ->count_buy += std::min(buy->quantity , sell->quantity);
                p4 ->sell += std::min(buy->quantity , sell->quantity)*(buy->price);
                p4 ->count_sell += std::min(buy->quantity , sell->quantity);
                //p->store.push_back(make_pair(buy->price , std::min(buy->quantity , sell->quantity)));
            }
        }
        else{
            cout<<buy->broker<<" purchased "<<std::min(buy->quantity , sell->quantity)<<" share of "<<stock1<<" from "<<sell->broker<<" for $"<<sell->price<<"/share"<<endl;
            Money_Transfered += std::min(buy->quantity , sell->quantity)*(sell->price);
            Shares_Traded += std::min(buy->quantity , sell->quantity);
            bool b2 = false;
            node_c* p1 = table_client[hash_key(buy->broker)].access(buy->broker,b2);
            node_c* p2 = table_client[hash_key(sell->broker)].access(sell->broker,b2);
            node_S* p3 = p1 ->RBT.access(stock1 , b2);
            node_S* p4 = p2 ->RBT.access(stock1 , b2);
            p3 ->buy += std::min(buy->quantity , sell->quantity)*(sell->price);
            p3 ->count_buy += std::min(buy->quantity , sell->quantity);
            p4 ->sell += std::min(buy->quantity , sell->quantity)*(sell->price);
            p4 ->count_sell += std::min(buy->quantity , sell->quantity);
            //p->store.push_back(make_pair(sell->price , std::min(buy->quantity , sell->quantity)));
        }

        int pri = std::min(buy->quantity , sell->quantity);
        p->buy_heap.edit_top(buy->quantity - pri);
        p->sell_heap.edit_top(sell->quantity - pri);
        continue;
    }
    break;
    }
}

void arrange(node* p){

    if(p == nullptr )return;
    sort(p->store.begin(),p->store.end()); // price:qiantity
    int q = 0;
    for(auto I : p->store){
        q += I.second;
    }
    int q1 ; int q2;
    if(q % 2 == 1){
        q1 = q/2;
        q2 = q/2;
    }
    else{
        q1 = q/2;
        q2 = q1+1;
    }
    int m1 = 0,m2;
    for(auto I : p-> store){
        if(m1 != 0){
            m2 = I.first;break;
        }
        q1 -= I.second;
        q2 -= I.second;
        if(q1 <= 0){
            m1 = I.first;
        }
        if(q2 <= 0){
            m2 = m1; break;
        }
    }

    double x = (m1+m2)/2;
    p->median = x;


    if(p -> left != nullptr){
        //sort(p->left->store.begin() , p->left->store.end());
        arrange(p->left);
    }
    if(p -> right != nullptr){
        //sort(p->right->store.begin() , p->right->store.end());
        arrange(p->right);
    }

}

void do_sth(){

    for(int i = 0 ; i < 5000 ; i++){
        node* p = table[1].getRoot();
        arrange(p);
    }

}

void string_process(const std::string& s) {

    std::string input = s;
    std::vector<std::string> words;

    // Create a string stream from the input string
    std::istringstream iss(input);

    // Iterate through the words in the string
    while (iss) {
        std::string word;
        iss >> word;
        if (!word.empty()) {
            words.push_back(word);
        }
    }

    if(words.size()< 7) return;

    for(auto I : words[0]){
        if(!( int(I) >= 48 && int(I) <= 57)){
            return;
        }
    }

    int start = stoi(words[0]);
    string brokername = words[1];
    bool b69 = false;
    node_client_names* ddgauywg = client_store.access(brokername,b69); // broker exist karga then wanna ... wanna uska node ban jaye ga
    string SB = words[2];

    string stock_ = words[3];

    int length = words.size();

    for(int ii = 4 ; ii <= length -4 ; ii++){
        stock_.append(" ");
        stock_.append(words[ii]);
    }

    if(words[length-3][0] != '$') return;
    if(words[length-2][0] != '#') return;
    if( words[2] != "SELL" && words[2] != "BUY") return;

    for(auto I : words[length-3].substr(1, words[length-3].length() - 1)){
        if(!(( int(I) >= 48 && int(I) <= 57)|| ( int(I) == 45))){
            return;
        }
    }

    for(auto I : words[length-2].substr(1, words[length-2].length() - 1)){
        if(!(( int(I) >= 48 && int(I) <= 57)|| ( int(I) == 45))){
            return;
        }
    }

    for(auto I : words[length-1]){
        if(!(( int(I) >= 48 && int(I) <= 57)|| ( int(I) == 45))){
            return;
        }
    }


    int price = stoi(words[length-3].substr(1, words[length-3].length() - 1));
    int quantity = stoi(words[length-2].substr(1, words[length-2].length() - 1));
    int timing = stoi(words[length-1]);
    if(timing == -1) timing = 10000000;

    //cerr<<price<<" "<<quantity<<" "<<timing<<endl;

    int key = hash_key(stock_);
    bool b = false;
    node* p = table[key].access(stock_ , b);

    if(SB == "BUY") b = true;
        else b = false;

    Trade_* trade_pointer = new Trade_(b , price , quantity , start , timing + start , brokername);

    //cout<<trade_pointer->quantity<<endl;

    //cout<<brokername<<endl;

    if(trade_pointer->b == true){
        p->buy_heap.insert(trade_pointer);
    }
    else{
        p->sell_heap.insert(trade_pointer);
    }

    // ---------------------------------------

    //Trade_* buy = p->buy_heap.top(start);
    //Trade_* sell = p->sell_heap.top(start);
    //cout<<"b";

    //buy->print_data();
    //sell->print_data();

    // ------------------------------------------------

    cmp ( stock_ , key , start , price);

}

void final_loop(node_S* p , int &sell_profit , int &buy_profit , int &sellc , int &buyc){
    if( p == nullptr) return;
    buy_profit -= p->buy;
    sell_profit += p->sell;
    //cout<<p->buy<<endl;
    //string stock = p->company_name;
    //int key = hash_key(stock);

    // bool b = false;
    // node* p2 = table[key].access(stock , b);
    //buy_profit += p2->median * p->count_buy;
    //sell_profit -= p2 ->median * p->count_sell;
    //cout<<p2->median<<endl;
    sellc += p->count_sell;
    buyc += p->count_buy;

    final_loop(p->left , sell_profit , buy_profit , sellc , buyc);
    final_loop(p->right , sell_profit , buy_profit , sellc , buyc);
}

void it_it(node_c* p){ // now u have client name; // or client pointer
    if(p == nullptr)return;

    // string c_name = p->client_name;
    int sell_profit = 0;
    int buy_profit = 0;
    int sellc = 0;
    int buyc = 0;
    
    node_S* ptr = p->RBT.getRoot();

    final_loop(ptr , sell_profit , buy_profit, sellc , buyc);

    cout<<p->client_name<<" bought "<<buyc<<" and sold "<<sellc<<" for a net transfer of $"<<(sell_profit+buy_profit)<<endl;

}

void finish_it(node_client_names* p){
    if( p == nullptr) return;
    string s = p->company_name;
    int key = hash_key(s);
    node_c* pointer = table_client[key].getRoot(); // particular company hai ye;
    //node_S* ptr = pointer->RBT.getRoot(); // 
    it_it(pointer);
    if(p->left != nullptr) finish_it(p->left);
    if(p->right != nullptr) finish_it(p->right);
}

// int main(){
    
//     std::fstream file("in.txt", std::ios::in);
//     std::string line;
//     while (getline(file, line)) { // take lines // take lines
//         string_process(line); // break the line // add nodes and modiefy everything
//     }
//     cout<<endl<<"---End of Day---"<<endl<<"Total Amount of Money Transferred: $"<<Money_Transfered<<endl<<"Number of Completed Trades: "<<Completed_Trade<<endl<<"Number of Shares Traded: "<<Shares_Traded<<endl;

//     //do_sth();

//     node_client_names* pp = client_store.getRoot();
//     finish_it(pp);

// }



market::market(int argc, char** argv)
{
	
}

void market::start()
{
    
    std::fstream file("output.txt", std::ios::in);
    std::string line;
    while (true) { // take lines
        std::getline(file, line);
        //cerr << line << endl;
        if(line == "") continue;
        //std::cout<<line<<std::endl; // break the line // add nodes and modiefy everything
        if(line == "@!") break;
        if(line == "TL") continue;
        if(line == "!@") continue;
        string_process(line); // break the line // add nodes and modiefy everything
    }

    cout<<endl<<"---End of Day---"<<endl<<"Total Amount of Money Transferred: $"<<Money_Transfered<<endl<<"Number of Completed Trades: "<<Completed_Trade<<endl<<"Number of Shares Traded: "<<Shares_Traded<<endl;

    //do_sth();

    node_client_names* pp = client_store.getRoot();
    finish_it(pp);
	
}

/////////////////////////////




