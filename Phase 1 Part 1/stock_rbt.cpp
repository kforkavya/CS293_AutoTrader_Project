#include "stock_rbt.h"
using ptr = Stock_RBT::ptr;

Stock_RBT::Stock_RBT()
{
    root = NULL;
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

const ptr Stock_RBT::getRoot() const
{ 
	return root; 
}

ptr Stock_RBT::access(std::string key, bool& inserted_first_time)
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
void Stock_RBT::rightrotate(ptr loc)
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
void Stock_RBT::leftrotate(ptr loc)
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
// caused by RBT insertion
void Stock_RBT::fixup(ptr loc)
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
