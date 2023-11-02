// Implementing Red-Black Tree in C++

#include <iostream>
using namespace std;

struct deal_node {
	std::string deal_string; // Deal String
    int price_estimator; //Price Estimator for this company stock (Last traded price)
    int best_selling_price; //Best Selling Price
    bool no_best_sp;
    int best_buying_price; //Best Buying Price
    bool no_best_bp;
	int color; // 1-red, 0-black
	deal_node* parent; // parent
	deal_node* right; // right-child
	deal_node* left; // left child
    deal_node() {}
    deal_node(std::string key): deal_string(key), price_estimator(-1.0), best_selling_price(-1.0), no_best_sp(true), best_buying_price(-1.0), no_best_bp(true), color(1), parent(nullptr), right(nullptr), left(nullptr) {}
};

typedef deal_node *deal_ptr;

class Deal_RBT {
   private:
  deal_ptr root;
  deal_ptr TNULL;

  void initializeNULLNode(deal_ptr node, deal_ptr parent) {
    node->deal_string = "";
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = 0;
  }

  // Preorder
  void preOrderHelper(deal_ptr node) {
    if (node != TNULL) {
      cout << node->deal_string << " ";
      preOrderHelper(node->left);
      preOrderHelper(node->right);
    }
  }

  // Inorder
  void inOrderHelper(deal_ptr node) {
    if (node != TNULL) {
      inOrderHelper(node->left);
      cout << node->deal_string << " ";
      inOrderHelper(node->right);
    }
  }

  // Post order
  void postOrderHelper(deal_ptr node) {
    if (node != TNULL) {
      postOrderHelper(node->left);
      postOrderHelper(node->right);
      cout << node->deal_string << " ";
    }
  }

  deal_ptr searchTreeHelper(deal_ptr node, string key) {
    if (node == TNULL || key == node->deal_string) {
      return node;
    }

    if (key < node->deal_string) {
      return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
  }

  // For balancing the tree after deletion
  void deleteFix(deal_ptr x) {
    deal_ptr s;
    while (x != root && x->color == 0) {
      if (x == x->parent->left) {
        s = x->parent->right;
        if (s->color == 1) {
          s->color = 0;
          x->parent->color = 1;
          leftRotate(x->parent);
          s = x->parent->right;
        }

        if (s->left->color == 0 && s->right->color == 0) {
          s->color = 1;
          x = x->parent;
        } else {
          if (s->right->color == 0) {
            s->left->color = 0;
            s->color = 1;
            rightRotate(s);
            s = x->parent->right;
          }

          s->color = x->parent->color;
          x->parent->color = 0;
          s->right->color = 0;
          leftRotate(x->parent);
          x = root;
        }
      } else {
        s = x->parent->left;
        if (s->color == 1) {
          s->color = 0;
          x->parent->color = 1;
          rightRotate(x->parent);
          s = x->parent->left;
        }

        if (s->right->color == 0 && s->right->color == 0) {
          s->color = 1;
          x = x->parent;
        } else {
          if (s->left->color == 0) {
            s->right->color = 0;
            s->color = 1;
            leftRotate(s);
            s = x->parent->left;
          }

          s->color = x->parent->color;
          x->parent->color = 0;
          s->left->color = 0;
          rightRotate(x->parent);
          x = root;
        }
      }
    }
    x->color = 0;
  }

  void rbTransplant(deal_ptr u, deal_ptr v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }

  void deleteNodeHelper(deal_ptr node, deal_ptr thisnode) {
    deal_ptr z = thisnode;
    deal_ptr x, y;

    if (z == TNULL) {
      cout << "Key not found in the tree" << endl;
      return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
      x = z->right;
      rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
      x = z->left;
      rbTransplant(z, z->left);
    } else {
      y = minimum(z->right);
      y_original_color = y->color;
      x = y->right;
      if (y->parent == z) {
        x->parent = y;
      } else {
        rbTransplant(y, y->right);
        y->right = z->right;
        y->right->parent = y;
      }

      rbTransplant(z, y);
      y->left = z->left;
      y->left->parent = y;
      y->color = z->color;
    }
    delete z;
    if (y_original_color == 0) {
      deleteFix(x);
    }
  }

  // For balancing the tree after insertion
void insertFix(deal_ptr k) {
    deal_ptr u;
    while (k->parent->color == 1) {
      if (k->parent == k->parent->parent->right) {
        u = k->parent->parent->left;
        if (u->color == 1) {
          u->color = 0;
          k->parent->color = 0;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->left) {
            k = k->parent;
            rightRotate(k);
          }
          k->parent->color = 0;
          k->parent->parent->color = 1;
          leftRotate(k->parent->parent);
        }
      } else {
        u = k->parent->parent->right;

        if (u->color == 1) {
          u->color = 0;
          k->parent->color = 0;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->right) {
            k = k->parent;
            leftRotate(k);
          }
          k->parent->color = 0;
          k->parent->parent->color = 1;
          rightRotate(k->parent->parent);
        }
      }
      if (k == root) {
        break;
      }
    }
    root->color = 0;
}

  void printHelper(deal_ptr root, string indent, bool last) {
    if (root != TNULL) {
      cout << indent;
      if (last) {
        cout << "R----";
        indent += "   ";
      } else {
        cout << "L----";
        indent += "|  ";
      }

      string sColor = root->color ? "RED" : "BLACK";
      cout << root->deal_string << "(" << sColor << ")" << endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

   public:
  Deal_RBT() {
    TNULL = new deal_node;
    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
  }

  void preorder() {
    preOrderHelper(this->root);
  }

  void inorder() {
    inOrderHelper(this->root);
  }

  void postorder() {
    postOrderHelper(this->root);
  }

  deal_ptr searchTree(string k) {
    return searchTreeHelper(this->root, k);
  }

  deal_ptr minimum(deal_ptr node) {
    while (node->left != TNULL) {
      node = node->left;
    }
    return node;
  }

  deal_ptr maximum(deal_ptr node) {
    while (node->right != TNULL) {
      node = node->right;
    }
    return node;
  }

  deal_ptr successor(deal_ptr x) {
    if (x->right != TNULL) {
      return minimum(x->right);
    }

    deal_ptr y = x->parent;
    while (y != TNULL && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  deal_ptr predecessor(deal_ptr x) {
    if (x->left != TNULL) {
      return maximum(x->left);
    }

    deal_ptr y = x->parent;
    while (y != TNULL && x == y->left) {
      x = y;
      y = y->parent;
    }

    return y;
  }

  void leftRotate(deal_ptr x) {
    deal_ptr y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
  }

  void rightRotate(deal_ptr x) {
    deal_ptr y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
      y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
  }

  // Inserting a node
  void insert(string key, deal_ptr& thisnode) {
    deal_ptr node = new deal_node;
    node->parent = nullptr;
    node->deal_string = key;
    node->left = TNULL;
    node->right = TNULL;
    node->color = 1;

    deal_ptr y = nullptr;
    deal_ptr x = this->root;

    while (x != TNULL) {
      y = x;
      if (node->deal_string < x->deal_string) {
        x = x->left;
      } else if (node->deal_string > x->deal_string) {
        x = x->right;
      }
      else{
        thisnode = x;
        return;
      }
    }

    node->parent = y;
    if (y == nullptr) {
      root = node;
    } else if (node->deal_string < y->deal_string) {
      y->left = node;
    } else {
      y->right = node;
    }

    if (node->parent == nullptr) {
      node->color = 0;
      return;
    }

    if (node->parent->parent == nullptr) {
      return;
    }
    thisnode = node;
    insertFix(node);
  }

  deal_ptr getRoot() {
    return this->root;
  }

  void deleteNode(deal_ptr thisnode) {
    deleteNodeHelper(this->root, thisnode);
  }

  void printTree() {
    if (root) {
      printHelper(this->root, "", true);
    }
  }
};

int main() {
  Deal_RBT bst; deal_ptr temp=NULL;
  bst.insert("11", temp);
  bst.insert("1", temp);
  bst.insert("24", temp);
  bst.insert("25", temp);
  cout<<(temp==NULL)<<endl;
  bst.printTree();
  cout << endl
     << "After deleting" << endl;
  bst.deleteNode(temp);
  bst.printTree();
}