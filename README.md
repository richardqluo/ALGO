# ALGO
//vector (array with consecutive memory) to replace queue (linked list with scattered memory) given size
class Vq{
private:
    vector<int> vt;
    int ct = 0;
public:
    Vq(int sz):size(sz){};
    const int size;
    
    void push(int p){
      if (vt.size()>=size){
          vt.resize(size); //add/remove item to fit the same size
          //vt.reserve(size) //reallocate if size > orig capacity to make new capacity >= size
          ct = 0;
          vt[ct] = p;
      }else{
          vt.push_back(p);
      }  
      ct++;  
    };
    
    int peek(int i){
        return vt.at(i); //check out_of_range exp before vt[i]
    };
};

int main() {
    Vq vq(2);
    vq.push(0);
    vq.push(1);
    vq.push(2);
    std::cout << vq.peek(0) << "\n";
}
