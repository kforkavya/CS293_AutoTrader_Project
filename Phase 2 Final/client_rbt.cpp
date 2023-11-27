#ifndef CLIENT_RBT_H
#define CLIENT_RBT_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include "stock_rbt_prime.h"

// Structure to represent each
// node_c in a stock red-black tree
struct node_c {
	std::string client_name; // Company of the stock
    
    Stock_RBT_Prime RBT;

	int color; // 1-red, 0-black
	node_c* parent; // parent
	node_c* right; // right-child
	node_c* left; // left child
    node_c(std::string key): client_name(key) , color(1) , parent(nullptr) , right(nullptr) , left(nullptr){}
};

class Client_RBT
{
    // global root for the entire tree
    node_c* root;

public:
    typedef node_c* ptr_client;

    // Constructor
    Client_RBT()
    {
        root = NULL;
    }

    // Helper Update Functions
    const ptr_client getRoot() const
    {
        return root;
    }

    // function to perform BST insertion of a node_c.
    // returns a pointer to the newly inserted node_c
    ptr_client access(std::string key, bool& inserted_first_time)
    {
        ptr_client temp = root;
        while(temp != NULL)
        {
            if(temp->client_name == key)
                return temp;
            if(temp->client_name < key && temp->left!=NULL)
                temp = temp->left;
            else if(temp->client_name > key && temp->right!=NULL)
                temp = temp->right;
            else if(temp->client_name < key && temp->left==NULL)
            {
                inserted_first_time = true;
                ptr_client newNodePtr = new node_c(key);
                newNodePtr->parent = temp;
                temp->left = newNodePtr;
                fixup(newNodePtr);
                return newNodePtr;
            }
            else
            {
                inserted_first_time = true;
                ptr_client newNodePtr = new node_c(key);
                newNodePtr->parent = temp;
                temp->right = newNodePtr;
                fixup(newNodePtr);
                return newNodePtr;
            }
        }
        //if code reaches here, it means root itself was null
        root = new node_c(key);
        inserted_first_time = true;
        root->color=0;
        return root;
    }
    // Function performing right rotation
    // of the passed node_c
    void rightrotate(ptr_client loc)
    {
        node_c* p = loc->parent;
        loc->parent=p->parent;
        if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
        p->parent=loc;
        p->left=loc->right;
        if(loc->right!=NULL) loc->right->parent=p;
        loc->right=p;
    }

    // Function performing left rotation
    // of the passed node_c
    void leftrotate(ptr_client loc)
    {
        node_c* p = loc->parent;
        loc->parent=p->parent;
        if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
        p->parent=loc;
        p->right=loc->left;
        if(loc->left!=NULL) loc->left->parent=p;
        loc->left=p;
    }

    // This function fixes violations
    // caused by BST insertion
    void fixup(ptr_client loc)
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
            ptr_client p_temp=loc->parent;
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

    bool isRightChild(ptr_client y)
    {
    return (y->parent!=NULL)&&(y->parent->right==y);
    }

    bool isLeftChild(ptr_client y)
    {
    return (y->parent!=NULL)&&(y->parent->left==y);
    }

    bool isSibling(ptr_client y)
    {
        return (y->parent!=NULL)&&(y->parent->left!=NULL&&y->parent->right!=NULL);
    }

    int Sibling(ptr_client y)
    {
        if(isSibling(y))
            if(isRightChild(y)) return y->parent->left->color;
            else if(isLeftChild(y)) return y->parent->right->color;
        return -1;
    }

    bool isSiblingRed(ptr_client y)
    {
        if(isSibling(y)&&Sibling(y)==1) return true;
        return false;
    }
};


#endif