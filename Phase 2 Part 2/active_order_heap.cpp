#include<string>
#include<vector>
using namespace std;

struct active_order_node
{
  string trader_name;
  int price; // Price for that order
  int quantity; // Quantity
  int expiration_time; // Time when it expires
  active_order_node(int price, int quantity, string name, int time): price(price), expiration_time(time), quantity(quantity), trader_name(name) {}
};

typedef active_order_node* active_order_ptr;

class max_heap
{
    vector<active_order_ptr> heap;
    int heap_size;
    public:
    max_heap()
    {
        heap_size = 0;
    }
    bool isEmpty()
    {
        return heap_size == 0;
    }
    void my_swap(int x, int y)
    {
        active_order_ptr swap = heap[x];
        heap[x] = heap[y];
        heap[y] = swap;
    }
    void insert(active_order_ptr pointer)
    {
        heap.push_back(pointer);
        heap_size++;
        int curr = heap_size-1;
        while(curr > 0)
        {
            int parent = (curr - 1)/2;
            if(heap[parent]->price >= heap[curr]->price)
                break;
            my_swap(curr, parent);
            curr = parent;
        }
    }
    active_order_ptr pop_max()
    {
        if(isEmpty()) return NULL;
        active_order_ptr result = heap[0];
        my_swap(0, heap_size-1);
        heap_size--;
        int curr = 0;
        while(curr < heap_size)
        {
            int largest = curr;
            int left = 2*curr+1;
            int right = left+1;
            if(left < heap_size)
                largest = heap[left]->price > heap[curr]->price ? left : curr;
            if(right < heap_size)
                largest = heap[right]->price > heap[largest]->price ? right : largest;
            if(curr == largest)
                break;
            my_swap(curr, largest);
            curr = largest;
        }
        return result;
    }
};

class min_heap
{
    vector<active_order_ptr> heap;
    int heap_size;
    public:
    min_heap()
    {
        heap_size = 0;
    }
    bool isEmpty()
    {
        return heap_size == 0;
    }
    void my_swap(int x, int y)
    {
        active_order_ptr swap = heap[x];
        heap[x] = heap[y];
        heap[y] = swap;
    }
    void insert(active_order_ptr pointer)
    {
        heap.push_back(pointer);
        heap_size++;
        int curr = heap_size-1;
        while(curr > 0)
        {
            int parent = (curr - 1)/2;
            if(heap[parent]->price <= heap[curr]->price)
                break;
            my_swap(curr, parent);
            curr = parent;
        }
    }
    active_order_ptr pop_min()
    {
        if(isEmpty()) return NULL;
        active_order_ptr result = heap[0];
        my_swap(0, heap_size-1);
        heap_size--;
        int curr = 0;
        while(curr < heap_size)
        {
            int smallest = curr;
            int left = 2*curr+1;
            int right = left+1;
            if(left < heap_size)
                smallest = heap[left]->price < heap[curr]->price ? left : curr;
            if(right < heap_size)
                smallest = heap[right]->price < heap[smallest]->price ? right : smallest;
            if(curr == smallest)
                break;
            my_swap(curr, smallest);
            curr = smallest;
        }
        return result;
    }
};