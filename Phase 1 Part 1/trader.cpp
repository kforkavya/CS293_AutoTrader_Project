#include "receiver.h"
#include "stock_rbt.cpp"
#include "hash.cpp"
using namespace std;

int main(int argc, char *argv[]) {
    char arg1[] = {'1', '\0'};
    if(strcmp(argv[1], arg1)==0)
    {
        //input
        Receiver rcv;
        sleep(5);
        string message = rcv.readIML();
        int curr_length = message.length();
        bool input_finished = (message[curr_length - 1] == '$') ? true : false;

        //some pre-processing
        Stock_RBT* Stocks = new Stock_RBT[10000];
        calc_pre_computed_table();
        cout<<message<<endl;
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
            assert(message[i]=='#');
            i++;
            if(i == curr_length && input_finished == false)
            {
                message = rcv.readIML();
                curr_length = message.length();
                input_finished = (message[curr_length - 1] == '$') ? true : false;
                i = 0;
            }
            if(message[i] == '\r')
            {
                i++;
                if(i == curr_length && input_finished == false)
                {
                    message = rcv.readIML();
                    curr_length = message.length();
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
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
                    input_finished = (message[curr_length - 1] == '$') ? true : false;
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
    return 0;
}
