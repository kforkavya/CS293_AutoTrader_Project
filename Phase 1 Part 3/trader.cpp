#include "receiver.h"
#include "stock_rbt.cpp"
#include "deal_rbt.cpp"
#include "order_book.cpp"
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
        while(curr_length == 0)
        {
            message = rcv.readIML();
            curr_length = message.length();
        }
        bool input_finished = (message[curr_length - 1] == '$') ? true : false;
        if(input_finished) rcv.terminate();
        //some pre-processing
        Stock_RBT* Stocks = new Stock_RBT[10000];
        calc_pre_computed_table();

        //processing and outputs
        int i = 0;
        while(message[i]!='$')
        {
            if(message[i+1] == '$') break;
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
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                while(curr_length == 0)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                }
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                if(input_finished) rcv.terminate();
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
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                while(curr_length == 0)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                }
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                if(input_finished) rcv.terminate();
                i = 0;
            }
            int price = stoi(price_string);
            char tag = message[i];
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                while(curr_length == 0)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                }
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                if(input_finished) rcv.terminate();
                i = 0;
            }
            assert(message[i]=='#');
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                while(curr_length == 0)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                }
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                if(input_finished) rcv.terminate();
                i = 0;
            }
            if(message[i] == '\r')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            if(message[i] == '\n')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
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
                if(cancelled == false && (stock_pointer->no_best_bp == true || (stock_pointer->no_best_bp == false && stock_pointer->best_buying_price > price)))
                {
                    stock_pointer->best_buying_price = price;
                    stock_pointer->no_best_bp = false;
                }
                //Cancellation Law
                if(cancelled == false && stock_pointer->no_best_sp == false && stock_pointer->best_selling_price == price)
                {
                    stock_pointer->best_selling_price = -1;
                    stock_pointer->no_best_sp = true;
                    if(stock_pointer->best_buying_price == price)
                    {
                        stock_pointer->no_best_bp = true;
                        stock_pointer->best_buying_price = -1;
                    }
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
                if(cancelled == false && (stock_pointer->no_best_sp == true || (stock_pointer->no_best_sp == false && stock_pointer->best_selling_price < price)))
                {
                    stock_pointer->best_selling_price = price;
                    stock_pointer->no_best_sp = false;
                }
                //Cancellation Law
                if(cancelled == false && stock_pointer->no_best_bp == false && stock_pointer->best_buying_price == price)
                {
                    stock_pointer->best_buying_price = -1;
                    stock_pointer->no_best_bp = true;
                    if(stock_pointer->best_selling_price == price)
                    {
                        stock_pointer->no_best_sp = true;
                        stock_pointer->best_selling_price = -1;
                    }
                    cancelled = true;
                }
            }
            //Now printing
            if(cancelled == true)
                cout<<"No Trade"<<endl;
            else
            {
                if(inserted_first_time)
                {
                    stock_pointer->price_estimator = price;
                    cout<<stock<<" "<<price<<" ";
                    if(tag == 'b')
                        cout<<"s"<<endl;
                    else
                        cout<<"b"<<endl;
                }
                else if(tag == 'b' && inserted_first_time == false)
                {
                    if(stock_pointer->price_estimator < price)
                    {
                        stock_pointer->price_estimator = price;
                        cout<<stock<<" "<<price<<" s"<<endl;
                        if(stock_pointer->no_best_sp == false && stock_pointer->best_selling_price == price)
                        {
                            stock_pointer->best_selling_price = -1;
                            stock_pointer->no_best_sp = true;
                        }
                    }
                    else
                        cout<<"No Trade"<<endl;
                }
                else
                {
                    if(stock_pointer->price_estimator > price)
                    {
                        stock_pointer->price_estimator = price;
                        cout<<stock<<" "<<price<<" b"<<endl;
                        if(stock_pointer->no_best_bp == false && stock_pointer->best_buying_price == price)
                        {
                            stock_pointer->best_buying_price = -1;
                            stock_pointer->no_best_bp = true;
                        }
                    }
                    else
                        cout<<"No Trade"<<endl;
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
        while(curr_length == 0)
        {
            message = rcv.readIML();
            curr_length = message.length();
        }
        bool input_finished = (message[curr_length - 1] == '$') ? true : false;
        if(input_finished) rcv.terminate();

        //some pre-processing
        Deal_RBT* Deals = new Deal_RBT[10000];
        Graph_List* Graph = new Graph_List();
        calc_pre_computed_table();
        int total_profit = 0;
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
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            assert(message[i]=='#');
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                while(curr_length == 0)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                }
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                if(input_finished) rcv.terminate();
                i = 0;
            }
            if(message[i] == '\r')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            if(message[i] == '\n')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }

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
            //cout<<line<<endl;
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
                //cout<<"Hey"<<endl;
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
                    //cout<<"Here2"<<endl;
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
                //cout<<"Anyone"<<endl;
                Deals[hash].deleteNode(deal_pointer);
            }
            if(cancelled==true)
            {
                cout<<"No Trade"<<endl;
            }
            else
            {
                graph_ptr curr_order = Graph->insert_order(deal_pointer, tag, order_no, price_deal, hash, stock_structure);
                vector<graph_ptr> max_arbitrage_lane; int max_profit;
                Graph->find_max_arbitrage(curr_order, max_arbitrage_lane, max_profit);
                if(max_arbitrage_lane.size()==0)
                {
                    cout<<"No Trade"<<endl;
                }
                else
                {
                    for(int i = 0; i<max_arbitrage_lane.size(); i++)
                    {
                        cout<<max_arbitrage_lane[i]->actual_stock_structure<<" "<<max_arbitrage_lane[i]->price<<" ";
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
                    total_profit += max_profit;
                }
            }
        }
        cout<<total_profit<<endl;
        delete_pre_computed_table();
        delete Graph;
        delete[] Deals;
    }
    else if(strcmp(argv[1], arg3)==0)
    {
        //input
        Receiver rcv;
        string message = rcv.readIML();
        int curr_length = message.length();
        while(curr_length == 0)
        {
            message = rcv.readIML();
            curr_length = message.length();
        }
        bool input_finished = (message[curr_length - 1] == '$') ? true : false;
        if(input_finished) rcv.terminate();

        //some pre-processing
        Order_Book_RBT* Orders = new Order_Book_RBT[10000];
        New_Graph_List* Graph = new New_Graph_List();
        calc_pre_computed_table();
        int total_profit = 0;
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
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            assert(message[i]=='#');
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                while(curr_length == 0)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                }
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                if(input_finished) rcv.terminate();
                i = 0;
            }
            if(message[i] == '\r')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            if(message[i] == '\n')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    while(curr_length == 0)
                    {
                        message = rcv.readIML();
                        curr_length = message.length();
                    }
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
                    if(input_finished) rcv.terminate();
                    i = 0;
                }
            }
            //separating input elements
            order_no++;
            int length = line.length();
            char tag = line[length-1];
            int third_space_index = length-2;
            int second_space_index = length-4;
            while(line[second_space_index]!=' ')
                second_space_index--;
            string quantity_order_string = line.substr(second_space_index+1, third_space_index-second_space_index-1);
            int quantity_order = stoi(quantity_order_string);
            int first_space_index = second_space_index - 2;
            while(line[first_space_index]!=' ')
                first_space_index--;
            string price_order_string = line.substr(first_space_index+1, second_space_index-first_space_index-1);
            int price_order = stoi(price_order_string);
            string stock_structure = line.substr(0, first_space_index);
            //cout<<line<<endl;
            //finally processing
            vector<pair<string, int>> tokenised_string;
            string sorted_stock_structure;
            stock_structure_disintegrator(stock_structure, tokenised_string, sorted_stock_structure);
            sorted_stock_structure += " "+price_order_string;
            //cout<<"Processing Sorted Stock = "<<sorted_stock_structure<<" Price = "<<price_order<<" Quantity = "<<quantity_order<<" Tag = "<<tag<<endl;
            bool cancelled = false, inserted_first_time = false;
            int hash = hashValue(sorted_stock_structure);
            order_book_ptr order_pointer;
            Orders[hash].access(sorted_stock_structure, order_pointer, inserted_first_time, stock_structure, tokenised_string, price_order, quantity_order, tag, order_no, hash);
            if(inserted_first_time == true)
            {
                //cout<<"Hey"<<endl;
                Graph->insert_order_in_graph(order_pointer);
            }
            else
            {
                if(tag == order_pointer->tag)
                {
                    //cout<<"Over here\n";
                    order_pointer->quantity += quantity_order;
                    //cout<<"This much done"<<endl;
                    Graph->delete_order_from_graph(order_pointer);
                    //cout<<"Deletion done"<<endl;
                    order_pointer->order_no = order_no;
                    Graph->insert_order_in_graph(order_pointer);
                    //cout<<"Insertion Done"<<endl;
                }
                else
                {
                    if(order_pointer->quantity > quantity_order)
                    {
                        order_pointer->quantity -= quantity_order;
                        order_pointer->order_no = order_no;
                        cancelled = true; //No Checks for Arbitrage
                    }
                    else if(order_pointer->quantity < quantity_order)
                    {
                        order_pointer->tag = tag;
                        order_pointer->quantity = quantity_order - order_pointer->quantity;
                        order_pointer->actual_stock_structure = stock_structure;
                        order_pointer->order_no = order_no;
                        Graph->delete_order_from_graph(order_pointer);
                        Graph->insert_order_in_graph(order_pointer);
                    }
                    else
                    {
                        Graph->delete_order_from_graph(order_pointer);
                        Orders[hash].deleteNode(order_pointer);
                        cancelled = true; //No Checks for Arbitrage
                    }
                }
            }
            if(cancelled==true)
            {
                //cout<<"Cancelled\n";
                cout<<"No Trade"<<endl;
            }
            else
            {
                vector<pair<order_book_ptr, int> > max_arbitrage_lane; int max_profit;
                Graph->find_max_arbitrage(order_pointer, max_arbitrage_lane, max_profit);
                //cout<<"Here ?"<<endl;
                if(max_arbitrage_lane.size()==0)
                {
                    cout<<"No Trade"<<endl;
                }
                else
                {
                    //cout<<"Arbitrage Found !"<<endl;
                    for(int i = 0; i<max_arbitrage_lane.size(); i++)
                    {
                        cout<<max_arbitrage_lane[i].first->actual_stock_structure<<" "<<max_arbitrage_lane[i].first->price<<" "<<max_arbitrage_lane[i].second<<" ";
                        cout<<(max_arbitrage_lane[i].first->tag == 's' ? 'b' : 's')<<endl;
                        char tag = max_arbitrage_lane[i].first->tag;
                        int hash = max_arbitrage_lane[i].first->hash;
                        int quantity_order = max_arbitrage_lane[i].first->quantity;
                        if(max_arbitrage_lane[i].second < quantity_order)
                            max_arbitrage_lane[i].first->quantity = quantity_order - max_arbitrage_lane[i].second;
                        else
                        {
                            //cout<<"Deleting..."<<endl;
                            Graph->delete_order_from_graph(max_arbitrage_lane[i].first);
                            //cout<<"Step 1 Done"<<endl;
                            Orders[hash].deleteNode(max_arbitrage_lane[i].first);
                            //cout<<"Completed"<<endl;
                        }
                    }
                    total_profit += max_profit;
                }
            }
        }
        cout<<total_profit<<endl;
        delete_pre_computed_table();
        delete Graph;
        delete[] Orders;
    }
    return 0;
}
