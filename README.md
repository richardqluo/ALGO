# ALGO
//vector (array with consecutive memory) to replace queue (linked list with scattered memory) given size
class vq{
private:
    vector<int> vt;
    int ct = 0;
public:
    int size; 
    
    void proc(int p){
      if (vt.size()>=size){
          ct = 0; //back to begin
          vt[ct] = p;
      }else{
          vt.push_back(p);
      }  
      ct++;  
    };
};
