#ifndef CAPP_MISC_H
#define CAPP_MISC_H
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <limits.h>
#include <sstream>
#include <memory>
using namespace std;

struct Node{
    int value;
    int min;
    Node* next;
    Node* rand;
    Node(int val,int mn=INT_MAX):value(val),min(mn),next(nullptr){}
};

struct TreeNode{
    TreeNode* right;
    TreeNode* left;
    TreeNode* next;//nextPtrRight
    vector<TreeNode*> subNodes;
    int value;
    char sym;//treeToExpression
    unsigned count;//for count smaller node than cur
    TreeNode(int val,char c=' '):value(val),sym(c){}
};
struct GraphNode{
    string value;
    vector<GraphNode*> directs;
    GraphNode(string val):value(val){}
};
struct Interval {
    int start,end;
    Interval(){};
    Interval(int s,int e):start(s),end(e){}
};
struct Job{
    int Id;
    int superId;
};
struct Point{
    int x;
    int y;
    Point(){}//Point p;
    Point(int a,int b):x(a),y(b){}
};
struct Rect{
    Point l;//lowerleft corner
    Point r;//upper right corner
    Rect(int lx,int ly,int rx,int ry){
        l=Point(lx,ly);
        r=Point(rx,ry);
    }
};
struct compNode {
    bool operator()(Node *p, Node *q) const {
        return p->value>q->value;
    }
};
//many repeated num by Node.count, push{if(v==top()) head->count++  pop{if (--head->count == 0) head=head->next;
class MinStack{
private:
    Node* head=nullptr;//seg fault if not set to nullptr
public:
    void push(int v){
        if (head==nullptr)
            head=new Node(0,v);//min=v, v-v=0
        else{

            int m=min(head->min,v);
            Node* t=new Node(v-m,m);//store val-min to save mem
            t->next=head;
            head=t;
        }
    }
    void pop(){
        if (head==nullptr)
            return;
        head=head->next;
    }
    int top(){
        if (head==nullptr)
            return INT_MAX;
        return head->value+head->min;//retore orig
    }
    int getMin(){
        if(head==nullptr)
            return INT_MAX;
        return head->min;
    }

};
template <class T>
class QueueByStack{
private:
    stack<T> in;
    stack<T> out;
public:
    void enqueue(T it){
        in.push(it);
    }
    T dequeue(){
        if(out.empty()){
            while(!in.empty()){
                T t=in.top();
                out.push(t);
                in.pop();
            }
        }
        T r=out.top();
        out.pop();
        return r;
    }
};

template<unsigned row, unsigned col>
class Matrix{
private:
    int m[row*col]{0};//int *m;
public:
    //Matrix(){m = new int[row*col]{0}; }
    //~Matrix(){ delete []m; }
    Matrix(const Matrix<row,col>& src){
        for (unsigned i=0;i<row;++i){
            for (unsigned j=0;j<col;++j) {
                //m[i*row+j]=src[i][j];
            }
        }
    }
    Matrix& operator=(const Matrix& src){
        if(this==&src)
            return *this;
        delete[] m;
        for (unsigned i=0;i<row;++i){
            for (unsigned j=0;j<col;++j) {
                //m[i*row+j]=src[i][j];
            }
        }
        return *this;
    }
    //Matrix<2,3> mx; mx[1][2]; = Proxy(1)[2]
    struct Proxy {
        int* arr;
        unsigned irow;
        Proxy(int a[],unsigned r) : arr(a),irow(r) { }
        int& operator[](const unsigned iCol) {
            return arr[irow*col+iCol];
        }
    };
    Proxy operator[](const unsigned iRow) {
        return Proxy(m,iRow);
    }
    Matrix operator+(const Matrix& src){
        for (unsigned i=0;i<row;++i){
            for (unsigned j=0;j<col;++j) {
                //m[i*row+j]+=src[i][j];
            }
        }
        return Matrix(*this);
    }
};

class Misc {
public:
    //similar O(n) find max a-b from int[]A int[]B: go thru A B find max|minA|B, return max(maxA-minB,maxB-minA),
    // if index a b can not be same, then need 2nd max|minA|B as well
    void maxProd(int a[],size_t size)
    {
        int max1=INT_MIN;
        int max2=INT_MIN;
        int max3=INT_MIN;

        int min1=INT_MAX;
        int min2=INT_MAX;
        for(unsigned i=0;i<size;i++)
        {
            if (a[i]>max1)
            {
                max3=max2;
                max2=max1;
                max1=a[i];

            }
            else if(a[i]>max2)
            {
                max3=max2;
                max2=a[i];
            }
            else if(a[i]>max3)
            {
                max3=a[i];
            }

            if(a[i]<min1)
            {
                min2=min1;
                min1=a[i];
            }
            else if(a[i]<min2)
            {
                min2=a[i];
            }

        }
        if(min1*min2>max2*max3)
        {
            cout<<min1*min2*max1<<endl;
        }
        else
        {
            cout<<max1*max2*max3<<endl;
        }

    }
    int maxSubArray(int a[],size_t size,int k)//O(n) REF JAVA vector<int>|* subSumZero
    {
        //vector<int>* res=new vector<int>();|vector<int> res;
        unordered_map<int,int> mp;
        mp[0]=-1;//case like {2 -2} [-1+1=0,1] with k=0
        int sum=0,mx=0;//mn=INT_MAX;
        for(int i=0;i<size;++i)
        {
            sum+=a[i];
            if(mp.count(sum-k)>0)//sum of a[mp[sum-k] to i] = k
            {
                mx=max(mx,i-mp[sum-k]);//mn=min(mn,i-mp[sum-k]);
                //res->push_back(mp[sum-k]+1);
                //res->push_back(i);
            }
            if(mp.count(sum)==0)//keep the earlist pos to ger max len, need not for min len
                mp[sum]=i;
        }
        return mx;//res
    }
    //increment n-1 of item each time, how many steps ti make all same
    //[1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]
    int minMoves(int num[],size_t sz){
        int mn=num[0];
        int sum=0;
        for(int i=0;i<sz;++i){
            mn=min(mn,num[i]);
            sum+=num[i];
        }
        return sum-mn*sz;//diff btw all min to num
    }
    int maxSub(int a[], size_t size)
    {
        int newSum=a[0];
        int max=a[0];
        size_t start=0, end=0;

        for(unsigned i=1;i<size;++i)
        {
            if(a[i]>newSum+a[i])
            {
                newSum=a[i];
                start=i;
            }
            else
            {
                newSum+=a[i];
            }
            if(newSum>max)
            {
                max=newSum;
                end=i;
            }
        }
        return max;
    }
    //sorted array is rotated at 7 0 pivot (0 1 2 4 5 6 7 > 4 5 6 7 0 1 2)
    int rotatePivot(int num[], size_t sz){
        int l=0, r=sz-1;
        while(l<r-1){
            int m=(l+r)/2;
            if(num[l]<num[m])//only need compare num[l], pivot right to m
                l=m;
            else//pivot left to m
                r=m;
        }//l=7 r=0
        return l;//or r
    }
    int findPeak(int a[], size_t size)
    {
        int s=0,e=size-1;
        while(s<e-1)
        {
            int m=(s+e)/2;
            if(a[m]<a[m-1])
            {
                e=m-1;
            }
            else if(a[m]<a[m+1])
            {
                s=m+1;
            }
            else
            {
                return a[m];
            }
        }
        return max(a[s],a[e]);
    }

    int sigNumBySet(int a[], size_t size)
    {
        unordered_set<int> num;
        for(unsigned i=0;i<size;++i)
        {
            if (num.count(a[i])>0)
            {
                num.erase(a[i]);
            }
            else
            {
                num.insert(a[i]);
            }
        }
        auto it=num.begin();
        return *it;
    }

    void printRepeating(int a[], size_t size)
    {
        int count[size-2];
        for(unsigned i=0;i<size;++i)
        {
            if(count[a[i]]==1)
            {
                cout<<a[i]<<endl;
            }
            count[a[i]]=1;
        }
    }
    void mergeSortedArray(int A[], int m, int B[], int n) {
        int i = m-1, j = n-1, index = m + n - 1;
        while (i >= 0 && j >= 0) {
            if (A[i] > B[j]) {
                A[index--] = A[i--];//from end avoid overwrite
            } else if (A[i] < B[j]) {
                A[index--] = B[j--];
            } else {//A[i] == B[j]
                A[index--] = A[i--];
                A[index--] = B[j--];
            }
        }
        while (i >= 0) {//only A has items left
            A[index--] = A[i--];
        }
        while (j >= 0) {
            A[index--] = B[j--];
        }
    }
    unsigned longestConsecutive(int a[], size_t size)
    {
        unordered_set<int> hs;
        unsigned mx=0;
        for(unsigned i=0;i<size;++i)
        {
            hs.insert(a[i]);
        }
        for(unsigned i=0;i<size;++i)
        {
            int left=a[i]-1;
            int right=a[i]+1;
            unsigned count=1;
            while(hs.count(left))
            {
                count++;
                hs.erase(left);
                left--;
            }
            while(hs.count(right))
            {
                count++;
                hs.erase(right);
                right++;
            }
            mx=max(count,mx);
            hs.erase(a[i]);
        }
        return mx;
    }

    int largestRectangle(int a[],size_t size)
    {
        stack<int> st;
        int mx=0;
        for(unsigned i=0;i<=size;++i)
        {
            int cur=(i==size)?-1:a[i];
            while(!st.empty() && cur<=a[st.top()])
            {
                int h=st.top();
                st.pop();
                int w=st.empty()?i:(i-st.top()-1);//or int w=i-h;
                cout<<a[h]<<"|"<<w<<endl;
                mx=max(a[h]*w,mx);
            }
            st.push(i);
        }
        return mx;
    }

    int trapWater(int a[],size_t size)
    {
        int lmax[size]={0};
        int rmax[size]={0};
        int wtr=0;
        //side bars do not trap water
        for(unsigned i=1;i<size;++i)
        {
            lmax[i]=max(lmax[i-1],a[i-1]);
            cout<<lmax[i]<<endl;
        }
        for(unsigned i=size-2;i>=0;--i)
        {
            rmax[i]=max(rmax[i+1],a[i+1]);
            cout<<rmax[i]<<endl;
            int mn=min(lmax[i],rmax[i]);
            if(mn>a[i])
            {
                wtr+=mn-a[i];
            }
        }
        return wtr;
    }
    int triangleCount(int a[],size_t size)
    {
        sort(a,a+size);
        int ct=0;
        for(unsigned i=size-1;i>2;--i)
        {
            int l=0,r=i-1;
            while(l<r)
            {
                if(a[l]+a[r]>a[i])
                {
                    ct+=r-l;
                    r--;
                }
                else
                {
                    l++;
                }
            }
        }
        return ct;
    }
    void pascalTriangle(){
        int pt[5][5];
        for(unsigned i=0;i<5;++i){
            for(unsigned j=0;j<=i;++j){
                if(j==0||j==i)
                    pt[i][j]=1;
                else
                    pt[i][j]=pt[i-1][j-1]+pt[i-1][j];
                cout<<pt[i][j];
            }
            cout<<endl;
        }
    }
    int pascalNumber(unsigned i,unsigned j){
        if(j==0||i==j)
            return 1;
        if(j==1||j==i-1)
            return i;
        return pascalNumber(i-1,j-1)+pascalNumber(i-1,j);
    }
    int maxSumTriangle(vector<vector<int>>& data){
        //vector<int> dp(data.back());
        for(unsigned row=data.size()-2;row>=0;--row){
            for(unsigned col=0;col<=row;++col){
                data[row][col]=data[row][col]+max(data[row+1][col],data[row+1][col+1]);
                //dp[col] = min(dp[col], dp[col + 1]) + data[row][col];
            }
        }
        return data[0][0];//dp[0]
    }
    int maxPointsOnLine(vector<Point>& points){
        if(points.empty())
            return 0;
        unordered_map<float, int> stat;
        int mx=1;
        for(int i=0;i<points.size();++i){
            int dup=0;
            stat.clear();//per base point
            for(int j=i+1;j<points.size();++j){//thru rest points
                if(points[i].x==points[j].x && points[i].y==points[j].y){
                    ++dup;
                }
                else{
                    //slope of vertical as INT_MAX
                    float slope= (points[j].x - points[i].x) == 0 ? INT_MAX : (float) (points[j].y - points[i].y) / (points[j].x - points[i].x);
                    if(stat.count(slope)>0)
                        stat[slope]++;
                    else
                        stat[slope]=2;//2nd point from start
                }
            }
            for(auto it=stat.begin();it!=stat.end();++it){
                if (it->second+dup>mx)
                    mx=it->second+dup;
            }
        }
        return mx;
    }
    bool isRectOverLap(Rect r1,Rect r2){
        if(r1.r.x<r2.l.x || r1.l.x>r2.r.x)//r1 left|right aside r2
            return false;
        if(r1.l.y>r2.r.y || r1.r.y<r2.l.y)//r1 above|below aside r2
            return false;
        return true;
    }
    //first index of substring b in string a
    int strStr(string a,string b){
        for(int i=0;i<a.size();++i){
            if(a[i]==b[0]){
                bool c=true;
                for(int j=1;j<b.size();++j){
                    if(a[i+j]!=b[j]){
                        c=false;
                        break;
                    }
                }
                if(c)
                    return i;
            }
        }
        return 0;
    }
    bool isPalindrome(string s){
        stack<char> st;
        int len=s.size();
        int idx=0;
        while(idx<len/2){
            st.push(s[idx]);
            idx++;
        }
        if (len%2==1)
            idx++;
        while(idx<len){
            if (st.empty())
                return false;
            if(s[idx]!=st.top())
                return false;
            idx++;
            st.pop();
        }
        return true;
    }
    string helper(string s, int l, int r){
        while(l>=0 && r<=s.size()-1 && s[l]==s[r]){
            l--;
            //count++; to find num of Palindrome include overlap
            r++;
        }
        //previous pl=l+1 and pr=r-1 length of pr-pl+1=r-l-1
        return s.substr(l+1,r-l-1);
    }
    string longestPalindrome(string s){
        if(s.size()==1)
            return s;
        string lp=s.substr(0,1);
        for (unsigned i=0;i<s.size();++i){
            string tmp=helper(s,i,i);
            if(tmp.size()>lp.size())
                lp=tmp;
            if(i<s.size()-1){
                tmp=helper(s,i,i+1);
                if(tmp.size()>lp.size())
                    lp=tmp;
            }

        }
        return lp;
    }
    bool isAnagram(string sa, string sb){
        if(sa.size()!=sb.size())
            return false;
        unsigned char ltr[128]={0};
        for (unsigned i=0;i<sa.size();++i){
            ltr[sa[i]]++;
            ltr[sb[i]]--;
        }
        for(unsigned i=0;i<128;++i){
            if (ltr[i]!=0)
                return false;
        }
        return true;

    }
    vector<vector<string>> groupAnagrams(string sa[], size_t size){
        vector<vector<string>> ret;
        unordered_map<string,vector<string>> mp;
        for (unsigned i=0;i<size;++i){
            string s=sa[i];
            sort(s.begin(),s.end());
            if(mp.count(s))
                mp[s].push_back(sa[i]);
            else{
                vector<string> vs;
                vs.push_back(sa[i]);
                mp.insert(make_pair(s,vs));
            }
        }
        for(auto m : mp)
            ret.push_back(m.second);
        return ret;
    }
    void swap(int num[], size_t bgn,size_t end){
        if(bgn==end)
            return;
        int t=num[bgn];
        num[bgn]=num[end];
        num[end]=t;
    }
    void permute(int num[],size_t sz,size_t bgn){
        if(bgn>=sz){
            for(unsigned i=0; i<sz;++i)
                cout<<num[i];
            cout<<endl;
        }
        for(unsigned i=bgn; i<sz;++i){
            swap(num,bgn,i);
            permute(num,sz,bgn+1);
            swap(num,i,bgn);
        }
    }
    static string key[]={"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
    void dailNumToStr(string dgt,vector<char>& lst,vector<string>& res){
        if(dgt.size()==0){
            string str{ lst.begin(), lst.end() };
            res.push_back(str);
            return;
        }
        int cur=dgt[0]-'0';
        string ltr=key[cur];
        for(char c : ltr){
            lst.push_back(c);
            dailNumToStr(dgt.substr(1),lst,res);
            lst.pop_back();
        }
    }
    void combNum(int num[],size_t sz,int bgn,int sum,vector<int>& lst, vector<vector<int>>& res){
        if(sum==0){
            res.push_back(lst);
            cout<<endl;
            return;
        }
        for(int i=bgn; i<sz;++i){
            if(num[i]>sum)
                continue;
            lst.push_back(num[i]);
            combNum(num,sz,i+1,sum-num[i],lst,res);
            lst.pop_back();
        }
    }
    int woodCut(unsigned l[], size_t sz, int k){
        if (sz==1)
            return l[0]/k;
        unsigned bgn=0,mid=0,mx=0;
        for(unsigned i=0;i<sz;++i){
            mx=max(mx,l[i]);//cant compare unsigned & int
        }
        unsigned end=mx-1;
        while(bgn+1<end){//bgn3,end5=mid4
            mid=(bgn+end)/2;
            unsigned ct=0;
            for(unsigned i=0;i<sz;++i)
                ct+=l[i]/mid;
            if(ct>=k){
                bgn=mid;
                mx=mid;
            }
            else{
                end=mid;
            }
        }
        return mx;
    }

    void subJobs(vector<Job> jbs,int id){
        unordered_map<int,vector<Job>*> rel;
        for(Job j:jbs){
            if(rel.count(j.superId)){
                vector<Job>* r=rel[j.superId];
                r->push_back(j);
            }
            else{
                vector<Job>* r=new vector<Job>();
                r->push_back(j);//or vector<int> store j.Id less mem
                rel[j.superId]=r;
            }
        }

    }
    void listSubJobs(unordered_map<int,vector<Job>*>& rel,int id){//(unordered_map<int,unique_ptr<vector<Job>>> rel,int id)
        vector<Job>* jbs=rel[id];
        for(Job j:*jbs){//jbs is pointer
            cout<<j.Id<<endl;
            listSubJobs(rel,j.Id);
        }
    }
    void listSubJobsBFS(unordered_map<int,unique_ptr<vector<Job>>> rel,int id){
        queue<int> q;
        q.push(id);
        while(!q.empty()){
            int cId=q.front();
            q.pop();
            unique_ptr<vector<Job>> jp=move(rel[cId]);//unique_ptr<vector<Job>> jp(new vector<Job>()); shared_ptr need not move
            for(Job& j:*jp){
                cout<<j.Id<<endl;
                q.push(j.Id);
            }
        }
    }
    int getKthSmallest(int n[],size_t sz,unsigned k,unsigned bgn,unsigned end){
        int pvt=n[end];//init call with bgn=0 end=size-1
        int l=bgn;
        int r=end-1;
        while(l<r){
            while(n[l]<pvt&&l<r){
                l++;
            }
            while(n[r]>pvt&&l<r){
                r--;
            }
            swap(n,l,r);
        }
        swap(n,l,end);
        if(k==l+1)
            return pvt;
        else if(k<l+1){
            return getKthSmallest(n,sz,k,bgn,l-1);
        }
        else{
            return getKthSmallest(n,sz,k-l-1,l+1,end);
        }
    }
    vector<int> streamMedian(int n[],size_t sz){
        priority_queue<int,vector<int>,greater<int>> maxq;
        priority_queue<int> minq;
        vector<int> ret;
        ret.push_back(n[0]);
        maxq.push(n[0]);
        for(int i=1;i<sz;++i){
            int pre=maxq.top();
            if(n[i]>pre)
                minq.push(n[i]);
            else
                maxq.push(n[i]);
            if(maxq.size()>minq.size()+1){
                minq.push(maxq.top());
                maxq.pop();
            }
            else if(maxq.size()<minq.size()){
                maxq.push(minq.top());
                minq.pop();
            }
            ret.push_back(maxq.top());
        }
        return ret;
    }
    //return P if A[0] + A[1] + ... + A[P−1] = A[P+1] + ... + A[N−2] + A[N−1] with O(n)
    int equilibriumIndex(vector<int> &A){
        int sum = 0;
        for(int i =0;i<A.size();i++){
            sum = sum + A[i];
        }
        int for_sum = 0;
        int result = 0;
        for(int i =0;i<A.size();i++){
            sum = sum-A[i];
            if(sum == for_sum){
                result = i;
                break;
            }
            for_sum = for_sum + A[i];
        }
        return result;
    }
//find index h in desc array where c[i<h]>=h and c[i>h]<=h by O(logn)
    int hIndex(vector<int>& citations) {//[6,5,2|3,1,0] 2|3
        sort(citations.begin(), citations.end(), greater<int>());
        int len = citations.size(), left = 0, right = len - 1;
        while (left < right) {
            int mid = (left + right)/2;
            if (citations[mid] == mid) return mid;
            else if (citations[mid] > mid) left = mid + 1;
            else right = mid - 1;//c[mid]<mid, to get bigger c[i] smaller i
        }
        return left;
    }
    //similiar put each item from unsorted array to array count[item]++ per occurrence
    //output thru its index (sorted), 0 no output, dup output -- till 0
    vector<int> countSortSimp(int in[], size_t sz, unsigned k){
        int ca[k+1];
        fill_n(ca,k+1,0);//cant ={0] for var size array
        for(int i=0;i<sz;++i)
            ca[in[i]]++;
        vector<int> out(sz);
        for(int j=0;j<=k;++j){
            while(ca[j]>0){//sorted index as value if exists
                out.push_back(j);
                --ca[j];
            }
        }
        return out;
    }
    vector<int> maxSlidingWindow(int num[], size_t sz, unsigned k){
        vector<int> rst;
        deque<int> dq;
        for(int i=0;i<sz;++i){
            if(i>=k){
                rst.push_back(num[dq.front()]);
                if(!dq.empty()&&dq.front()<=i-k)
                    dq.pop_front();
            }
            while(!dq.empty()&&num[dq.front()]<num[i])
                dq.pop_back();
            dq.push_back(i);
        }
        rst.push_back(num[dq.front()]);
        return rst;
    }
    vector<GraphNode*> topSort(vector<GraphNode*> graph){
        vector<GraphNode*> rst;
        unordered_map<string,unsigned> mp;
        for(GraphNode* gn : graph){
            for(GraphNode* dn : gn->directs){
                string key=dn->value;
                if(mp.count(key))
                    mp[key]++;
                else
                    mp[key]=1;
            }
        }
        queue<GraphNode*> gq;
        for(GraphNode* gn : graph){
            if(mp.count(gn->value)==0){
                rst.push_back(gn);
                gq.push(gn);
            }
        }
        while(!gq.empty()){
            GraphNode* gn=gq.front();
            gq.pop();
            for(GraphNode* dn : gn->directs){
                string key=dn->value;
                mp[key]--;
                if(mp[key]==0){
                    rst.push_back(dn);
                    gq.push(dn);
                }
            }
        }
        return rst;
    }
    //{a c d} {b c e}={a b c d e} or {b a c e d}  not {a e c b d}
    //tks=ac cd bc ce sz=5
    //similar {"caa", "aaa", "aab"} > c a b
    //compare each adjacent words find the first different letters load to pairs ca ab
    vector<char> topOrder(vector<pair<char,char>> tks, int sz) {
        int ct[sz]{0};
        queue<int> tq;
        vector<char> rs;
        for (auto& t : tks)
            ct[t.second-'a']++;//map char to int
        for(int i=0;i<sz;++i){
            if(ct[i]==0){
                tq.push(i);
                rs.push_back(i+'a');
            }
        }
        while(!tq.empty()){
            int f=tq.front();
            tq.pop();
            for (auto& t : tks){
                if (f==t.first-'a'){
                    int s=t.second-'a';
                    if(--ct[s] == 0){
                        tq.push(s);
                        rs.push_back(s+'a');//t.second
                    }
                }
            }
        }
        return rs;
    }
//1->2->3->4 as 2->1->4->3
    Node* swapPairs(Node* hd){
        Node* h=new Node(0);
        h->next=hd;//extra node to track because orig head will be swapped
        Node* p=h;

        while(p->next!=nullptr && p->next->next!=nullptr){//0-1-2-3
            Node* t1=p;//t1(0) p(0)
            p=p->next;//p(1)
            t1->next=p->next;//0->2
            Node* t2=p->next->next;//t2(3)  remember before reset next next
            p->next->next=p;//2->1
            p->next=t2;//1->3
            p=t2;//p(3)
        }//0-2-1-3
        return h->next;
    }
//O(1) space
    Node* copyRandomList(Node* hd){
        Node* i=hd;
        Node* n;
        while(i!=nullptr){
            n=i->next;
            Node* c=new Node(i->value);
            i->next=c;
            c->next=n;
            i=n;
        }
        i=hd;
        while(i!=nullptr){
            if(i->rand!=nullptr){
                i->next->rand=i->rand->next;
            }
            i=i->next->next;
        }
        i=hd;
        n=i->next;
        Node* nhd=n;
        while(i!=nullptr){
            i->next=n->next;
            i=i->next;
            n->next=i==nullptr?nullptr:i->next;
            n=n->next;
        }
        return nhd;
    }
    //Merge Interval 2D similarly merge on X & Y
    int mergeInterval(vector<Interval> av){
        int maxLen=0;
        stack<int> st;
        for(Interval i : av){
            if(st.empty()){
                st.push(i.start);
                st.push(i.end);
            }
            else{
                if(st.top()>=i.start && st.top()<i.end){//=start|end push|not new end
                    st.pop();
                    st.push(i.end);
                }
                else if(st.top()<i.start){
                    int e=st.top();
                    st.pop();
                    maxLen=max(maxLen,e-st.top());
                    st.pop();
                    st.push(i.start);
                    st.push(i.end);
                }
            }
        }
        if(!st.empty()){
            int e=st.top();
            st.pop();
            maxLen=max(maxLen,e-st.top());
            st.pop();
        }
        return maxLen;
    }
    bool isValidBST(TreeNode* root,double min,double max){//isValidBST(root,INT_MIN,INT_MAX)
        if(root==nullptr)
            return  true;
        if(root->value<=min||root->value>=max)
            return false;
        return isValidBST(root->left,min,root->value) && isValidBST(root->right,root->value,max);
    }
    int maxDepth(TreeNode* root){
        if(root==nullptr)
            return 0;
        int l=maxDepth(root->left);
        int r=maxDepth(root->right);
        return max(l,r)+1;//min for minDepth
    }
    int maxDepths(TreeNode* root){
        if(root==nullptr)
            return 0;
        int curMax=0;
        for(TreeNode* n:root->subNodes){
            int curDepth=maxDepths(n);
            curMax=max(curMax,curDepth);
        }
        return curMax+1;
    }
    int maxSum (TreeNode* root){
        if(root==nullptr)
            return 0;
        int lmx=maxSum(root->left);
        int rmx=maxSum(root->right);
        return root->value+max(lmx,rmx);
    }
    //max distance btw nodes
    int diameter(TreeNode * tree)
    {
        if (tree == NULL)
            return 0;
        int lheight = maxDepth(tree->left);
        int rheight = maxDepth(tree->right);
        int ldiameter = diameter(tree->left);
        int rdiameter = diameter(tree->right);
        //longest path between leaves via root vs diameter of root’s left|right subtree
        return max(lheight + rheight + 1, max(ldiameter, rdiameter));
    }
    bool isBalanced(TreeNode* root){
        int lh=0,rh=0;
        if(root==nullptr)
            return 0;
        lh=maxDepth(root->left);
        rh=maxDepth(root->right);
        if(abs(lh-rh)<=1 && isBalanced(root->left) && isBalanced(root->right))
            return true;
        return false;
    }
    bool isFull(TreeNode* root){
        if(root==nullptr)
            return true;
        if(root->left==nullptr&&root->right==nullptr)
            return true;
        if(root->left==nullptr||root->right==nullptr)//same as compareTree
            return false;
        return isFull(root->left)&&isFull(root->right);
    }
    bool hasPathSum(TreeNode* root, int sum){
        if (root==nullptr)
            return false;
        if (root->value>sum)//only applicable if all node->value>0
            return false;
        if (root->value==sum && root->left==nullptr && root->right==nullptr)
            return true;

        return hasPathSum(root->left,sum-root->value)||hasPathSum(root->right,sum-root->value);
    }
//REV#
    void pathSum(TreeNode* root,int sum,vector<vector<int>>& result,vector<int>& list){
//void pathMinSum(TreeNode* root,int curSum,int minSum,vector<vector<int>>& result,vector<int>& list){
        if(root->value>sum)//if use sum==0 then 1 more recursion and cant tell if leaf
            //if(curSum+root->value>minSum)
            return;
        if(root->value==sum && root->left==nullptr && root->right==nullptr){
            //if(curSum+root->value<minSum && root->left==nullptr && root->right==nullptr){
            list.push_back(root->value);//list from previous recursion does not include current node
            result.push_back(list);
            list.pop_back();
        }
        if(root->left!=nullptr){
            list.push_back(root->value);
            pathSum(root->left,sum-root->value,result,list);
            //pathMinSum(root->left,curSum+root->value,minSum,result,list);
            list.pop_back();
        }
        if(root->right!=nullptr){
            list.push_back(root->value);
            pathSum(root->right,sum-root->value,result,list);
            //pathMinSum(root->left,curSum+root->value,minSum,result,list);
            list.pop_back();
        }
    }

    int maxPathSum (TreeNode* root,int sum){
        if(root==nullptr)
            return sum;
        int mx=max(maxPathSum(root->left,root->value+sum),maxPathSum(root->right,root->value+sum));
        return max(sum,mx);//root->value < 0
    }
    //depth of path with most distinct nodes
    int maxPathDistinct (TreeNode* root,unordered_set<int>& prev){
        if(root==nullptr)
            return 0;
        char add=1;
        if(prev.count(root->value)>0)
            add=0;//not to increment if duplicate
        prev.insert(root->value);
        int mx=max(maxPathDistinct(root->left,prev)+add,maxPathDistinct(root->right,prev)+add);
        prev.erase(root->value);
        return mx;
    }
//dfs to sum int at each node, x10 per layer
    int sumNum(TreeNode* root,int sum){
        if(root==nullptr)
            return 0;
        if(root->left==nullptr&&root->right==nullptr)
            return sum*10+root->value;
        return sumNum(root->left,sum*10+root->value)+sumNum(root->right,sum*10+root->value);
    }
    int sumNumBFS(TreeNode* root){
        int sum;
        queue<TreeNode*> q;
        q.push(root);
        while(!q.empty()){
            for(int i=0;i<q.size();++i){
                TreeNode* c=q.front();
                q.pop();
                sum+=10*sum+c->value;
                if(c->left!=nullptr)
                    q.push(c->left);
                if(c->right!=nullptr)
                    q.push(c->right);
            }
        }
        return sum;
    }
    unsigned insertNode(TreeNode* root,int val){
        if(root==nullptr){//only 1st node
            root=new TreeNode(val);
            return 0;
        }
        while(root!=nullptr){
            if(root->value>val){
                if(root->left==nullptr){
                    root->left=new TreeNode(val);
                    int ct=root->count;
                    root->count++;//root has additional left
                    return ct;
                }
                else{
                    root=root->left;
                }
            }else{
                if(root->right==nullptr){
                    root->right=new TreeNode(val);
                    root->right->count=root->count+1;// >root and all its smaller
                    return root->right->count;
                }
                else{
                    root=root->right;
                }
            }
        }
    }
    //Count of Smaller Numbers After Self [5, 2, 6, 1], after 5|2 there are 2|1 smaller (2,1)|(1), output [2, 1, 1, 0].
    vector<int> smallerNumsCount(int num[],size_t sz){
        vector<int> ret;
        TreeNode* root=new TreeNode(num[sz-1]);
        for(unsigned i=sz-2;i>=0;--i){
            int ct=insertNode(root,num[i]);
            ret.push_back(ct);
        }
        return ret;
    }
    void removeleaf(TreeNode* p) {
        if (p == NULL) {
            return;
        }
        else {
            if (p->left || p->right) {
                removeleaf(p->left);
                removeleaf(p->right);
            }
            else {
                delete p;
                return;
            }
        }
    }
    //REV#
    TreeNode* lowestCommonAncestor(TreeNode* root,TreeNode* l,TreeNode* r){//l<r
        while(root!=nullptr){
            if(root->value<l->value)
                root=root->right;//return lowestCommonAncestor(root->right,l,r);
            else if(root->value>r->value)
                root=root->left;//return lowestCommonAncestor(root->left,l,r);
            else
                return root;
        }
        return nullptr;
    }
//revised
    void findPrevNextTree(TreeNode* root,int val){
        int prev=INT_MIN,next=INT_MAX;
        while(root!=nullptr){
            if(root->value>val){
                next=root->value;
                if(root->left->value<val){
                    prev=root->left->value;
                    return;
                }
                else
                    root=root->left; //findPrevNextRecursive(root->left,prev,next,val);
            }
            else if(root->value<val){
                prev=root->value;
                if(root->right->value>val){
                    next=root->right->value;
                    return;
                }
                else
                    root=root->right;//findPrevNextRecursive(root->right,prev,next,val);
            }
            else{
                prev=root->left->value;
                next=root->right->value;
                return;
            }
        }
    }
    void findPrevNextArray(int num[],size_t sz,int val){
        int bgn=0,mid=0,end=sz-1;
        int prev=INT_MIN,next=INT_MAX;
        while(bgn<end-1){//beg(prev) always < end(next)
            mid=(bgn+end)/2;
            if(num[mid]<val)
                bgn=mid;
            else if(num[mid]>val)
                end=mid;
            else{
                prev=num[mid-1];
                next=num[mid+1];
                return;
            }
        }
        prev=num[bgn];
        next=num[end];
    }
    //UNI#finding right|left most occurrences of key in sort array with duplicate
    int getRightMost(int num[], size_t sz,int val)//Left
    {   int m, r=sz-1, l=0;
        while( r - l > 1 )
        {  m = l + (r - l)/2;
            if( num[m] < val )
                l = m;
            else if(num[m] > val)
                r = m;
            else
                l=m;//r=m; when num[m] == val keep move toward right|left till not
        }
        return l;//return r;
    }
    TreeNode* sortedArrayToBST(int num[],int bgn,int end){//end=size-1
        if(bgn>end)
            return nullptr;
        int mid=(bgn+end)/2;
        TreeNode* root=new TreeNode(num[mid]);
        root->left=sortedArrayToBST(num,bgn,mid-1);
        root->right=sortedArrayToBST(num,mid+1,end);
        return root;
    }
    Node* findMid(Node* head){
        Node* slow=head;
        Node* fast=head;
        while(fast->next!=nullptr && fast->next->next!=nullptr){
            slow=slow->next;
            fast=fast->next->next;
        }
        return slow;
    }
    TreeNode* sortedListToBST(Node* head){
        if(head==nullptr)
            return nullptr;
        else if(head->next==nullptr)
            return new TreeNode(head->value);
        Node* mid= findMid(head);
        TreeNode* root=new TreeNode(mid->next->value);
        root->right=sortedListToBST(mid->next->next);
        mid->next=nullptr;//can only break by mid-next
        root->left=sortedListToBST(head);
        return root;
    }
    Node* IterateMergeList(Node* ln,Node* rn){//ref recursiveMergeList
        Node* tn=new Node(0);
        Node* hd=tn;
        while(ln!=nullptr&&rn!=nullptr){
            if(ln->value<rn->value){
                hd->next=ln;
                ln=ln->next;
            }
            if(ln->value<rn->value){
                hd->next=rn;
                rn=rn->next;
            }
            hd=hd->next;
        }
        if(ln!=nullptr)
            hd->next=ln;
        else if(rn!=nullptr)
            hd->next=rn;
        return tn->next;
    }
//O(n log n) time using constant space complexity
    Node* sortList(Node* head){
        if(head==nullptr || head->next==nullptr)
            return head;//recurse till single node
        Node* mid=findMid(head);
        Node* r=sortList(mid->next);
        mid->next=nullptr;
        Node* l=sortList(head);
        return IterateMergeList(r,l);
    }

//log(k) * n
    Node* mergeKLists(Node* heads[],size_t sz){
        priority_queue<Node*, vector<Node*>, compNode> pq;
        Node* hd=new Node(0);
        Node* cur=hd;
        for(unsigned i=0;i<sz-1;++i){
            if(heads[i]!=nullptr)
                pq.push(heads[i]);
        }
        while(!pq.empty()){
            Node* tn=pq.top();
            pq.pop();
            cur->next=tn;
            cur=cur->next;
            if(tn->next!=nullptr)//if input vector<queue<int>>, use pair{i,val} to track vector[i] for next val
                pq.push(tn->next);
        }
        return hd->next;
    }
    vector<int> preorderTraversal(TreeNode* root){
        vector<int> rv;
        if(root==nullptr)
            return rv;
        stack<TreeNode*> stk;
        stk.push(root);
        while(!stk.empty()){
            TreeNode* cur=stk.top();
            stk.pop();
            rv.push_back(cur->value);
            if(cur->right!=nullptr)
                stk.push(cur->right);//push right before left and left pop before right
            if(cur->left!=nullptr)
                stk.push(cur->left);
        }
        return rv;
    }
    //in-order:   4 2 5 (1) 6 7 3 8 left-root-right
    //pre-order: (1) 2 4 5  3 7 6 8 root-left-right
    TreeNode* buildHelper(vector<int> preOrder,int preBgn,int preEnd,vector<int> inorder,int inBgn,int inEnd){
        TreeNode* root=new TreeNode(preOrder[preBgn]);
        int pos=0;
        for(unsigned i=0;i<inorder.size()-1;++i){
            if(inorder[i]==preOrder[preBgn]){
                pos=i;
                break;
            }
        }
        int len=pos-inBgn;
        root->left=buildHelper(preOrder,preBgn+1,preBgn+len,inorder,inBgn,pos-1);
        root->right=buildHelper(preOrder,preBgn+len+1,preEnd,inorder,pos+1,inEnd);
        return root;
    }
    bool twoSumTree(TreeNode* root,int sum){
        stack<TreeNode*> stk1;
        stack<TreeNode*> stk2;
        TreeNode* cur1=root;
        TreeNode* cur2=root;
        while(!stk1.empty()||!stk2.empty()||cur1!=nullptr||cur2!=nullptr){
            if(cur1!=nullptr||cur2!=nullptr){
                if(cur1!=nullptr){
                    cur1=cur1->left;
                    stk1.push(cur1);
                }
                if(cur2!=nullptr){
                    cur2=cur2->right;
                    stk2.push(cur2);
                }
            }
            else{//till reach the end of left and right
                int val1=stk1.top()->value;
                int val2=stk2.top()->value;
                if(val1==val2)
                    break;//left=right
                if(val1+val2==sum)
                    return true;
                else if(val1+val2<sum){//increase by val1
                    cur1=stk1.top()->right;
                    stk1.pop();
                }
                else{//val1+val2>sum, reduce by val2
                    cur2=stk2.top()->left;
                    stk2.pop();
                }
            }
        }
        return false;
    }

    bool compareTree(TreeNode* t1,TreeNode* t2){
        if(t1==nullptr&&t2==nullptr)
            return true;
        else if(t1==nullptr||t2==nullptr)
            return false;
        return compareTree(t1->left,t2->right) && compareTree(t1->right,t2->left);//symmetric
    }
    //REV#
    vector<int> rightSideView(TreeNode* root){//int nodeLayer(TreeNode* root,int layer,int order)
        vector<int>  res;
        if(root==nullptr)
            return res;
        queue<TreeNode*> tq;
        //int level=0
        while(!tq.empty()){
            size_t sz=tq.size();
            //level++;
            for(unsigned i=0;i<sz;++i){
                TreeNode* cur=tq.front();
                tq.pop();
                //if(level==layer && i=order-1)
                //return cur->value;
                if(i==0)
                    res.push_back(cur->value);
                if(cur->right!=nullptr)
                    tq.push(cur->right);
                if(cur->left!=nullptr)
                    tq.push(cur->left);
            }//nodes pudhed by whole layer
        }
        return res;
    }
    //Next Pointers to Right Node or null, recursive for space O(1)
    void nextPtrRight(TreeNode* root){
        if(root==nullptr)
            return;
        TreeNode* ln=root->left;
        TreeNode* rn=root->right;
        if(ln!=nullptr){
            ln->next=rn;
            if(ln->right!=nullptr)
                ln->right->next=rn->left;
        }
        nextPtrRight(ln);
        nextPtrRight(rn);
    }
    int getWeight(unsigned base,char c){
        if(c=='+'||c=='-')
            return base+1;
        if(c=='*'||c=='/')
            return base+2;
        return INT_MAX;
    }
    TreeNode* expressionToTree(string exp){
        stack<TreeNode*> stk;
        unsigned base=0;
        int val=0;
        for(char c : exp){
            if(c=='('){
                base+=10;
                continue;
            }
            if(c==')'){
                base-=base;
                continue;
            }
            val=getWeight(base,c);
            TreeNode* cur=new TreeNode(val,c);
            while(!stk.empty() && val<=stk.top()->value){//min tree
                TreeNode* prev=stk.top();
                stk.pop();
                cur->left=prev;
            }
            if(!stk.empty()){
                stk.top()->right=cur;
            }
            stk.push(cur);
        }
        TreeNode* res=stk.top();
        while(!stk.empty()){
            res=stk.top();
            stk.pop();
        }
        return res;
    }
    string treeToExpression(TreeNode* root){
        string cur="";
        string l="";
        string r="";
        if(root!=nullptr){
            cur=root->sym;
            if(root->sym=='*' || root->sym=='/'){
                if(root->left->sym=='+'||root->left->sym=='-')////(a+b)*...
                    l=l;//l="("+treeToExpression(root->left)+")";
                else
                    l=treeToExpression(root->left);
                if(root->right->sym=='+'||root->left->sym=='-')//...*(a+b)
                    r=r;//r="("+treeToExpression(root->right)+")";
                else
                    r=treeToExpression(root->right);
            }
            else{//a+b+c+d a*b+c*d
                l=treeToExpression(root->left);
                r=treeToExpression(root->right);
            }
        }
        return l.append(cur.append(r));//l+cur+r
    }
    int calulate(int a, int b, const string &op) {
        if (op == "+") return a + b;
        else if (op == "-") return a - b;
        else if (op == "*") return a * b;
        else return a / b;
    }
    bool isOK(const string &op1, const string &op2) {//op1 is higher than op2
        if (op1 == "*" || op1 == "/" || op2 == ")") return true;
        else return op2 == "+" || op2 == "-";
    }
    //UNI# assume the expression is valid, 1 stack store variables, 1 stack store operators
    //if var push to stack,
    //if op higher than prevop, push to stack
    //if op lower than prevop,calc all prevops higher than op before push op
    //if op is ( push to stack
    //if op is ) calc all prevopFs before prev (
    int evaluateExpression(vector<string> &expression) {
        if (expression.empty()) return 0;
        stack<int> stk_val;
        stack<string> stk_op;
        for (int i = 0; i <= expression.size(); ++i) {
            if (i < expression.size() && isdigit(expression[i])) {
                stk_val.push(atoi(expression[i].c_str()));//keep push if variable
            } else if (i == expression.size() || expression[i] != "(") {
                while (!stk_op.empty() && stk_op.top() != "(" && (i == expression.size() || isOK(stk_op.top(), expression[i]))) {
                    int tmp = stk_val.top();//if prev op is not ( or cur op is lower keep calc till
                    stk_val.pop();
                    stk_val.top() = calulate(stk_val.top(), tmp, stk_op.top());
                    stk_op.pop();
                }
                if (i == expression.size()) break;
                else if (expression[i] == ")") stk_op.pop();
                else stk_op.push(expression[i]);
            } else {
                stk_op.push(expression[i]);//push (
            }
        }
        return stk_val.top();
    }
    bool wordBreak(string s,unordered_set<string> dict){
        if(s.empty() || dict.count(s))
            return true;
        string ss=string("#").append(s);
        int len = ss.size();
        bool dp[len];
        dp[0]=true;//treat # as in dict

        for(int i=1;i<len;++i){
            for(int k=0;k<i;++k){
                //valid if dp[k] == true and s[k+1,i] in dictionary 0<k<i
                dp[i]=dp[k]&&dict.count(ss.substr(k+1,i-k));
                if(dp[i])//as long as one comb valid
                    break;
            }
        }
        return dp[len-1];
    }
    //"13 DUP 4 POP 5 DUP + DUP + -" rerutn 7
    int solution2(string &S) {
        stringstream ss(S);
        vector<string> sv;
        string buf;
        while (ss >> buf)
            sv.push_back(buf);
        stack<int> st;
        for (auto c : sv){
            if(int ci=atoi(c.c_str())>0)
                st.push(ci);
            else {
                if (c == "+" && st.size()>=2) {
                    int s1=st.top();
                    st.pop();
                    int s2=st.top();
                    st.pop();
                    int x = s1+s2;
                    st.push(x);
                }
                else if (c == "-" && st.size()>=2) {
                    int s1=st.top();
                    st.pop();
                    int s2=st.top();
                    st.pop();
                    int x = s1-s2;
                    st.push(x);
                }
                else if (c == "DUP") {
                    int s1=st.top();
                    st.push(s1);
                }
                else if (c == "POP") {
                    st.pop();
                }
                else
                    return -1;
            }
        }
        return st.top();
    }
    const int m=10,n=10;
    static int matrix[m][n];
    //end = m * n - 1; while(start<end){  mid = (end + start)/2;
    //int num = matrix[mid/n][mid%n];//[row][col]  if (num <|> target) { start|end =mid;}...O(logN+M)
    bool searchSortMatrix(int** matrix,int val){
        int r=5,c=0;//start from first item in last row
        while(r>=0 && c<=10){
            if(val<matrix[r][c])
                --r;
            else if(val>matrix[r][c])
                ++c;
            else
                return true;
        }
        return false;
    }
    //max gain sell 8 by any way 3:7+5:15=22
    //{ 1, 4, 7, 9,15, 17,18,20,0} append 0 to actual array before call
    int maxGain(int gain[],size_t sz){
        int dp[sz]={0};//dp[0]=0
        for(int i=1;i<sz;++i){
            for(int j=1;j<=i;++j)//j=0 same as i
                //for all j btw [0,i] gain by sell j + max gain of sell(i - j)
                dp[i]=max(dp[i],dp[i-j]+gain[j-1]);//j-1: sell 2 map to gain[1]
        }
        return dp[sz-1];
    }
    int robMax(int gain[],size_t sz){
        int dp[sz+1]={0};
        dp[1]=gain[0];//dp[0]=0;
        for(int i=2;i<sz+1;++i){
            dp[i]=max(dp[i-1],dp[i-2]+gain[i-1]); //gain[i-1] for i
        }
        return dp[sz];
    }
    //BFS need queue to store all next nodes from current nodes
    int traverse(int row,int col,int way,vector<int>&path,vector<vector<int>>& result){
        if(row<0||row>=m||col<0||col>=n)
            return 0;
        else{
            int pos=matrix[row][col];
            if(pos==1||pos==-1)
                return 0;
            else if(row==m-1&&col==n-1){
                path.push_back(way);
                result.push_back(path);
                path.pop_back();
                return 1;
            }
            matrix[row][col]=-1;
            path.push_back(way);//previous step
            int ret=traverse(++row,col,1,path,result)+
                    traverse(row,++col,2,path,result)+
                    traverse(--row,col,3,path,result)+
                    traverse(row,--col,4,path,result);
            path.pop_back();
            matrix[row][col]=pos;
            return ret;//return after restore
        }
    }
    int minPathSum(int** matrix){//err matrix[][] must have const size
    //int uniqPathSum(int** matrix){
        int dp[m][n];//O(MN)time
        dp[0][0]=matrix[0][0];
        for(unsigned r=1;r<n;++r){
            dp[r][0]=dp[r-1][0]+matrix[r][0];
            //dp[r][0]=1;
        }
        for(unsigned c=1;c<m;++c){
            dp[0][c]=dp[0][c-1]+matrix[0][c];
            //dp[0][c]=1;
        }
        for(unsigned r=1;r<n;++r){
            for(unsigned c=1;c<m;++c)
                dp[r][c]=min(dp[r-1][c],dp[r][c-1])+matrix[r][c];
            //dp[r][c]=dp[r-1][c]+dp[r][c-1];
        }

    }
    //loop thru all pos to call with vector<vector<int> > dp(m, vector<int>(n, 0));  and return max
    int longestIncreasePath(vector<vector<int>> &matrix, vector<vector<int>> &dp, int i, int j) {
        if (dp[i][j])
            return dp[i][j];//if alreay calc
        vector<vector<int> > dirs = {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
        int mx = 1, m = matrix.size(), n = matrix[0].size();
        for (auto a : dirs) {
            int x = i + a[0], y = j + a[1];
            if (x < 0 || x >= m || y < 0 || y >= n || matrix[x][y] <= matrix[i][j])
                continue;
            int len = 1 + longestIncreasePath(matrix, dp, x, y);
            mx = max(mx, len);
        }
        dp[i][j] = mx;
        return mx;
    }
    const int R=10,C=10;
    void findWordsInMatrix(char matrix[R][C], int i, int j, string &str)
    {
        char preval=matrix[i][j];
        matrix[i][j] = '#';//mark visited
        str.push_back(preval);
        //if (dict.count(str)>0) cout << str << endl;// print str is present in dictionary unordered_set
        vector<vector<int> > dirs = {{0, -1}, {-1, 0}, {0, 1}, {1, 0},{-1, -1}, {-1, 1}, {-1, 1}, {1, 1}};//8 ways
        for (auto a : dirs) {
            int row = i + a[0], col = j + a[1];
            if (row>=0 && row<R && col>=0 && col<C && matrix[row][col] != '#')
                findWordsInMatrix(matrix,row, col, str);
        }
        str.pop_back();// Erase current character from string
        matrix[i][j] = preval;
    }
    //total number of ways to decode a digits message based on map 'A'=1 'B'=2  . 'J'=10.. 'Z'=26
    int numDecodings(char *digits, int n)//121
    {
        if (n == 0 || n == 1)
            return 1;
        int count = 0;
        if (digits[n-1] > '0')     // If the last digit > 0
            count =  numDecodings(digits, n-1); //return as 1 way if each digit >0 : 1 2 1 = A B A
        if (digits[n-2] < '2' || (digits[n-2] == '2' && digits[n-1] < '7') )// If the last two digits is from 10 to 26
            count +=  numDecodings(digits, n-2); //21 = U, count 1 from above step+recur(1)
        return count;
    }
    //#REV O(n)
    int numDecodingsDP(char *digits, int n)
    {
        int count[n+1]{}; //count[n]:digits[n-1]
        count[0] = 1;
        count[1] = 1; //digits[0] can only 1 way
        for (int i = 2; i <= n; i++)
        {
            if (digits[i-1] > '0')
                count[i] = count[i-1];//count[i-1] X pass as 1 way
            if (digits[i-2] < '2' || (digits[i-2] == '2' && digits[i-1] < '7') )
                count[i] += count[i-2];//count[i-2] XX add as extra way
        }
        return count[n];
    }
    int minPathSumDFS(int row,int col){
    //int uniqPathSumDFS(int row,int col){
    //int minPathSumDFS(int row,int col, step k){ similar after k steps
        if(row==m-1&&col==n-1)// || k=0
            return matrix[row][col];
        //return 1;
        if(row<m-1&&col<n-1){
            int d=minPathSumDFS(++row,col)+matrix[row][col];//(++row,col,k-1)
            int r=minPathSumDFS(row,++col)+matrix[row][col];
            return min(d,r);
            //return uniqPathSumDFS(++row,col)+uniqPathSumDFS(++row,col);
        }
        if(row<m-1)
            return minPathSumDFS(++row,col)+matrix[row][col];
        //return uniqPathSumDFS(++row,col);
        if(col<n-1)
            return minPathSumDFS(row,++col)+matrix[row][col];
        //return uniqPathSumDFS(row,++col);
        return 0;//out of boundary
    }
    int calculateMinimumHP(int** matrix){
        int hp[m][n];
        hp[m-1][n-1]=max(1-matrix[m-1][n-1],1);
        for(unsigned r=m-2;r>=0;--r)
            hp[r][n-1]=max(hp[r+1][n-1]-matrix[r][n-1],1);
        for(unsigned c=n-2;c>=0;--c)
            hp[m-1][c]=max(hp[m-1][c+1]-matrix[m-1][c],1);
        for(unsigned r=m-2;r>=0;--r){
            for(unsigned c=n-2;c>=0;--c){
                int down=max(hp[r+1][c]-matrix[r][c],1);
                int right=max(hp[r][c+1]-matrix[r][c],1);
                hp[r][c]=min(down,right);
            }
        }
        return hp[0][0];
    }
    int sideIslands(int r,int c,int land){
        if(r<0||c<0||r>m-1||c>n-1)//side to boundary
            return 1;
        if(matrix[r][c]==-land)//revisit
            return 0;
        if(matrix[r][c]!=land)//side to water or other land
            return 1;
        matrix[r][c]=-land;
        return sideIslands(++r,c,land)+
               sideIslands(r,++c,land)+
               sideIslands(--r,c,land)+
               sideIslands(r--,c,land);

    }
    int numIslands(int** matrix){
        int sum=0;
        for(int r=0;r<m-1;++r){
            for(int c=0;c<n-1;++c){
                if(matrix[r][c]!=0)//land
                    sum+=sideIslands(r,c,matrix[r][c]);
            }
        }
        return sum;
    }
    int maxSquare(int** matrix){
        int dp[m][n];
        int maxLen=0;
        for(int r=0;r<m-1;++r){
            dp[r][m-1]=matrix[r][m-1];
            maxLen=max(maxLen,dp[r][m-1]);
        }
        for(int c=0;c<n-1;++c){
            dp[n-1][c]=matrix[n-1][c];
            maxLen=max(maxLen,dp[n-1][c]);
        }
        for(int r=m-2;r>=0;--r){
            for(int c=n-2;c>=0;--c){
                if(matrix[r][c]==1){
                    //if adjacent 3 cells are at least 1, then cur cell (1) form 2x2 1s
                    dp[r][c]=min(min(dp[r+1][c],dp[r][c+1]),dp[r+1][c+1])+1;
                    maxLen=max(maxLen,dp[r][c]);
                }
            }
        }
        return maxLen*maxLen;
    }
};


#endif //CAPP_MISC_H
