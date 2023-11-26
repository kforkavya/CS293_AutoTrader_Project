#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

// Structure to represent each
// node in a deal red-black tree
struct node {
	std::string company_name; // Company of the stock
    int price_estimator; //Price Estimator for this company stock (Last traded price)
    int best_selling_price; //Best Selling Price
    bool no_best_sp;
    int best_buying_price; //Best Buying Price
    bool no_best_bp;
	int color; // 1-red, 0-black
	node* parent; // parent
	node* right; // right-child
	node* left; // left child
    node(std::string key): company_name(key), price_estimator(-1.0), best_selling_price(-1.0), no_best_sp(true), best_buying_price(-1.0), no_best_bp(true), color(1), parent(nullptr), right(nullptr), left(nullptr) {}
};

class Stock_RBT
{
    // global root for the entire tree
    node* root;

public:
    typedef node* ptr;

    // Constructor
    Stock_RBT();

    // Helper Update Functions
    const ptr getRoot() const;

    // function to perform BST insertion of a node.
    // returns a pointer to the newly inserted node
    ptr access(std::string key, bool& inserted_first_time);

    // Function performing right rotation
    // of the passed node
    void rightrotate(ptr loc);

    // Function performing left rotation
    // of the passed node
    void leftrotate(ptr loc);

    // This function fixes violations
    // caused by BST insertion
    void fixup(ptr loc);
};