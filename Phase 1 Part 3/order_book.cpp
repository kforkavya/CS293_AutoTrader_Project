#include<string>
#include<vector>
#include<iostream>
using namespace std;

struct order_graph_node
{
  struct order_book_node* stock_ptr;
  string actual_stock_structure;
  char tag;
  int quantity;
  int order_no;
  vector<pair<int, order_graph_node*> > adjacent_nodes;
  order_graph_node* prev;
  order_graph_node* next;
  order_graph_node(order_book_node* stock_ptr, char tag, int quantity, int order_no, string key) : stock_ptr(stock_ptr), tag(tag), quantity(quantity), order_no(order_no), actual_stock_structure(key) {}
};

typedef order_graph_node* order_graph_ptr;

struct order_book_node
{
  std::string identity_string; // String that helps me in searching where this node lies in the RBT
  // It is basically Sorted Stock Structure concatenated with price in it.
  vector<pair<string, int>> tokenised_string;
  int price; // Price for that order
  int hash; // Original Hash
  vector<pair<int, order_graph_node*> > order_list; // List of Orders
  char tag; // Character for 's' or 'b' tag
  int color; // 1-red, 0-black
  order_book_node* parent; // parent in RBT
  order_book_node* right; // right-child in RBT
  order_book_node* left; // left child in RBT
  order_book_node() {}
  order_book_node(string key, vector<pair<string, int>> tokenised_string, int price, int hash): identity_string(key), tokenised_string(tokenised_string), price(price), parent(NULL), color(1), hash(hash) {}
};

typedef order_book_node* order_book_ptr;

class Order_Book_RBT {
   private:
  order_book_ptr root;
  order_book_ptr TNULL;

  void initializeNULLNode(order_book_ptr node, order_book_ptr parent) {
    node->identity_string = "";
    node->parent = parent;
    node->left = nullptr;
    node->right = nullptr;
    node->color = 0;
  }

  // Preorder
  void preOrderHelper(order_book_ptr node) {
    if (node != TNULL) {
      cout << node->identity_string << " ";
      preOrderHelper(node->left);
      preOrderHelper(node->right);
    }
  }

  // Inorder
  void inOrderHelper(order_book_ptr node) {
    if (node != TNULL) {
      inOrderHelper(node->left);
      cout << node->identity_string << " ";
      inOrderHelper(node->right);
    }
  }

  // Post order
  void postOrderHelper(order_book_ptr node) {
    if (node != TNULL) {
      postOrderHelper(node->left);
      postOrderHelper(node->right);
      cout << node->identity_string << " ";
    }
  }

  order_book_ptr searchTreeHelper(order_book_ptr node, string key) {
    if (node == TNULL || key == node->identity_string) {
      return node;
    }

    if (key < node->identity_string) {
      return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
  }

  // For balancing the tree after deletion
  void deleteFix(order_book_ptr x) {
    order_book_ptr s;
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

  void rbTransplant(order_book_ptr u, order_book_ptr v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }

  void deleteNodeHelper(order_book_ptr node, order_book_ptr thisnode) {
    order_book_ptr z = thisnode;
    order_book_ptr x, y;
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
void insertFix(order_book_ptr k) {
    order_book_ptr u;
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

  void printHelper(order_book_ptr root, string indent, bool last) {
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
      cout << root->identity_string << "(" << sColor << ")" << endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

   public:
  Order_Book_RBT() {
    TNULL = new order_book_node;
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

  order_book_ptr searchTree(string k) {
    return searchTreeHelper(this->root, k);
  }

  order_book_ptr minimum(order_book_ptr node) {
    while (node->left != TNULL) {
      node = node->left;
    }
    return node;
  }

  order_book_ptr maximum(order_book_ptr node) {
    while (node->right != TNULL) {
      node = node->right;
    }
    return node;
  }

  order_book_ptr successor(order_book_ptr x) {
    if (x->right != TNULL) {
      return minimum(x->right);
    }

    order_book_ptr y = x->parent;
    while (y != TNULL && x == y->right) {
      x = y;
      y = y->parent;
    }
    return y;
  }

  order_book_ptr predecessor(order_book_ptr x) {
    if (x->left != TNULL) {
      return maximum(x->left);
    }

    order_book_ptr y = x->parent;
    while (y != TNULL && x == y->left) {
      x = y;
      y = y->parent;
    }

    return y;
  }

  void leftRotate(order_book_ptr x) {
    order_book_ptr y = x->right;
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

  void rightRotate(order_book_ptr x) {
    order_book_ptr y = x->left;
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
  void access(string key, order_book_ptr& thisnode, bool& inserted_first_time, vector<pair<string, int>> tokenised_string, int price, int hash) 
  {
    order_book_ptr temp = root;
    while(temp != TNULL)
    {
        if(temp->identity_string == key)
        {
            thisnode = temp;
            inserted_first_time = false;
            return;
        }
        if(temp->identity_string > key && temp->left!=TNULL)
            temp = temp->left;
        else if(temp->identity_string < key && temp->right!=TNULL)
            temp = temp->right;
        else if(temp->identity_string > key && temp->left==TNULL)
        {
            inserted_first_time = true;
            order_book_ptr newNodePtr = new order_book_node(key, tokenised_string, price, hash);
            newNodePtr->left = TNULL;
            newNodePtr->right = TNULL;
            newNodePtr->parent = temp;
            temp->left = newNodePtr;
            fixup(newNodePtr);
            thisnode = newNodePtr;
            return;
        }
        else if(temp->identity_string < key && temp->right==TNULL)
        {
            inserted_first_time = true;
            order_book_ptr newNodePtr = new order_book_node(key, tokenised_string, price, hash);
            newNodePtr->left = TNULL;
            newNodePtr->right = TNULL;
            newNodePtr->parent = temp;
            temp->right = newNodePtr;
            fixup(newNodePtr);
            thisnode = newNodePtr;
            return;
        }
    }
    //if code reaches here, it means root itself was null
    root = new order_book_node(key, tokenised_string, price, hash);
    root->left = TNULL;
    root->right = TNULL;
    inserted_first_time = true;
    root->color=0;
    thisnode = root;
  }

  order_book_ptr getRoot() {
    return this->root;
  }

  void deleteNode(order_book_ptr thisnode) {
    deleteNodeHelper(this->root, thisnode);
  }

  void printTree() {
    if (root) {
      printHelper(this->root, "", true);
    }
  }

  void fixup(order_book_ptr loc)
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
        order_book_ptr p_temp=loc->parent;
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

void rightrotate(order_book_ptr loc)
{
    order_book_node* p = loc->parent;
    loc->parent=p->parent;
    if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
    p->parent=loc;
    p->left=loc->right;
    if(loc->right!=TNULL) loc->right->parent=p;
    loc->right=p;
}

void leftrotate(order_book_ptr loc)
{
    order_book_node* p = loc->parent;
    loc->parent=p->parent;
    if(p->parent==NULL) root=loc; else if(isRightChild(p)) p->parent->right=loc; else p->parent->left=loc;
    p->parent=loc;
    p->right=loc->left;
    if(loc->left!=TNULL) loc->left->parent=p;
    loc->left=p;
}
bool isRightChild(order_book_ptr y)
{
  return (y->parent!=NULL)&&(y->parent->right==y);
}

bool isLeftChild(order_book_ptr y)
{
  return (y->parent!=NULL)&&(y->parent->left==y);
}

bool isSibling(order_book_ptr y)
{
    return (y->parent!=NULL)&&(y->parent->left!=NULL&&y->parent->right!=NULL);
}

int Sibling(order_book_ptr y)
{
    if(isSibling(y))
        if(isRightChild(y)) return y->parent->left->color;
        else if(isLeftChild(y)) return y->parent->right->color;
    return -1;
}

bool isSiblingRed(order_book_ptr y)
{
    if(isSibling(y)&&Sibling(y)==1) return true;
    return false;
}
};

struct NewPathNode{
    vector<pair<order_graph_ptr, int> > path;
    int profit_till_now;
    vector<pair<string, int>> stock_structure;
    int zeroes;
    NewPathNode(): profit_till_now(0) {}
    NewPathNode(NewPathNode* prev_path)
    {
        this->path = prev_path->path;
        this->profit_till_now = prev_path->profit_till_now;
        this->stock_structure = prev_path->stock_structure;
        this->zeroes = prev_path->zeroes;
    }
    NewPathNode(NewPathNode* prev_path, order_graph_ptr node, int quantity)
    {
        this->path = prev_path->path;
        this->path.push_back({node, quantity});
        this->zeroes = prev_path->zeroes;
        int l1 = prev_path->stock_structure.size();
        int l2 = node->stock_ptr->tokenised_string.size();
        int c1 = 0, c2 = 0;
        while(c1<l1 && c2<l2)
        {
            if(prev_path->stock_structure[c1].first < node->stock_ptr->tokenised_string[c2].first)
            {
                this->stock_structure.push_back(prev_path->stock_structure[c1]);
                c1++;
            }
            else if(prev_path->stock_structure[c1].first > node->stock_ptr->tokenised_string[c2].first)
            {
                this->stock_structure.push_back(make_pair(node->stock_ptr->tokenised_string[c2].first, quantity*(node->tag=='s'? 1 : -1)*node->stock_ptr->tokenised_string[c2].second));
                c2++;
            }
            else
            {
                int new_quantity = prev_path->stock_structure[c1].second + quantity*(node->tag == 's' ? 1 : -1)*node->stock_ptr->tokenised_string[c2].second;
                if(prev_path->stock_structure[c1].second==0) this->zeroes--;
                else if(new_quantity==0) this->zeroes++;
                this->stock_structure.push_back(make_pair(prev_path->stock_structure[c1].first, new_quantity));
                c1++; c2++;
            }
        }
        while(c1<l1)
            this->stock_structure.push_back(prev_path->stock_structure[c1++]);
        while(c2<l2)
        {
            this->stock_structure.push_back(make_pair(node->stock_ptr->tokenised_string[c2].first, quantity*(node->tag=='s'? 1 : -1)*node->stock_ptr->tokenised_string[c2].second));
            c2++;
        }
        this->profit_till_now = prev_path->profit_till_now + quantity*(node->tag == 'b' ? 1 : -1)*node->stock_ptr->price;
    }
};

typedef NewPathNode *new_path_ptr;

class New_Graph_List {
    private:
    order_graph_ptr root;
    public:
    order_graph_ptr tail;

    public:
    New_Graph_List()
    {
        root=NULL;
        tail=NULL;
    }

    void insert_order_in_graph(order_graph_ptr order_pointer)
    {
        if(root==NULL)
        {
            root=tail=order_pointer;
        }
        else
        {
            order_graph_ptr iterator = root;
            while(iterator != NULL)
            {
                //cout<<"Here"<<endl;
                //cout << iterator->identity_string << endl;
                order_pointer->adjacent_nodes.push_back(make_pair(iterator->order_no, iterator));
                //cout<<"Here2"<<endl;
                iterator=iterator->next;
            }
            tail->next = order_pointer;
            order_pointer->prev = tail;
            order_pointer->next = NULL;
            tail = order_pointer;
            //cout << "Inserted Successfully" << endl;
        }
    }

    void delete_order_from_graph(order_graph_ptr thisnode)
    {
        int order_no = thisnode->order_no;
        order_graph_node* temp = tail;
        while(temp!=NULL&&temp->order_no > order_no)
        {
            delete_adjacent_node(temp, order_no);
            temp = temp->prev;
        }
        //now let's delete the node from the vertex list
        if(root==tail)
        {
            temp->next = temp->prev = NULL;
            root=tail=NULL;
            return;
        }
        if(root==temp)
        {
            root=temp->next;
            temp->next = NULL;
            root->prev=NULL;
            return;
        }
        if(tail==temp)
        {
            tail=temp->prev;
            temp->prev=NULL;
            tail->next=NULL;
            return;
        }
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        temp->prev = temp->next = NULL;
    }

    void delete_adjacent_node(order_graph_ptr thisnode, int order_no_to_be_deleted)
    {
        int l = 0, u = thisnode->adjacent_nodes.size()-1, m = -1;
        while(l<=u)
        {
            m = (l+u)/2;
            if(thisnode->adjacent_nodes[m].first==order_no_to_be_deleted)
                break;
            else if(thisnode->adjacent_nodes[m].first<order_no_to_be_deleted)
                l = m+1;
            else
                u = m-1;
        }
        if(m==-1) return;
        //now delete from adjacency vector
        thisnode->adjacent_nodes.erase(thisnode->adjacent_nodes.begin() + m);
    }

    void find_max_arbitrage(order_graph_ptr top_order_book_ptr, vector<pair<order_graph_ptr, int> >& max_arbitrage_lane, int& max_profit)
    {
        max_profit = 0;
        vector<pair<new_path_ptr, pair<order_graph_ptr, int>>> my_stack;
        new_path_ptr temp = new NewPathNode();
        my_stack.push_back({temp, {top_order_book_ptr, top_order_book_ptr->quantity}});
        while(my_stack.size() > 0)
        {
            new_path_ptr prev_path = my_stack[my_stack.size()-1].first;
            order_graph_ptr curr_node = my_stack[my_stack.size()-1].second.first;
            int curr_quantity = my_stack[my_stack.size()-1].second.second;
            //cout<<"Stock = "<<curr_node->actual_stock_structure<<" Price = "<<curr_node->stock_ptr->price<<" Quantity = "<<curr_quantity<<" Tag = "<<curr_node->tag<<endl;
            //cout<<my_stack.size()<<endl;
            new_path_ptr curr_path = new NewPathNode(prev_path, curr_node, curr_quantity);
            my_stack.pop_back();
            //Checking if arbitrage
            if(curr_path->zeroes == curr_path->stock_structure.size() && curr_path->stock_structure.size()!=0)
            {
                if(curr_path->profit_till_now > max_profit)
                {
                    //cout << "Max Arbitrage Found !" << endl;
                    max_arbitrage_lane = curr_path->path;
                    max_profit = curr_path->profit_till_now;
                }
            }
            if(curr_quantity - 1 > 0)
                my_stack.push_back({prev_path, {curr_node, curr_quantity - 1}});
            else
                free(prev_path);
            if(curr_quantity - 1 >= 0)
            {
                for(int i = 0; i<curr_node->adjacent_nodes.size(); i++)
                {
                    new_path_ptr temp = new NewPathNode(curr_path);
                    my_stack.push_back({temp, {curr_node->adjacent_nodes[i].second, curr_node->adjacent_nodes[i].second->quantity}});
                }
            }
            free(curr_path);
        }
    }
};
