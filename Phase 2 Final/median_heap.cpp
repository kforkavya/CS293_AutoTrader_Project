#ifndef M_HEAP_H
#define M_HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#define INF 10000000 // 7-zeros 


class M_Heap{

    public:

    int b;

    std::vector<int> m_heap;

    void insert(int trade_pointer){

        m_heap.push_back(trade_pointer);
        int index = m_heap.size() - 1;

        if(b == true){ // b == true is max heap
            while (index > 0) {
                int parent = (index - 1)/2;

                if (m_heap[parent] < m_heap[index]){
                    std::swap(m_heap[parent], m_heap[index]);
                    index = parent;
                }
                else break;
            }
        }

        else{
            while (index > 0) {
                int parent = (index - 1) / 2;

                if (m_heap[parent] > m_heap[index]) {
                    std::swap(m_heap[parent], m_heap[index]);
                    index = parent;
                }
                else break;
            }
        }

        return;
    }

    void heapify(int p){ 

        if(b == true){

            int core = 0;
            int l = 0;
            int r = m_heap.size() -1;
            r -=l;
            p-=l;
            l = 0;
            int rs,ls;
            if(2*p+1>r) ls = -1000000;
            else ls = (m_heap[2*p +1 + core]);
            if(2*p+2>r) rs = -1000000;
            else rs = (m_heap[2*p +2 + core]);

            if((m_heap[p + core]) > std::max(ls , rs)) return;
            else{
                if(ls > rs){ 
                    std::swap(m_heap[p + core],m_heap[2*p+1 + core]);
                    heapify(2*p+1+core);
                }
                if(rs >= ls){ 
                    std::swap(m_heap[p + core] ,m_heap[2*p+2 + core]);
                    heapify(2*p+2+core);
                }
            }
        }


        else{

            int core = 0;
            int l = 0;
            int r = m_heap.size() -1;
            r -=l;
            p-=l;
            l = 0;
            int rs,ls;
            if(2*p+1>r)ls = 1000000;
            else ls = (m_heap[2*p +1 + core]);
            if(2*p+2>r)rs = 1000000;
            else rs = (m_heap[2*p +2 + core]);

            if((m_heap[p + core]) < std::min(ls , rs)) return;
            else{
                if(ls > rs){ 
                    std::swap(m_heap[p + core],m_heap[2*p+2 + core]);
                    heapify(2*p+2+core);
                }
                if(rs >= ls){ 
                    std::swap(m_heap[p + core] ,m_heap[2*p+1 + core]);
                    heapify(2*p+1+core);
                }
            }

        }

        return;
    }

    int top(){

        if(m_heap.size() == 0){
            // std::cout<<"hello"<<std::endl;
            // Trade_* trade = new Trade_(false , 0 , 0 , 0 , 0 , "blank"); 
            // return trade ; // when  quntitity returened is zero ... means no valid trade is existing
            return -1;
        }

        return m_heap[0];
    }

    int heap_size(){
        return m_heap.size();
    }

    void pop(){
        int x = m_heap.size();
        int val = m_heap[x-1];
        m_heap.erase(m_heap.end()-1);
        m_heap[0] = val;
        heapify(0);
    }

};


#endif