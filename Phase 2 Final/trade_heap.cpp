#ifndef TRADE_HEAP_H
#define TRADE_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#define INF 10000000 // 7-zeros 

struct Trade_{
    int price;
    std::string broker;
    int quantity;
    int start;
    int end;
    bool b ; // 1b 0s
    int line_tec;
    Trade_( bool b_ , int price_ , int quantity_ , int start_ , int end_ , std::string broker_ , int linexxx){
        b = b_;
        price = price_;
        quantity = quantity_;
        start = start_;
        end = end_;
        broker = broker_;
        line_tec = linexxx;
    }

    void print_data(){
        std::cout<<"broker : "<<broker<<std::endl;
        std::cout<<"price : "<<price<<std::endl;
        std::cout<<"quant : "<<quantity<<std::endl;
    }
};

class Insider_Heap{

    public:

    std::vector<Trade_> v_heap;

    void insert(Trade_* trade_pointer){

        v_heap.push_back(*trade_pointer);
        int index = v_heap.size() - 1;

        if(v_heap[0].b == true){
            while (index > 0) {
                int parent = (index - 1)/2;
                //std::cout<<"a";
                if(parent == index)break;

                if (v_heap[parent].price < v_heap[index].price){
                    //std::cout<<"1";
                    std::swap(v_heap[parent], v_heap[index]);
                    index = parent;
                }
                else if(v_heap[parent].price == v_heap[index].price){
                    //std::cout<<"2";
                    if(v_heap[parent].line_tec == v_heap[index].line_tec){
                        //std::cout<<"3";
                        if(v_heap[parent].broker > v_heap[index].broker){
                            std::swap(v_heap[parent], v_heap[index]);
                            index = parent;
                            //std::cout<<"c";
                        }
                        else{
                            break;
                        }
                    }
                    else{
                        break;
                    }
                }
                else break;
            }
            //std::cout<<"b";
        }

        else{
            while (index > 0) {
                int parent = (index - 1) / 2;

                if (v_heap[parent].price > v_heap[index].price) {
                    std::swap(v_heap[parent], v_heap[index]);
                    index = parent;
                }
                else if(v_heap[parent].price == v_heap[index].price){
                    //std::cout<<"2";
                    if(v_heap[parent].line_tec == v_heap[index].line_tec){
                        //std::cout<<"3";
                        if(v_heap[parent].broker > v_heap[index].broker){
                            std::swap(v_heap[parent], v_heap[index]);
                            index = parent;
                            //std::cout<<"c";
                        }
                        else{
                            break;
                        }
                    }
                    else{
                        break;
                    }
                }
                else break;
            }
        }

        return;
    }

    void heapify(int p){ 

        if(v_heap[p].b == true){

            int core = 0;
            int l = 0;
            int r = v_heap.size() -1;
            r -=l;
            p-=l;
            l = 0;
            int rs,ls;
            if(2*p+1>r) ls = -1000000;
            else ls = (v_heap[2*p +1 + core]).price;
            if(2*p+2>r) rs = -1000000;
            else rs = (v_heap[2*p +2 + core]).price;

            if((v_heap[p + core]).price > std::max(ls , rs)) return;
            else if ((v_heap[p + core]).price < std::max(ls , rs)){
                if(ls > rs){ 
                    std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                    heapify(2*p+1+core);
                }
                if(rs > ls){ 
                    std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                    heapify(2*p+2+core);
                }
                if(rs == ls){
                    if(v_heap[2*p+1+core].line_tec < v_heap[2*p + 2].line_tec){
                        std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                        heapify(2*p+1+core);
                    }
                    else if(v_heap[2*p+1+core].line_tec > v_heap[2*p + 2].line_tec){
                        std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                        heapify(2*p+2+core);
                    }
                    else{
                        if(v_heap[2*p+core].broker < v_heap[2*p + 2].broker){
                            std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                            heapify(2*p+1+core);
                        }
                        else{
                            std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                            heapify(2*p+2+core);
                        }
                    }
                }
            }
            else{
                if(ls > rs){ // ls matches with top
                    if(v_heap[2*p+1+core].line_tec < v_heap[p + core].line_tec){
                        std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                        heapify(2*p+1+core);
                    }
                    else if (v_heap[2*p+1+core].line_tec == v_heap[p + core].line_tec){
                        if(v_heap[2*p+1+core].broker < v_heap[p + core].broker){
                            std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                            heapify(2*p+1+core);
                        }
                    }
                    else{
                        
                        // do nth
                    }
                }
                if(ls < rs){ // rs matches with top
                    if(v_heap[2*p+2+core].line_tec < v_heap[p + core].line_tec){
                        std::swap(v_heap[p + core],v_heap[2*p+2 + core]);
                        heapify(2*p+2+core);
                    }
                    else if (v_heap[2*p+2+core].line_tec == v_heap[p + core].line_tec){
                        if(v_heap[2*p+2+core].broker < v_heap[p + core].broker){
                            std::swap(v_heap[p + core],v_heap[2*p+2 + core]);
                            heapify(2*p+2+core);
                        }
                    }
                    else{
                        // do nth
                    }
                }
                if(rs == ls){ // 

                    // will write it
                    //std::cout<<"tec1";
                    if(v_heap[2*p+1+core].line_tec < v_heap[2*p + 2].line_tec){
                        if(v_heap[p+core].line_tec < v_heap[2*p + 1].line_tec){
                            // do nth
                        }
                        else if(v_heap[p+core].line_tec > v_heap[2*p + 1].line_tec){
                            std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                            heapify(2*p+1+core);
                        }
                        else{
                            if(v_heap[p+core].broker < v_heap[2*p + 1].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                                heapify(2*p+1+core);
                            }
                        }
                    }
                    else if(v_heap[2*p+1+core].line_tec > v_heap[2*p + 2].line_tec){
                        if(v_heap[p+core].line_tec < v_heap[2*p + 2].line_tec){
                            // do nth
                        }
                        else if(v_heap[p+core].line_tec > v_heap[2*p + 2].line_tec){
                            std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                            heapify(2*p+2+core);
                        }
                        else{
                            if(v_heap[p+core].broker < v_heap[2*p + 2].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                                heapify(2*p+2+core);
                            }
                        }
                    }
                    else{ // niche dono ka time stamp same hai

                        // will do final
                        //std::cout<<"tecf1";
                        if(v_heap[p+core].line_tec == v_heap[2*p + 2].line_tec){ // time stamps hai
                            if(v_heap[p+core].broker > v_heap[2*p + 2].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                                heapify(2*p+2+core);
                            }
                            else if(v_heap[p+core].broker > v_heap[2*p + 1].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                                heapify(2*p+1+core);
                            }
                            else{
                                // do nth
                            }
                        }
                        else{
                            if(v_heap[p+core].line_tec > v_heap[2*p + 2].line_tec){
                                if(v_heap[p+core].broker > v_heap[2*p + 2].broker){
                                    std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                                    heapify(2*p+2+core);
                                }
                                else if(v_heap[p+core].broker > v_heap[2*p + 1].broker){
                                    std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                                    heapify(2*p+1+core);
                                }
                                else{
                                    // do nth
                                }
                            }
                        }
                        
                    }
                    
                }
            }

        }


        else{

            int core = 0;
            int l = 0;
            int r = v_heap.size() -1;
            r -=l;
            p-=l;
            l = 0;
            int rs,ls;
            if(2*p+1>r)ls = 1000000;
            else ls = (v_heap[2*p +1 + core]).price;
            if(2*p+2>r)rs = 1000000;
            else rs = (v_heap[2*p +2 + core]).price;

            if((v_heap[p + core]).price < std::min(ls , rs)) return;
            else if((v_heap[p + core]).price > std::min(ls , rs)){
                if(ls > rs){ 
                    std::swap(v_heap[p + core],v_heap[2*p+2 + core]);
                    heapify(2*p+2+core);
                }
                if(rs > ls){ 
                    std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                    heapify(2*p+1+core);
                }
                if(rs == ls){
                    if(v_heap[2*p+1+core].line_tec < v_heap[2*p + 2].line_tec){
                        std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                        heapify(2*p+1+core);
                    }
                    else if(v_heap[2*p+1+core].line_tec > v_heap[2*p + 2].line_tec){
                        std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                        heapify(2*p+2+core);
                    }
                    else{
                        if(v_heap[2*p+1+core].broker < v_heap[2*p + 2].broker){
                            std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                            heapify(2*p+1+core);
                        }
                        else{
                            std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                            heapify(2*p+2+core);
                        }
                    }
                }
            }
            else{
                if(ls < rs){ // ls matches with top
                    if(v_heap[2*p+1+core].line_tec < v_heap[p + core].line_tec){
                        std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                        heapify(2*p+1+core);
                    }
                    else if (v_heap[2*p+1+core].line_tec == v_heap[p + core].line_tec){
                        if(v_heap[2*p+1+core].broker < v_heap[p + core].broker){
                            std::swap(v_heap[p + core],v_heap[2*p+1 + core]);
                            heapify(2*p+1+core);
                        }
                    }
                    else{
                        // do nth
                    }
                }
                if(ls > rs){ // rs matches with top
                    if(v_heap[2*p+2+core].line_tec < v_heap[p + core].line_tec){
                        std::swap(v_heap[p + core],v_heap[2*p+2 + core]);
                        heapify(2*p+2+core);
                    }
                    else if (v_heap[2*p+2+core].line_tec == v_heap[p + core].line_tec){
                        if(v_heap[2*p+2+core].broker < v_heap[p + core].broker){
                            std::swap(v_heap[p + core],v_heap[2*p+2 + core]);
                            heapify(2*p+2+core);
                        }
                    }
                    else{
                        // do nth
                    }
                }
                if(rs == ls){ // 

                    // will write it
                    //std::cout<<"tec1";
                    if(v_heap[2*p+1+core].line_tec < v_heap[2*p + 2].line_tec){
                        if(v_heap[p+core].line_tec < v_heap[2*p + 1].line_tec){
                            // do nth
                        }
                        else if(v_heap[p+core].line_tec > v_heap[2*p + 1].line_tec){
                            std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                            heapify(2*p+1+core);
                        }
                        else{
                            if(v_heap[p+core].broker < v_heap[2*p + 1].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                                heapify(2*p+1+core);
                            }
                        }
                    }
                    else if(v_heap[2*p+1+core].line_tec > v_heap[2*p + 2].line_tec){
                        if(v_heap[p+core].line_tec < v_heap[2*p + 2].line_tec){
                            // do nth
                        }
                        else if(v_heap[p+core].line_tec > v_heap[2*p + 2].line_tec){
                            std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                            heapify(2*p+2+core);
                        }
                        else{
                            if(v_heap[p+core].broker < v_heap[2*p + 2].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                                heapify(2*p+2+core);
                            }
                        }
                    }
                    else{ // niche dono ka time stamp same hai

                        // will do final
                        //std::cout<<"tecf1";
                        if(v_heap[p+core].line_tec == v_heap[2*p + 2].line_tec){ // time stamps hai
                            if(v_heap[p+core].broker > v_heap[2*p + 2].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                                heapify(2*p+2+core);
                            }
                            else if(v_heap[p+core].broker > v_heap[2*p + 1].broker){
                                std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                                heapify(2*p+1+core);
                            }
                            else{
                                // do nth
                            }
                        }
                        else{
                            if(v_heap[p+core].line_tec > v_heap[2*p + 2].line_tec){
                                if(v_heap[p+core].broker > v_heap[2*p + 2].broker){
                                    std::swap(v_heap[p + core] ,v_heap[2*p+2 + core]);
                                    heapify(2*p+2+core);
                                }
                                else if(v_heap[p+core].broker > v_heap[2*p + 1].broker){
                                    std::swap(v_heap[p + core] ,v_heap[2*p+1 + core]);
                                    heapify(2*p+1+core);
                                }
                                else{
                                    // do nth
                                }
                            }
                        }
                        
                    }
                    
                }
            }

        }

        return;
    }

    void delete_timeouts_zeros(){
        return;
    }

    void delete_top(){
        
        Trade_ last = v_heap[v_heap.size() - 1];
        v_heap[0] = last;
        v_heap.pop_back();

        heapify(0);

    }

    Trade_* top(int current_time){

        if(v_heap.size() == 0){
            // std::cout<<"hello"<<std::endl;
            // Trade_* trade = new Trade_(false , 0 , 0 , 0 , 0 , "blank"); 
            // return trade ; // when  quntitity returened is zero ... means no valid trade is existing
            return nullptr;
        }

        while(current_time > v_heap[0].end){
            delete_top();
            if(v_heap.size() == 0){
                // Trade_* trade = new Trade_(false , 0 , 0 , 0 , 0 , "blank");
                // return trade ; // when  quntitity returened is zero ... means no valid trade is existing
                return nullptr;
            }
        }

        Trade_* trade_pointer = new Trade_(false , 0 , 0 , 0 , 0 , "blank" , 0);
        *trade_pointer =  v_heap[0];
        return trade_pointer;
        //return nullptr;
    }

    void edit_top(int new_quantity){
        if(new_quantity == 0){
            delete_top();
        }
        else{
            v_heap[0].quantity = new_quantity;
        }
    }

};


#endif