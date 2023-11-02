#include<string>
#include<vector>
#include<iostream>
using namespace std;
struct deal_node
{
  std::string deal_string; // Deal String
  string actual_stock_structure;
  vector<pair<string, int>> tokenised_string;
  int price; //Price for that deal
  int best_selling_price; //Best Selling Price
  bool no_best_sp;
  GraphNode* best_sp_pointer;
  int best_buying_price; //Best Buying Price
  bool no_best_bp;
  GraphNode* best_bp_pointer;
  int color; // 1-red, 0-black
  deal_node* parent; // parent
  deal_node* right; // right-child
  deal_node* left; // left child
  deal_node() {}
  deal_node(std::string key): deal_string(key), price(-1.0), best_selling_price(-1.0), no_best_sp(true), best_sp_pointer(NULL), best_buying_price(-1.0), no_best_bp(true), best_bp_pointer(NULL), color(1), parent(nullptr), right(nullptr), left(nullptr) {}
};

struct GraphNode{
    int order_no;
    graph_ptr deal_pointer;
    char tag;
    int price;
    vector<pair<int, GraphNode*>> adjacent_nodes;
    int color; // 1-red, 0-black
    GraphNode* parent; // parent
    GraphNode* right; // right-child
    GraphNode* left; // left child
    GraphNode() {}
    GraphNode(int key): color(1), parent(NULL), right(NULL), left(NULL), deal_pointer(NULL), order_no(key) {}
};

typedef GraphNode *graph_ptr;

class Graph_RBT {
   private:
  graph_ptr root;
  graph_ptr TNULL;

  // Preorder
  void preOrderHelper(graph_ptr node) {
    if (node != TNULL) {
      cout << node->order_no << " ";
      preOrderHelper(node->left);
      preOrderHelper(node->right);
    }
  }

  // Inorder
  void inOrderHelper(graph_ptr node) {
    if (node != TNULL) {
      inOrderHelper(node->left);
      cout << node->order_no << " ";
      inOrderHelper(node->right);
    }
  }

  // Post order
  void postOrderHelper(graph_ptr node) {
    if (node != TNULL) {
      postOrderHelper(node->left);
      postOrderHelper(node->right);
      cout << node->order_no << " ";
    }
  }

  graph_ptr searchTreeHelper(graph_ptr node, int key) {
    if (node == TNULL || key == node->order_no) {
      return node;
    }

    if (key < node->order_no) {
      return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
  }

  // For balancing the tree after deletion
  void deleteFix(graph_ptr x) {
    graph_ptr s;
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

  void rbTransplant(graph_ptr u, graph_ptr v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }

  void deleteNodeHelper(graph_ptr node, graph_ptr thisnode) {
    graph_ptr z = thisnode;
    graph_ptr x, y;

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
void insertFix(graph_ptr k) {
    graph_ptr u;
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

  void printHelper(graph_ptr root, string indent, bool last) {
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
      cout << root->order_no << "(" << sColor << ")" << endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

   public:
  Graph_RBT() {
    TNULL = new GraphNode;
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

  graph_ptr searchTree(int k) {
    return searchTreeHelper(this->root, k);
  }

  graph_ptr minimum(graph_ptr node) {
    while (node->left != TNULL) {
      node = node->left;
    }
    return node;
  }

  graph_ptr maximum(graph_ptr node) {
    while (node->right != TNULL) {
      node = node->right;
    }
    return node;
  }

  graph_ptr successor(graph_ptr x) {
    if (x->right != TNULL) {
      return minimum(x->right);
    }

    graph_ptr y = x->parent;
    while (y != TNULL && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  graph_ptr predecessor(graph_ptr x) {
    if (x->left != TNULL) {
      return maximum(x->left);
    }

    graph_ptr y = x->parent;
    while (y != TNULL && x == y->left) {
      x = y;
      y = y->parent;
    }

    return y;
  }

  void leftRotate(graph_ptr x) {
    graph_ptr y = x->right;
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

  void rightRotate(graph_ptr x) {
    graph_ptr y = x->left;
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
  void access(int key, graph_ptr& thisnode, bool& inserted_first_time) {
    graph_ptr node = new GraphNode(key);

    graph_ptr y = nullptr;
    graph_ptr x = this->root;

    while (x != TNULL) {
      y = x;
      if (node->order_no < x->order_no) {
        x = x->left;
      } else if (node->order_no > x->order_no) {
        x = x->right;
      }
      else{
        free(node);
        thisnode = x;
        inserted_first_time = false;
        return;
      }
    }

    node->parent = y;
    if (y == nullptr) {
      root = node;
    } else if (node->order_no < y->order_no) {
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
    inserted_first_time = true;
    insertFix(node);
  }

  graph_ptr getRoot() {
    return this->root;
  }

  void deleteNode(graph_ptr thisnode) {
    deleteNodeHelper(this->root, thisnode);
  }

  void printTree() {
    if (root) {
      printHelper(this->root, "", true);
    }
  }
};
