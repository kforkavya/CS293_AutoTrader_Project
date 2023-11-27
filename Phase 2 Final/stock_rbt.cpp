#ifndef STOCK_RBT_H
#define STOCK_RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include "trade_heap.cpp"
#include "median_heap.cpp"

// Structure to represent each
// node in a stock red-black tree
struct node {
	std::string company_name; // Company of the stock
    Insider_Heap buy_heap;
    Insider_Heap sell_heap;
    std::vector<std::pair<int , int >> store; // price quantity
    double median;
    bool trade_happened;  // to check kabse ye dekhna hai
	int color; // 1-red, 0-black
	node* parent; // parent
	node* right; // right-child
	node* left; // left child
    M_Heap* heap_max;
    M_Heap* heap_min;
    //node(std::string key): company_name(key) , color(1) , parent(nullptr) , right(nullptr) , left(nullptr) , trade_happened(false) {}

    node(std::string key) {
        company_name = key;
        color = 1;
        parent = nullptr;
        right = nullptr;
        left = nullptr;
        trade_happened = false;
        heap_max = new M_Heap();
        heap_max -> b = true;
        heap_min = new M_Heap();
        heap_min -> b = false;
    }

    int get_median(){
        return (int)(heap_max->top()/2.0 + heap_max->top() /2.0);
    }

    void insert_median(int price , int quantity){
        if(!trade_happened){
            trade_happened = true;
            heap_max->insert(price);
            heap_min->insert(price);
            quantity -=1;
        }
        while(quantity != 0){
            if(heap_max->heap_size() == heap_min->heap_size()){
                if(price < heap_max->top()){
                    heap_max->insert(price);
                    quantity -=1;
                }
                else{
                    heap_min->insert(price);
                    int x = heap_min->top();
                    heap_min->pop();
                    heap_max->insert(x);
                    quantity -=1;
                }
            }
            else{ /// keeping size of heap_max bigger

                if(price < heap_max->top()){
                    heap_max->insert(price);
                    quantity -=1;
                    int x = heap_max->top();
                    heap_max->pop();
                    heap_min->insert(x);
                }
                else{
                    heap_min->insert(price);
                    quantity -=1;
                }

            }
        }
    }
};

class Stock_RBT
{
    // global root for the entire tree
    node* root;

public:
    typedef node* ptr;

    // Constructor
    Stock_RBT()
    {
        root = NULL;
    }

    // Helper Update Functions
    const ptr getRoot() const
    {
        return root;
    }

    // function to perform BST insertion of a node.
    // returns a pointer to the newly inserted node
    ptr access(std::string key, bool& inserted_first_time)
    {
        ptr temp = root;
        while(temp != NULL)
        {
            if(temp->company_name == key)
                return temp;
            if(temp->company_name < key && temp->left!=NULL)
                temp = temp->left;
            else if(temp->company_name > key && temp->right!=NULL)
                temp = temp->right;
            else if(temp->company_name < key && temp->left==NULL)
            {
                inserted_first_time = true;
                ptr newNodePtr = new node(key);
                newNodePtr->parent = temp;
                temp->left = newNodePtr;
                fixup(newNodePtr);
                return newNodePtr;
            }
            else
            {
                inserted_first_time = true;
                ptr newNodePtr = new node(key);
                newNodePtr->parent = temp;
                temp->right = newNodePtr;
                fixup(newNodePtr);
                return newNodePtr;
            }
        }
        //if code reaches here, it means root itself was null
        root = new node(key);
        inserted_first_time = true;
        root->color=0;
        return root;
    }

    // Function performing right rotation
    // of the passed node
    void rightrotate(ptr loc)
    {
        node* p = loc->parent;
        loc->parent=p->parent;
        if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
        p->parent=loc;
        p->left=loc->right;
        if(loc->right!=NULL) loc->right->parent=p;
        loc->right=p;
    }

    // Function performing left rotation
    // of the passed node
    void leftrotate(ptr loc)
    {
        node* p = loc->parent;
        loc->parent=p->parent;
        if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
        p->parent=loc;
        p->right=loc->left;
        if(loc->left!=NULL) loc->left->parent=p;
        loc->left=p;
    }

    // This function fixes violations
    // caused by BST insertion
    void fixup(ptr loc)
    {
        if(loc==NULL) return;
        if(loc==root&&loc->color==0) return;
        if(loc==root&&loc->color==1)
        {
            loc->color=0; return;
        }
        if(loc->parent->color==0) return; //Checking Red-Red Violation
        //Case 1
        if(isSiblingRed(loc->parent))
        {
            loc->parent->color=0;
            loc->parent->parent->color=1;
            if(isRightChild(loc->parent)) loc->parent->parent->left->color=0;
            else loc->parent->parent->right->color=0;
            fixup(loc->parent->parent);
            return;
        }
        else
        {
            ptr p_temp=loc->parent;
            if(p_temp==root) {fixup(p_temp); return;}
            //Case 2
            if(isLeftChild(loc)&&isRightChild(loc->parent))
            {
                rightrotate(loc); fixup(p_temp); return;
            }
            else if(isLeftChild(loc->parent)&&isRightChild(loc))
            {
                leftrotate(loc); fixup(p_temp); return;
            }
            //Case 3
            else if(isLeftChild(loc)&&isLeftChild(loc->parent))
            {
                loc->parent->color=0;
                loc->parent->parent->color=1;
                rightrotate(loc->parent);
                return;
            }
            else
            {
                loc->parent->color=0;
                loc->parent->parent->color=1;
                leftrotate(loc->parent);
                return;
            }
        }
    }

    bool isRightChild(ptr y)
    {
    return (y->parent!=NULL)&&(y->parent->right==y);
    }

    bool isLeftChild(ptr y)
    {
    return (y->parent!=NULL)&&(y->parent->left==y);
    }

    bool isSibling(ptr y)
    {
        return (y->parent!=NULL)&&(y->parent->left!=NULL&&y->parent->right!=NULL);
    }

    int Sibling(ptr y)
    {
        if(isSibling(y))
            if(isRightChild(y)) return y->parent->left->color;
            else if(isLeftChild(y)) return y->parent->right->color;
        return -1;
    }

    bool isSiblingRed(ptr y)
    {
        if(isSibling(y)&&Sibling(y)==1) return true;
        return false;
    }
};


#endif