#include "receiver.h"
#include "stock_rbt.cpp"
#include "deal_rbt.cpp"
#include "hash.cpp"
#include "string.cpp"
using namespace std;

int main(int argc, char *argv[]) {
    char arg1[] = {'1', '\0'}, arg2[] = {'2', '\0'}, arg3[] = {'3', '\0'};
    if(strcmp(argv[1], arg1)==0)
    {
        //input
        Receiver rcv;
        string message = rcv.readIML();
        int curr_length = message.length();
        bool input_finished = (message[curr_length - 1] == '$') ? true : false;

        //some pre-processing
        Stock_RBT* Stocks = new Stock_RBT[10000];
        calc_pre_computed_table();

        //processing and outputs
        int i = 0;
        while(message[i]!='$')
        {
            //Input extraction
            string stock = "";
            while(message[i]!=' ')
            {
                if((message[i] >= 'A' && message[i] <= 'Z')||(message[i] >= 'a' && message[i] <= 'z'))
                    stock = stock + message[i];
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    i = 0;
                }
            }
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                i = 0;
            }
            string price_string = "";
            while(message[i]!=' ')
            {
                if(message[i] >= '0' && message[i] <= '9')
                price_string = price_string + message[i];
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    i = 0;
                }
            }
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                i = 0;
            }
            int price = stoi(price_string);
            char tag = message[i];
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                i = 0;
            }
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                i = 0;
            }

            //Now finally processing
            bool cancelled = false, inserted_first_time = false;
            int hash = hashValue(stock);
            ptr stock_pointer = Stocks[hash].access(stock, inserted_first_time);
            if(tag == 's' && inserted_first_time == false)
            {
                //No multiple stocks of same structure
                if(stock_pointer->no_best_bp == false && stock_pointer->best_buying_price == price)
                    cancelled = true;
                //If already my best_buying_price was the best, then this order is cancelled.
                if(stock_pointer->no_best_bp == false && stock_pointer->best_buying_price < price)
                    cancelled = true;
                //If this price is lower than the best buying price yet, then I can improve on it.
                if(stock_pointer->no_best_bp == true || (stock_pointer->no_best_bp == false && stock_pointer->best_buying_price > price))
                {
                    stock_pointer->best_buying_price = price;
                    stock_pointer->no_best_bp = false;
                }
                //Cancellation Law
                if(stock_pointer->no_best_sp == false && stock_pointer->best_selling_price == price)
                {
                    stock_pointer->best_selling_price = -1;
                    stock_pointer->no_best_sp = true;
                    cancelled = true;
                }
            }
            else if(tag == 'b' && inserted_first_time == false)
            {
                //No multiple stocks of same structure
                if(stock_pointer->no_best_sp == false && stock_pointer->best_selling_price == price)
                    cancelled = true;
                //If already my best_selling_price was the best, then this order is cancelled.
                if(stock_pointer->no_best_sp == false && stock_pointer->best_selling_price > price)
                    cancelled = true;
                //If this price is higher than the best selling price yet, then I can improve on it.
                if(stock_pointer->no_best_sp == true || (stock_pointer->no_best_sp == false && stock_pointer->best_selling_price < price))
                {
                    stock_pointer->best_selling_price = price;
                    stock_pointer->no_best_sp = false;
                }
                //Cancellation Law
                if(stock_pointer->no_best_bp == false && stock_pointer->best_buying_price == price)
                {
                    stock_pointer->best_buying_price = -1;
                    stock_pointer->no_best_bp = true;
                    cancelled = true;
                }
            }
            //Now printing
            if(cancelled == true)
                cout<<"No Trade\n";
            else
            {
                if(inserted_first_time)
                {
                    stock_pointer->price_estimator = price;
                    cout<<stock<<" "<<price<<" ";
                    if(tag == 'b')
                        cout<<"s\n";
                    else
                        cout<<"b\n";
                }
                else if(tag == 'b' && inserted_first_time == false)
                {
                    if(stock_pointer->price_estimator < price)
                    {
                        stock_pointer->price_estimator = price;
                        cout<<stock<<" "<<price<<" s\n";
                        if(stock_pointer->no_best_sp == false && stock_pointer->best_selling_price == price)
                        {
                            stock_pointer->best_selling_price = -1;
                            stock_pointer->no_best_sp = true;
                        }
                    }
                    else
                        cout<<"No Trade\n";
                }
                else
                {
                    if(stock_pointer->price_estimator > price)
                    {
                        stock_pointer->price_estimator = price;
                        cout<<stock<<" "<<price<<" b\n";
                        if(stock_pointer->no_best_bp == false && stock_pointer->best_buying_price == price)
                        {
                            stock_pointer->best_buying_price = -1;
                            stock_pointer->no_best_bp = true;
                        }
                    }
                    else
                        cout<<"No Trade\n";
                }
            }
        }
        delete_pre_computed_table();
        delete[] Stocks;
    }
    else if(strcmp(argv[1], arg2)==0)
    {
        //input
        Receiver rcv;
        string message = rcv.readIML();
        int curr_length = message.length();
        bool input_finished = (message[curr_length - 1] == '$') ? true : false;

        //some pre-processing
        Deal_RBT* Deals = new Deal_RBT[10000];
        Graph_List* Graph = new Graph_List();
        calc_pre_computed_table();
        //extracting input
        int i = 0, order_no = 0;
        while(message[i]!='$')
        {
            string line = "";
            while(message[i]!='#')
            {
                if((message[i] >= '0' && message[i] <= '9')||(message[i] >= 'A' && message[i] <= 'Z')||(message[i] >= 'a' && message[i] <= 'z')||message[i]==' '||message[i]=='-')
                    line = line + message[i];
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    i = 0;
                }
            }
            i++;
            if(message[i]!='$') i++;
            //separating input elements
            order_no++;
            int length = line.length();
            char tag = line[length-1];
            int second_space_index = length-2;
            int first_space_index = length-4;
            while(line[first_space_index]!=' ')
                first_space_index--;
            string price_deal_string = line.substr(first_space_index+1, second_space_index-first_space_index-1);
            int price_deal = stoi(price_deal_string);
            string stock_structure = line.substr(0, first_space_index);
            //finally processing
            vector<pair<string, int>> tokenised_string;
            string sorted_stock_structure;
            stock_structure_disintegrator(stock_structure, tokenised_string, sorted_stock_structure);
            bool cancelled = false, inserted_first_time = false;
            int hash = hashValue(sorted_stock_structure);
            deal_ptr deal_pointer;
            Deals[hash].access(sorted_stock_structure, deal_pointer, inserted_first_time, stock_structure, tokenised_string, price_deal);
            if(inserted_first_time == true)
            {
                if(tag == 's')
                {
                    deal_pointer->best_buying_price = price_deal;
                    deal_pointer->no_best_bp = false;
                }
                else
                {
                    deal_pointer->best_selling_price = price_deal;
                    deal_pointer->no_best_sp = false;
                }
            }
            if(tag == 's' && inserted_first_time == false)
            {
                //No multiple stocks of same structure
                if(deal_pointer->no_best_bp == false && deal_pointer->best_buying_price == price_deal)
                    cancelled = true;
                //If already my best_buying_price was the best, then this order is cancelled.
                if(deal_pointer->no_best_bp == false && deal_pointer->best_buying_price < price_deal)
                    cancelled = true;
                //If this price is lower than the best buying price yet, then I can improve on it.
                if(deal_pointer->no_best_bp == true || (deal_pointer->no_best_bp == false && deal_pointer->best_buying_price > price_deal))
                {
                    deal_pointer->best_buying_price = price_deal;
                    if(deal_pointer->no_best_bp==false) Graph->delete_best_bp_order(deal_pointer);
                    deal_pointer->no_best_bp = false;
                }
                //Cancellation Law
                if(deal_pointer->no_best_sp == false && deal_pointer->best_selling_price == price_deal)
                {
                    if(deal_pointer->best_buying_price == price_deal)
                    {
                        deal_pointer->best_buying_price = -1;
                        deal_pointer->no_best_bp = true;
                    }
                    deal_pointer->best_selling_price = -1;
                    deal_pointer->no_best_sp = true;
                    Graph->delete_best_sp_order(deal_pointer);
                    cancelled = true;
                }
            }
            else if(tag == 'b' && inserted_first_time == false)
            {
                //No multiple stocks of same structure
                if(deal_pointer->no_best_sp == false && deal_pointer->best_selling_price == price_deal)
                    cancelled = true;
                //If already my best_selling_price was the best, then this order is cancelled.
                if(deal_pointer->no_best_sp == false && deal_pointer->best_selling_price > price_deal)
                    cancelled = true;
                //If this price is higher than the best selling price yet, then I can improve on it.
                if(deal_pointer->no_best_sp == true || (deal_pointer->no_best_sp == false && deal_pointer->best_selling_price < price_deal))
                {
                    deal_pointer->best_selling_price = price_deal;
                    if(deal_pointer->no_best_sp==false) Graph->delete_best_sp_order(deal_pointer);
                    deal_pointer->no_best_sp = false;
                }
                //Cancellation Law
                if(deal_pointer->no_best_bp == false && deal_pointer->best_buying_price == price_deal)
                {
                    if(deal_pointer->best_selling_price == price_deal)
                    {
                        deal_pointer->best_selling_price = -1;
                        deal_pointer->no_best_sp = true;
                    }
                    deal_pointer->best_buying_price = -1;
                    deal_pointer->no_best_bp = true;
                    Graph->delete_best_bp_order(deal_pointer);
                    cancelled = true;
                }
            }
            if(deal_pointer->no_best_bp == true && deal_pointer->no_best_sp == true)
            {
                Deals[hash].deleteNode(deal_pointer);
            }
            if(cancelled==true)
            {
                cout<<"No Trade\n";
            }
            else
            {
                graph_ptr curr_order = Graph->insert_order(deal_pointer, tag, order_no, price_deal, hash);
                vector<graph_ptr> max_arbitrage_lane; int max_profit;
                Graph->find_max_arbitrage(curr_order, max_arbitrage_lane, max_profit);
                if(max_arbitrage_lane.size()==0)
                {
                    cout<<"No Trade\n";
                }
                else
                {
                    for(int i = 0; i<max_arbitrage_lane.size(); i++)
                    {
                        cout<<max_arbitrage_lane[i]->deal_pointer->actual_stock_structure<<" "<<max_arbitrage_lane[i]->price<<" ";
                        cout<<(max_arbitrage_lane[i]->tag == 's' ? 'b' : 's')<<endl;
                        char tag = max_arbitrage_lane[i]->tag;
                        int hash = max_arbitrage_lane[i]->hash;
                        deal_ptr deal_pointer = max_arbitrage_lane[i]->deal_pointer;
                        Graph->deleteGraphNode(max_arbitrage_lane[i]);
                        if(tag == 'b')
                        {
                            deal_pointer->best_selling_price = -1;
                            deal_pointer->no_best_sp = true;
                            if(deal_pointer->no_best_bp == true)
                            {
                                Deals[hash].deleteNode(deal_pointer);
                            }
                        }
                        else
                        {
                            deal_pointer->best_buying_price = -1;
                            deal_pointer->no_best_bp = true;
                            if(deal_pointer->no_best_sp == true)
                            {
                                Deals[hash].deleteNode(deal_pointer);
                            }
                        }
                    }
                    cout<<max_profit<<"\n";
                }
            }
        }
        delete_pre_computed_table();
        delete Graph;
        delete[] Deals;
    }
    return 0;
}
