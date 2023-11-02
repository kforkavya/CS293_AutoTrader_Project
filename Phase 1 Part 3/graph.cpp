#include<string>
#include<vector>
#include<iostream>
using namespace std;

struct deal_node
{
  std::string deal_string; // Deal String
  vector<pair<string, int>> tokenised_string;
  int price; //Price for that deal
  int best_selling_price; //Best Selling Price
  bool no_best_sp;
  struct GraphNode* best_sp_pointer;
  int best_buying_price; //Best Buying Price
  bool no_best_bp;
  struct GraphNode* best_bp_pointer;
  int color; // 1-red, 0-black
  deal_node* parent; // parent
  deal_node* right; // right-child
  deal_node* left; // left child
  deal_node() {}
  deal_node(std::string key, vector<pair<string, int>> tokenised_string, int price_deal): deal_string(key), price(price_deal), tokenised_string(tokenised_string), best_selling_price(-1.0), no_best_sp(true), best_sp_pointer(NULL), best_buying_price(-1.0), no_best_bp(true), best_bp_pointer(NULL), color(1), parent(nullptr), right(nullptr), left(nullptr) {}
};

typedef deal_node* deal_ptr;

struct GraphNode{
    std::string actual_stock_structure;
    int order_no;
    deal_ptr deal_pointer;
    char tag;
    int price;
    int hash;
    vector<pair<int, GraphNode*>> adjacent_nodes;
    GraphNode* prev;
    GraphNode* next;
    GraphNode() {}
    GraphNode(deal_node* deal_pointer, int key, char tag, int price, int hash, std::string a): hash(hash), prev(NULL), next(NULL), deal_pointer(deal_pointer), order_no(key), tag(tag), price(price), actual_stock_structure(a) {}
};

typedef GraphNode *graph_ptr;

struct PathNode{
    vector<graph_ptr> path;
    int profit_till_now;
    vector<pair<string, int>> stock_structure;
    int zeroes;
    PathNode(): profit_till_now(0), zeroes(0) {}
    PathNode(PathNode* prev_path)
    {
        this->path = prev_path->path;
        this->profit_till_now = prev_path->profit_till_now;
        this->zeroes = prev_path->zeroes;
        this->stock_structure = prev_path->stock_structure;
    }
    PathNode(PathNode* prev_path, graph_ptr node)
    {
        this->path = prev_path->path;
        this->path.push_back(node);
        this->zeroes = prev_path->zeroes;
        deal_ptr deal_pointer = node->deal_pointer;
        int l1 = prev_path->stock_structure.size();
        int l2 = deal_pointer->tokenised_string.size();
        int c1 = 0, c2 = 0;
        while(c1<l1 && c2<l2)
        {
            if(prev_path->stock_structure[c1].first < deal_pointer->tokenised_string[c2].first)
            {
                this->stock_structure.push_back(prev_path->stock_structure[c1]);
                c1++;
            }
            else if(prev_path->stock_structure[c1].first > deal_pointer->tokenised_string[c2].first)
            {
                this->stock_structure.push_back(make_pair(deal_pointer->tokenised_string[c2].first, (node->tag=='s'? 1 : -1)*deal_pointer->tokenised_string[c2].second));
                c2++;
            }
            else
            {
                int new_quantity = prev_path->stock_structure[c1].second + (node->tag == 's' ? 1 : -1)*deal_pointer->tokenised_string[c2].second;
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
            this->stock_structure.push_back(make_pair(deal_pointer->tokenised_string[c2].first, (node->tag=='s'? 1 : -1)*deal_pointer->tokenised_string[c2].second));
            c2++;
        }
        this->profit_till_now = prev_path->profit_till_now + (node->tag == 'b' ? 1 : -1)*node->price;
    }
};

typedef PathNode *path_ptr;

class Graph_List {
    private:
    graph_ptr root;
    graph_ptr tail;

    public:
    Graph_List()
    {
        root=NULL;
        tail=NULL;
    }

    GraphNode* insert_order(deal_node* deal_pointer, char tag, int order_no, int price, int hash, std::string stock_structure)
    {
        GraphNode* temp = insertGraphNode(deal_pointer, order_no, tag, price, hash, stock_structure);
        if(tag == 's')
            deal_pointer->best_bp_pointer = temp;
        else
            deal_pointer->best_sp_pointer = temp;
        return temp;
    }

    GraphNode* insertGraphNode(deal_node* deal_pointer, int order_no, char tag, int price, int hash, std::string stock_structure)
    {
        GraphNode* temp = new GraphNode(deal_pointer, order_no, tag, price, hash, stock_structure);
        if(root==NULL)
        {
            root=tail=temp;
        }
        else
        {
            graph_ptr iterator = root;
            while(iterator != NULL)
            {
                temp->adjacent_nodes.push_back(make_pair(iterator->order_no, iterator));
                iterator=iterator->next;
            }
            tail->next = temp;
            temp->prev = tail;
            tail = temp;
        }
        return temp;
    }

    void deleteGraphNode(graph_ptr thisnode)
    {
        int order_no = thisnode->order_no;
        GraphNode* temp = tail;
        while(temp->order_no > order_no)
        {
            delete_adjacent_node(temp, order_no);
            temp = temp->prev;
        }
        //now let's delete the node from the vertex list
        if(root==tail)
        {
            free(thisnode);
            root=tail=NULL;
            return;
        }
        if(root==temp)
        {
            root=temp->next;
            root->prev=NULL;
            free(thisnode);
            return;
        }
        if(tail==temp)
        {
            tail=temp->prev;
            tail->next=NULL;
            free(thisnode);
            return;
        }
        temp->next->prev = temp->prev;
        temp->prev->next = temp->next;
        free(thisnode);
    }

    void delete_adjacent_node(graph_ptr thisnode, int order_no_to_be_deleted)
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

    void delete_best_sp_order(deal_ptr deal_pointer)
    {
        //cout<<"Deleting : "<<deal_pointer->best_sp_pointer->actual_stock_structure<<" "<<deal_pointer->best_sp_pointer->price<<endl;
        deleteGraphNode(deal_pointer->best_sp_pointer);
        deal_pointer->best_sp_pointer=NULL;
    }

    void delete_best_bp_order(deal_ptr deal_pointer)
    {
        deleteGraphNode(deal_pointer->best_bp_pointer);
        deal_pointer->best_bp_pointer=NULL;
    }

    void find_max_arbitrage(graph_ptr top_graph_ptr, vector<graph_ptr>& max_arbitrage_lane, int& max_profit)
    {
        max_profit = 0;
        vector<pair<path_ptr, graph_ptr>> my_stack;
        path_ptr temp = new PathNode();
        my_stack.push_back(make_pair(temp, top_graph_ptr));
        while(my_stack.size() > 0)
        {
            path_ptr prev_path = my_stack[my_stack.size()-1].first;
            graph_ptr curr_node = my_stack[my_stack.size()-1].second;
            path_ptr curr_path = new PathNode(prev_path, curr_node);
            my_stack.pop_back();
            //Checking if arbitrage
            if(curr_path->zeroes == curr_path->stock_structure.size() && curr_path->stock_structure.size()!=0)
            {
                if(curr_path->profit_till_now > max_profit)
                {
                    max_arbitrage_lane = curr_path->path;
                    max_profit = curr_path->profit_till_now;
                }
            }
            else
            {
                for(int i = curr_node->adjacent_nodes.size()-1; i>=0; i--)
                {
                    path_ptr temp = new PathNode(curr_path);
                    my_stack.push_back(make_pair(temp, curr_node->adjacent_nodes[i].second));
                }
                free(curr_path);
            }
            free(prev_path);
        }
    }
};

