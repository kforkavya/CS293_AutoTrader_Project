#include "stock_rbt_prime.h"
using ptr_S = Stock_RBT_Prime::ptr;

// ---------------------------------------

ptr_S Stock_RBT_Prime::access_null(std::string key, bool& inserted_first_time)
{
    ptr_S temp = root;
    while(temp != NULL)
    {
        if(temp->company_name == key)
            return temp;
        if(temp->company_name < key && temp->left!=NULL)
            temp = temp->left;
        else if(temp->company_name > key && temp->right!=NULL)
            temp = temp->right;
        else
        {
            return nullptr;
        }
    }
    // if code reaches here, it means root itself was null
    // root = new node_S(key);
    // inserted_first_time = true;
    // root->color=0;
    // return root;
    return nullptr;
}

// Function performing right rotation
// of the passed node_S

// Function performing left rotation
// of the passed node_S

// This function fixes violations
// caused by RBT insertion
