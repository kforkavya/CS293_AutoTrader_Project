#ifndef STOCK_RBTP_H
#define STOCK_RBTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>

// Structure to represent each
// node_S in a stock red-black tree
struct node_S {
	std::string company_name; // Company of the stock

    int sell;
    int buy;
    int count_sell;// sell - 
    int count_buy;
    
	int color; // 1-red, 0-black
	node_S* parent; // parent
	node_S* right; // right-child
	node_S* left; // left child
    node_S(std::string key): company_name(key) ,count_sell(0) ,count_buy(0), buy(0) , sell(0),  color(1) , parent(nullptr) , right(nullptr) , left(nullptr){}
};

class Stock_RBT_Prime
{
    // global root for the entire tree
    node_S* root;

public:
    typedef node_S* ptr;

    // Constructor
    Stock_RBT_Prime()
    {
        root = NULL;
    }

    // Helper Update Functions
    const ptr getRoot() const
    {
        return root;
    }

    // function to perform BST insertion of a node_S.
    // returns a pointer to the newly inserted node_S
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
                ptr newNodeptr = new node_S(key);
                newNodeptr->parent = temp;
                temp->left = newNodeptr;
                fixup(newNodeptr);
                return newNodeptr;
            }
            else
            {
                inserted_first_time = true;
                ptr newNodeptr = new node_S(key);
                newNodeptr->parent = temp;
                temp->right = newNodeptr;
                fixup(newNodeptr);
                return newNodeptr;
            }
        }
        //if code reaches here, it means root itself was null
        root = new node_S(key);
        inserted_first_time = true;
        root->color=0;
        return root;
    }

    ptr access_null(std::string key, bool& inserted_first_time);

    // Function performing right rotation
    // of the passed node_S
    void rightrotate(ptr loc)
    {
        node_S* p = loc->parent;
        loc->parent=p->parent;
        if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
        p->parent=loc;
        p->left=loc->right;
        if(loc->right!=NULL) loc->right->parent=p;
        loc->right=p;
    }

    // Function performing left rotation
    // of the passed node_S
    void leftrotate(ptr loc)
    {
        node_S* p = loc->parent;
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