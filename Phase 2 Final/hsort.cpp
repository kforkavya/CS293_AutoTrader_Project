#include <vector>
#include <cmath>
using std::vector;

// implement heapsort - perhaps using a heap class
void HF(vector<int>& v, int l, int r, int p = 0){ // HEAPIFY
    int core = l;
    r -=l;
    p-=l;
    l = 0;
    int rs,ls;
    if(2*p+1>r)ls = -1000000;
    else ls = v[2*p +1 + core];
    if(2*p+2>r)rs = -1000000;
    else rs = v[2*p +2 + core];

    if(v[p + core] > std::max(ls , rs)) return;
    else{
        if(ls > rs){ 
            std::swap(v[p + core],v[2*p+1 + core]);
            HF(v , l+core , r + core , 2*p+1+core);
        }
        if(rs >= ls){ 
            std::swap(v[p + core] ,v[2*p+2 + core]);
            HF(v , l+core , r + core , 2*p+2+core);
        }
    }

}

// implement heapsort - perhaps using a heap class
void hsort(vector<int>& v, int left, int right) {
    for(int i = right ; i >= left ; i--){
        HF(v , left , right , i);
    }
    while(true){
        if(left == right)break;
        std::swap(v[right],v[left]);
        right --;
        HF(v , left , right , left);
    }
}