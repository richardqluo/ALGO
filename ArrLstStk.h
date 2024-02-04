#ifndef CAPP_ARRLSTSTK_H
#define CAPP_ARRLSTSTK_H
#include <iostream>
#include <memory>
#include <algorithm>
#include <limits.h>
#include <limits>
#include <queue>
#include <stack>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <bitset>
#include <vector>
#include <assert.h>
using namespace std;

class ArrLstStk {
public:
    char firstUniqueNum(char* head)//char[] head, also can be firstUniqueChar ref JAVA
    {//each 8bit number in array by [signed]char [-128,127] or unsigned char[0,255]
        if(head == nullptr)//char head[]={'a','b','a','c','\0'};
            return -1;
        unsigned char count[256];//occurance up to 255 of each input char number value in [-128,127]
        char* cur = head;//head stay begin for 2nd loop
        while(*cur++ != '\0'){
            count[*cur]++;
        }
        //if head much longer than 256, loop thru count instead
        while(*head++ != '\0'){
            //similar find mode (most occurance) num/char
            //if (count[*head]>maxCount){maxCount=count[*head]; maxItem=*head;}
            if (count[*head]==1)
                return *head;
        }
    }
    //UNI#
    vector<int> intersectKsortedArr(int** nums,int rows){
        int curIdx[rows]={0};//latest pos of each array O(mn)
        int max=INT_MIN;
        int count=0;
        vector<int> ret;
        while(true) {//loop thru each row/array again and again till one reach the end
            for (int i = 0; i < rows; ++i) {
                if (nums[i][curIdx[i]] == '\0')
                    return ret;//if any array reach the end
                if (nums[i][curIdx[i]] > max) {
                    max = nums[i][curIdx[i]];
                    count = 1;
                }
                else if (nums[i][curIdx[i]] == max) {
                    count++;
                    if (count == rows) {
                        ret.push_back(max);//max is current same value across arrays
                    }
                }
                curIdx[i]++;
            }
        }
    }
    //UNI#
    void verticalOutput2D(int** nums,int rows){
        int curIdx[rows]={0};//latest pos of each array
        int count=0;
        while(true) {//loop thru each row/array again and again till all reach the end
            for (int i = 0; i < rows; ++i) {
                if (nums[i][curIdx[i]] != '\0'){
                    ++curIdx[i];
                    ++count;
                    cout<<nums[i][curIdx[i]];
                }
            }
            if (count == 0) {//all reach the end
                return;
            }
            count=0;
        }
    }
    //ar1[] = {2, 3, 7, 10, 12}, ar2[] = {1, 5, 7, 8} return 1 + 5 + 7 + 10 + 12=35
    //UNI#start from either sorted array and switch over another array at any common item, find the max sum thru the path
    int maxPathSumCross2Arr(int ar1[], int ar2[], int m, int n)
    {
        int i = 0, j = 0;
        int  result = 0, sum1 = 0, sum2 = 0;
        while (i < m && j < n)
        {
            if (ar1[i] < ar2[j])
                sum1 += ar1[i++];
            else if (ar1[i] > ar2[j])
                sum2 += ar2[j++];
            else  // we reached a common point, sum1|2 has sum of ar1|2 before it
            {
                result += max(sum1, sum2);
                sum1 = 0, sum2 = 0;//reset to restart calc from next uncommon element
                while (i < m &&  j < n && ar1[i] == ar2[j])//rest common items if so
                {
                    result = result + ar1[i++];
                    j++;
                }
            }
        }
        while (i < m)
            sum1  +=  ar1[i++];//add rest ar1 longer then ar2
        while (j < n)
            sum2 +=  ar2[j++];
        result +=  max(sum1, sum2);
        return result;
    }
    vector<int> findIntersect2Arr(vector<int> a,vector<int> b) {
        std::sort(a.begin(), a.end());//a.size()<b.size()
        for(vector<int>::iterator it=b.begin();it!=b.end();){
            //also can use hashset keep a[i], O(n) space
            if(!std::binary_search(a.begin(),a.end(),*it)) {//time O(logn), similar find difference
                it = b.erase(it);
            }
            else{
                ++it;
            }
        }
        return b;
    }

    //UNI# cut off the extra of longer list, then both start from head, will reach the intersection node at the same time
    ListNode* getIntersecNode(ListNode *headA, ListNode *headB) {
        if(!headA || !headB) return NULL;
        int lenA = 0, lenB = 0;
        ListNode *tailA = headA, *tailB = headB;
        while(tailA->next) {//if cycle use hashset to check
            tailA = tailA->next;
            lenA++;
        }
        while(tailB->next) {
            tailB = tailB->next;
            lenB++;
        }
        if(tailA!=tailB) return NULL;//nothing in common
        if(lenA>lenB) {//move the diff
            for(int i=0; i<lenA-lenB; i++)
                headA = headA->next;
        }
        else {
            for(int i=0; i<lenB-lenA; i++)
                headB = headB->next;
        }
        while(headA!=headB) {//move till meet
            headA = headA->next;
            headB = headB->next;
        }
        return headA;
    }
    //UNI#
    ListNode* getKIntersectNodes(vector<ListNode*> heads){
        unordered_map<ListNode*,unsigned> mp;//pointer(node address) as key
        ListNode* hd=heads[0];
        mp[hd]=1;
        while(true){
            for(vector<ListNode*>::iterator it=heads.begin()+1;it!=heads.end();++it){//iterator& err because begin() is rvalue
                if(*it==nullptr)
                    break;
                ListNode* cur=*it;
                if(mp.count(cur)>0){
                    mp[cur]++;//use map vs int because list is not sorted
                    if(mp[cur]==heads.size())
                        return cur;
                }
                *it=cur->next;
            }
            hd=hd->next;
            mp[hd]=1;
        }
        return NULL;
    }
    //Sorting array by frequency and then alphabetically
    void sortArrFreqAlph(int a[],int size) {//char[] use int|char[26] count
        std::unordered_map<int, unsigned int> count;
        for (int i=0;i<size;++i)
            count[a[i]]++;//count the number of ocurences of each
        //Use the values of that map as comparison, order alphabetically if same count
        std::sort(a, a+size,
                  [&](const char &lhs, const char &rhs) {//same as bool comp(const char &lhs, const char &rhs) with closure
                      if (count[lhs] == count[rhs])
                          return lhs < rhs;
                      return count[lhs] > count[rhs];
                      //return count[lhs] > count[rhs] || count[lhs] == count[rhs] && lhs < rhs;
                  }
        );
    }
    //UNI# binary search sorted string array, Find “ball" in [“at”,””,””,””,”ball”,””,””,”car”,”dad”,””] return 4.
    int findStrSortArr(const string s[], int left, int right, const string& x)
    {
        if(left>right)
            return -1;
        int mid=(left+right)/2;
        if(s[mid].empty())
        {
            int t = mid;
            while ( t < right && s[t].empty() )
                ++t;
            if(t==right)//all string after mid are empty, no need to go after it
                return findStrSortArr(s, left, mid-1, x);//keep mid pos
            else//t at next unempty string after mid
                mid=t;
        }
        if(s[mid]==x)
            return mid;
        else if(s[mid]<x)//string only compared by operator <, > get error
            return findStrSortArr(s, mid+1, right, x);
        else
            return findStrSortArr(s, left, mid-1, x);
    }
    unsigned twoSum(int a[], size_t size, int sum)
    {
        unordered_set<int> hs;
        //unordered_map<int,unsigned> hm;
        unsigned count=0;
        //unsigned minRange=INT_MAX;
        for(unsigned i=0;i<size;++i)
        {
            int n=sum-a[i];
            if (!hs.count(a[i]))//if(hm.count(a[i]))
            {
                if(hs.count(n))//if(hm.count(n))
                {
                    count++;
                    cout<<a[i]<<":"<<n<<endl;
                    //minRange=min(minRange,abs(hm[n]-i));//remove dup check, do not update if exist in hm for max range
                }
                hs.insert(a[i]);
                //hm.insert(a[i],i);
            }
        }
        return count;//return minRange;
    }
    //UNI#
    unsigned twoSumBitSet(int a[], size_t size, int sum)
    {
        bitset<128> bs;//assume num btw [0,100~2x64] in 64bit machine, each bit map to a num exist 1 or not 0
        unsigned count=0;
        for(unsigned i=0;i<size;++i)
        {
            int n=sum-a[i];
            if(!bs.test(a[i]))
            {
                if(bs.test(n))
                {
                    count++;
                    cout<<a[i]<<":"<<n<<endl;
                }
                bs.set(a[i]);//set(3) ...01000
            }
        }
        return count;
    }
    //UNI#length of the longest substring contains at most 2|K distinct characters, eceba ece(3)
    int lengthOfLongestSubstringDistinct(string s) {
        int res = 0, left = 0;
        unordered_map<char, int> mp;
        for (int i = 0; i < s.size(); ++i) {
            ++mp[s[i]];
            while (mp.size() > 2) {//keep only 2|k distinct count
                if (--mp[s[left]] == 0)
                    mp.erase(s[left]);//keep reduce from left till one can be removed to decrement mp size
                ++left;//as queue front ~ flipZeroToOneMaxOneArr
            }
            res = max(res, i - left + 1);
        }
        return res;
    }
    int longestUniqueSubstring(string s){
        unordered_map<char,int> mp;
        int right=0,left=right-1,max=0;
        string result;
        while(right!=s.length()-1){
            pair<unordered_map<char, int>::iterator, bool> pr = mp.insert(pair<char, int>(s.at(right), right));
            if (!pr.second){//if exists
                int cur = pr.first->second;
                left=max(left,pr.first->second);
                mp[cur]=s.at(right);//replace old value
            }
            /*simple
            if(mp.count(s[right])) {
                left = max(left, mp[s[right]]);
            }
            mp[s[right]]=right;
            */
            if(max<right-left){//max init = 1
                max=right-left;
                result=s.substr(left,max);
            }
            right++;
        }
        return max;
    }
    //1, 11, 21, 1211, 111221, 312211, 13112221, 1113213211, …
    //1 >one 1> 11 >two 1> 21 >one 2 one 1> 1211
    string countAndSay(int n) {
        if(n<1) return "";
        string ret = "1";
        for(int i=2; i<=n; i++) {
            string temp = "";
            int count = 1;
            char prev = ret[0];
            for(int j=1; j<ret.size(); j++) {
                if(ret[j]==prev)
                    count++;
                else {
                    temp += to_string(count);//2
                    temp.push_back(prev);//21
                    count = 1;//reset
                    prev = ret[j];
                }
            }
            temp += to_string(count);//for last 11 always
            temp.push_back(prev);
            ret = temp;
        }
        return ret;
    }
    string encode(string si){//~ countAndSay
        char prev;
        string sr;
        int ct=0;
        for(auto& c : si){
            if(prev==c)
                ++ct;
            else{
                if(ct>1)
                    sr.append(to_string(ct));
                sr.push_back(c);
                prev=c;
                ct=1;
            }
        }
        if(ct>1)
            sr.append(to_string(ct));//last char
        return sr;
    }
    string decode(string si){//ab2->abb
        char prev;
        string sr;
        int ct=0;
        for(auto& c : si){
            if (c>='0' && c<='9'){
                ct=10*ct+c-'0';
            }
            else{
                while(--ct>0)//discount 1st
                    sr.push_back(prev);
                sr.push_back(c);
                prev=c;
                ct=0;
            }
        }
        while(--ct>0)
            sr.push_back(prev);
        return sr;
    }
    //O(n) flip m 0to1 in binary array to maximize consecutive 1s
    void flipZeroToOneMaxOneArr(bool arr[], int n, int m)
    {
        int wL = 0, wR = 0;// Left and right indexes of current window
        int bestL = 0, bestWindow = 0;// Left index and size of the widest window
        int zeroCount = 0;// Count of zeroes in current window

        while (wR < n)//sliding window to contain m 0s, record the max case
        {
            // If zero count of current window is less than m, widen the window toward right
            if (zeroCount <= m)//= to keep moving thru whole array
            {
                if (arr[wR] == 0)
                    zeroCount++;
                wR++;
            }
            // If zero count of current window is more than m, reduce the window from left
            if (zeroCount > m)
            {
                if (arr[wL] == 0)
                    zeroCount--;
                wL++;
            }
            // Updqate widest window if this window size is more
            if (wR-wL > bestWindow)
            {
                bestWindow = wR-wL;
                bestL = wL;
            }
        }
        // Print positions of zeroes in the widest window
        for (int i=0; i<bestWindow; i++)
        {
            if (arr[bestL+i] == 0)
                cout << bestL+i << " ";
        }
    }
    static bool comparer(int a, int b){
        string sa=to_string(a);
        string temp=sa;
        string sb=to_string(b);
        sa = sa.append(sb);
        sb = sb.append(temp);
        if (sa.compare(sb) >=0){//<= for smallest
            return true;
        }else{
            return false;
        }
    }
    //[3, 30, 34, 5, 9], the largest formed number can be formed is 9534330
    string largestNumFormedFromArr(int num[], int size) {
        string res="";
        sort(num, num+size, comparer);
        if (num[0]==0){return "0";}
        for(int i=0;i<size;i++){
            res = res.append(to_string(num[i]));
        }
        return res;
    }
    //UNI#
    void printX(unsigned n){
        for(unsigned i=0;i<n;++i){
            for(unsigned j=0;j<n;++j){
                if(i==j||i+j==n-1)
                    cout<<'X';
                else
                    cout<<' ';
            }
            cout<<endl;
        }
    }
    void printXs(unsigned n,unsigned i,unsigned j){
        if(i==j||i+j==n-1)
            cout<<'X';
        else
            cout<<' ';
        if(j<n-1)//n-2 ++j=n-1
            printXs(n,i,++j);
        cout<<endl;
        j=0;
        if(i<n-1)
            printXs(n,++i,j);

    }
    //UNI#
    bool isValidBrackets(string str){
        stack<char> stk;
        char map[256] = {0};//init map[any char]=0
        map['{'] = 1;
        map['['] = 1;
        map['('] = 1;
        map['"'] = 2;
        map['}'] = '{';
        map[']'] = '[';
        map[')'] = '(';
        for (char cur:str) {
            char act = map[cur];
            if(act == 0)
                continue;
            else if(act == 1)
                stk.push(cur);
            else if(act == 2) {
                if ('"'== stk.top())
                    stk.pop();
                else
                    stk.push(cur);
            }
            else{
                if(stk.empty()||act!=stk.top())
                    return false;
                stk.pop();
            }
        }
        if (!stk.empty())
            return false;
        return true;
    }
    //regular expression matching ‘.’ for any single char and ‘*’ for zero or more of the preceding char
    //DP? http://bangbingsyb.blogspot.com/2014/11/leetcode-regular-expression-matching.html
    bool isMatchExp(const char *s, const char *p) {//long string stack overflow
        assert(s && p);
        if (*p == '\0') return *s == '\0';

        // next char is not '*': must match current character
        if (*(p+1) != '*') {
            assert(*p != '*');
            return ((*p == *s) || (*p == '.' && *s != '\0')) && isMatchExp(s+1, p+1);
        }
        // next char is '*'
        while ((*p == *s) || (*p == '.' && *s != '\0')) {
            if (isMatchExp(s, p+2)) return true;
            s++;
        }
        return isMatchExp(s, p+2);
    }
    //((a + b) * c ) + (a + b) > (a + b)*c + a + b, assume it is valid
    //or form expressionTree and flatten ref JAVA
    string removeDupFromExpressionTree(string str){
        stack<char> num;
        deque<char> sub;
        bool keep;
        string result;
        for (char cur:str) {
            if((sub.empty()||keep) && cur!='('){//start without ( or previous ( evaluated as keep
                num.push(cur);//proceed till next (
            }else {
                if (cur == '(') {
                    //if(sub.front()=='('){
                        //TODO:multiple layers (()) is handled recursively as single layer of ()
                    //}
                    keep=false;//reset keep
                    if (num.top() == '*' || num.top() == '/') {//*(...) keep
                        keep = true;
                        num.push(cur);//continue to put to main stack if keep==true
                    }
                    else {//+(
                        sub.push_back(cur);//put ( to deque
                    }
                } else if (cur == '+' || cur == '-') {//keep==false (...)+ remove
                    if (sub.end() == ')' && !keep) {//remove() and append rest of sub to main stack
                        sub.pop_back();
                        sub.pop_front();
                        while(!sub.empty()){
                            num.push(sub.front());
                            sub.pop_front();
                        }
                    }
                }
                else{
                    sub.push_back(cur);//put potential dup sub string ) to deque
                }
            }
        }
        while(!num.empty()){
            //result=num.top()+result;
        }
        return result;
    }
    void combWithAll(string prefix, string letters){
        if(letters.size()<=1){
            cout<<prefix.append(letters)<<endl;
        }else{
            for(auto it = letters.begin(); it != letters.end(); ) {
                prefix.append(string(1, *it));
                it = letters.erase(it);//can not simply use for(auto char c : letters) to erase
                combWithAll(prefix,letters);
            }
        }
    }
    void combNum(int num[],int size, vector<int>& list, vector<vector<int>>& result){
        if(list.size()==size) {
            result.push_back(list);
            return;
        }
        for(int i=0;i<size;++i){
            if(num[i]==-1)
                continue;
            list.push_back(num[i]);
            int preVal = num[i];
            num[i]=-1;
            combNum(num,size,list,result);
            num[i]=preVal;
            list.pop_back();
        }
    }
    //subtract any two numbers from list and add to result if > 0 and not exists in list till unable to
    //list init ={8,5} result = {85326174}, insert list to prev prior to call
    void subtractComb(vector<int>& list,unordered_set<int>& prev, vector<vector<int>>& result){
        bool ct=true;
        for(int i=0;i<list.size()-1;++i){
            int sub = abs(list.back()-list[i]);//recursively used last added item to substract rest
            if (sub != 0 && !prev.count(sub)){
                ct=false;
                prev.insert(sub);
                list.push_back(sub);
                subtractComb(list, prev, result);
                list.pop_back();
                prev.erase(sub);
            }
        }
        if(ct)//list is an exhausted path if nothing to change
            result.push_back(list);
    }
    //s = "catsanddog", sentences=["cats and dog", "cat sand dog"].
    void findWordBreak(string &s, unordered_set<string> &dict, int start, vector<string> &sol, vector<string> &sentences) {
        if(start==s.size() && !sol.empty()) {   // find a valid words break
            string temp = sol[0];
            for(int i=1; i<sol.size(); i++)
                temp.append(string(" ").append(sol[i]));
            sentences.push_back(temp);
        }
        string word;
        for(int i=start; i<s.size(); i++) {
            word.push_back(s[i]);
            if(dict.count(word)!=0) {//cat | cats go to [i+1] s | a
                sol.push_back(word);
                findWordBreak(s, dict, i+1, sol, sentences);
                sol.pop_back();
            }
        }
    }
    bool hasCycleList(ListNode* head)
    {
        ListNode* slow=head;
        ListNode* fast=head;
        while(fast!= nullptr && fast->next!= nullptr)
        {
            slow=slow->next;
            fast=fast->next->next;
            if(slow->value==fast->value)//slow==fast if value is not unique
            {
                return true;

                int len=0,loop=0;
                while(slow->value != fast->value)
                {
                    slow=slow->next;
                    loop++;
                }
                slow=head;
                while(slow->next->value!=fast->next->value)//slow->next!=fast->next if dup value
                {
                    slow=slow->next;
                    fast=fast->next;
                    len++;
                }
                return  len+loop;
            }
        }
        return false;
    }
    ListNode* reverseIterativeList(ListNode* head)
    {
        ListNode * cur = head->next;
        head->next = nullptr;
        while (cur){
            ListNode * tmp = cur->next;
            cur->next = head;
            head = cur;
            cur = tmp;
        }
        return head;
    }
    ListNode* reverseRecursiveList(ListNode* head)
    {
        if(head== nullptr||head->next== nullptr)
            return head;
        ListNode * cur = head->next;
        head->next = nullptr;
        ListNode* rest = reverseRecursiveList(cur);
        cur->next=head;
        return rest;
    }
    void printReverseListRecursive(ListNode* traverse)
    {
        if (traverse == nullptr)
            return;
        printReverseListRecursive(traverse->next);//recursive print last first
        std::cout << traverse->value << std::endl;
    }
    bool isPalindromeStack(ListNode* head) {
        if (!head || !head->next) return true;
        ListNode *slow = head, *fast = head;
        stack<int> s;
        s.push(head->value);
        while (fast->next && fast->next->next) {//must fast->next first to avoid null pointer on fast->next->next
            slow = slow->next;
            fast = fast->next->next;
            s.push(slow->value);
        }
        if (fast->next == nullptr)
            s.pop();//fast->next->next=null odd size fast->next==null skip mid item, even size fast->next!=null
        while (slow->next) {
            if (s.empty())
                return false;
            slow = slow->next;
            int tmp = s.top();
            s.pop();
            if (tmp != slow->value) return false;
        }
        return true;
    }
    //{1,2,3,4}, reorder to {1,4,2,3}.
    //bool listPalindrome (ListNode* head){ O(n) time and O(1) space
    void reOrderList(ListNode* head){
        if(head!= nullptr && head->next!= nullptr){
            ListNode* slow=head;
            ListNode* fast=head;
            while(fast->next!= nullptr && fast->next->next!= nullptr){
                slow=slow->next;
                fast=fast->next->next;
            }
            ListNode* mid=slow->next;//middle node
            slow->next= nullptr;//disconnect halves
            mid=reverseIterativeList(mid);
            while(mid!= nullptr && head!= nullptr){
                //head size=mid size or +1, latter case the last head item wont matter
                //if (head->value!= mid->value) return false;//isPalindrome without stack O(1) space
                //head=head->next;
                //mid=mic->next;
                ListNode* t1=head->next;
                ListNode* t2=mid->next;
                head->next=mid;
                mid->next=t1;
                head=t1;
                mid=t2;
            }
            //return true;
        }
    }
    //sorted by abs 1 -> -2 -> -3 -> 4 -> -5 to -5 -> -3 -> -2 -> 1 -> 4 sorted by val
    //>similar -2->1->3 square 4->1->9, sort to 1->4->9
    ListNode* reSortAbsList(ListNode* head) {
        ListNode *prev = head;
        ListNode *curr = head->next;
        //prev->value = pow(prev->value,2); //square head first
        while (curr != nullptr) {
            //curr->value = pow(curr->value,2); //square each node
            if (curr->value < prev->value) {
                prev->next = curr->next;// Detach curr from linked list
                curr->next = head;// Move current node to beginning
                head = curr;//since already sorted by abs -2 -> -3 will transform to -3 -> -2
                curr = prev;//temporarily point to prev to be later updated by the last line
            }
            else
                prev = curr;
            curr = curr->next;
        }
        return head;
    }
    //node contains an additional random pointer which could point to any node in the list or null.
    ListNode *copyRandomPtrList(ListNode *head) {
        if(!head) return NULL;
        //pointer can be key by comparing target memory address
        unordered_map<ListNode*, ListNode*> ht;//map old node to new copy, ref JAVA
        ListNode *p1 = head;
        ListNode *p2 = new ListNode(head->value);
        ht[head] = p2;
        while(p1) {//O(n)
            if(p1->next) {
                if(ht.count(p1->next))
                    p2->next = ht[p1->next];
                else {
                    p2->next = new ListNode(p1->next->value);//new next node
                    ht[p1->next] = p2->next;
                }
            }
            if(p1->random) {
                if(ht.count(p1->random))
                    p2->random = ht[p1->random];//if exist ht[p1->random] must be already copied random node
                else {
                    p2->random = new ListNode(p1->random->value);//new random node
                    ht[p1->random] = p2->random;
                }
            }
            p1 = p1->next;
            p2 = p2->next;
        }
        return ht[head];//copy of head
    }
    int helper(ListNode *node) {
        if (!node) return 1;//add 1 if reach the tail
        int carry = helper(node->next);//recursive add to tail instead of head
        int sum = node->value + carry;
        node->value = sum % 10;
        return sum / 10;//carry to next higher digit
    }
    //add 1 to 9->9->9 = 1->0->0->0
    ListNode* plusOneByList(ListNode* head) {
        if (!head) return head;
        int carry = helper(head);
        if (carry == 1) {//add 1-> to head
            ListNode *res = new ListNode(1);
            res->next = head;
            return res;
        }
        return head;
    }
    //https://github.com/panks/BigInteger/blob/master/BigInteger.cpp
    //123 (head)3->2->1, string ref JAVA
    //if 1->2->3 need reverse first
    ListNode* bigAddByList(ListNode* a, ListNode* b){
        ListNode* result;
        ListNode* temp=result;//result stay head
        int carry = 0;
        while(a!= nullptr || b!= nullptr){
            int va=0, vb=0;
            if (a!= nullptr)
                va=a->value;
            if (b!= nullptr)
                vb=b->value;
            int sum=va+vb+carry;
            temp=new ListNode((sum)%10);
            carry = (sum)/10;
            temp=temp->next;
            a=a->next;
            b=b->next;
        }
        if (carry>0)
            temp=new ListNode(carry);
        return result;
    }
    //or use stack to reverse 1->2->3
    ListNode* addListByStack(ListNode* a, ListNode* b) {
        ListNode *result;
        ListNode *temp;
        stack<ListNode *> sa;
        stack<ListNode *> sb;
        int carry = 0;
        while (a != nullptr) {
            sa.push(a);
            a = a->next;
        }
        while (b != nullptr) {
            sb.push(b);
            b = b->next;
        }
        while(!sa.empty() || !sb.empty()){
            int va=0, vb=0;
            if(!sa.empty()){
                va = sa.top()->value;
                sa.pop();
            }
            if(!sb.empty()) {
                vb = sb.top()->value;
                sb.pop();
            }
            int sum=va+vb+carry;
            temp=new ListNode((sum)%10);
            temp->next=result;//prepend list
            result=temp;
            carry = (sum)/10;
        }
        if (carry>0) {
            temp = new ListNode(carry);
            temp->next=result;
            result=temp;
        }
        return result;
    }
    //int value of a>=b
    //if str1.length < str2.length || (str1.length == str2.length  && str1.compareTo(str2) < 0))
    ListNode* bigSubByList(ListNode* a, ListNode* b){
        int va, vb;
        ListNode* ta=a;//a still at head
        ListNode* tb=b;
        while(ta!= nullptr && tb!= nullptr){
            ta=ta->next;
            tb=tb->next;
        }
        //if int value of b>a, return -(b-a)
        if(ta == nullptr){
            if (tb!= nullptr){//b long than a
                return bigSubByList(b,a);
            }else {//same length, reiterate comp higher decimal for b > a
                ta=a;
                tb=b;
                while(ta!= nullptr) {
                    if (ta->value < tb->value)
                        return bigSubByList(b, a);
                    ta=ta->next;
                    tb=tb->next;
                }
            }
        }
        //highest digit at head, reverse before subtract
        ListNode* result;
        ListNode* temp=result;
        int borrow = 0;
        while(a!= nullptr){
            int vb=0;//for digits where a exceeds b, consider a-0
            if (b!= nullptr)
                vb=b->value;
            if(a->value - borrow >= vb){
                temp=new ListNode(a->value - vb - borrow);
            } else{
                temp=new ListNode(a->value - vb + 10);
                borrow=1;
            }
            temp=temp->next;
            a=a->next;
            if (b!= nullptr)
                b=b->next;
        }
        return result;
    }
    Node* recursiveMergeList( Node* a, Node* b )
    {
        if (a == nullptr)
            return b;//return the other that is not null
        if (b == nullptr)
            return a;
        Node* result;
        if (a->data < b->data){//put the smaller one in result
            result = a;
            result->down = recursiveMergeList( a->down, b );//or iterateMergeList
        }
        else{
            result = b;
            result->down = recursiveMergeList( a, b->down );
        }
        return result;
    }
    /* 5 7 8 10 19 20 20 22 30 35 40 45 50
   each down list are already sorted
   5 -> 10 -> 19 -> 28
   |    |     |     |
   V    V     V     V
   7    20    22    35
   |          |     |
   V          V     V
   8          50    40
   |                |
   V                V
   30               45
    */
    Node* flatten2DList(Node* root)
    {
        if (root == nullptr || root->right == nullptr)
            return root;
        return recursiveMergeList( root, flatten2DList(root->right) );//recusive merge all vertical lists
    }
    //search value in 4-directions linked list
    Node* find(Node* root, int val, unordered_set<Node*>& hist){
        if(root==nullptr || hist.count(root)>0)//set to avoid redo node
            return nullptr;
        if(root->data==val)
            return root;
        hist.insert(root);
        Node* up= find(root->up,val,hist);
        if(up)
            return up;
        Node* down=find(root->down,val,hist);
        if(down)
            return down;
        Node* prev=find(root->left,val,hist);
        if(down)
            return prev;
        Node* next=find(root->right,val,hist);
        if(next)
            return next;
        hist.erase(root);
    }
    int maxOverlap(Interval intervals[], int size)
    {
        customLess comp;
        std::sort(intervals, intervals + size,comp);//sort(v, v + sizeof(v) / sizeof (v[0])); begin|end(intervals) not with ptr
        priority_queue<int> pq;
        int count =1;
        pq.push(intervals[0].End);
        for(int i=1;i<size;++i)
        {
            if(intervals[i].Start<pq.top())
            {
                count++;
            }
            else
            {
                pq.pop();
            }
            pq.push(intervals[i].End);
        }
        return count;

    }
    int getTreeLevelOfNodeValue(TreeNode *ptr,int val,int level) {
        if (ptr == NULL)
            return 0;
        if(ptr->value < val){
            ptr=ptr->left;
        }
        else if(ptr->value > val){
            ptr=ptr->right;
        }
        if (ptr->value == val) {
            return level;
        }
        return getTreeLevelOfNodeValue(ptr, val, level+1);
    }
    void inOrderIterative(TreeNode* currRoot)
    {
        if(currRoot== nullptr)
            return;
        stack<TreeNode> stk;//stack<TreeNode*> stk;
        TreeNode* p=currRoot;
        while(!stk.empty() || p!= nullptr)
        {
            if(p!= nullptr)
            {
                stk.push(*p);//stk.push(p);
                p=p->left;
            }
            else
            {
                TreeNode t = stk.top();//TreeNode* t = stk.top();
                stk.pop();
                cout<<t.value<<endl;//cout<<t->value<<endl;
                //ListNode* cur=new ListNode(t->value);
                //head->next=cur; head=cur; convert tree to single|double linked list
                p=t.right;//p=t->right;
            }
        }
    }
    TreeNode* arrayToMaxTree(int a[])
    {
        stack<TreeNode*> stk;//use ptr for less mem
        int size = sizeof(a)/ sizeof(a[0]);
        for(int i=0;i<size;++i)
        {
            TreeNode* node=new TreeNode(a[i]);//must on heap to return ptr out of { }
            while(!stk.empty() && node->value>=stk.top()->value)
            {
                node->left=stk.top();
                stk.pop();
            }
            if(!stk.empty())
            {
                stk.top()->right=node;
            }
            stk.push(node);
        }
        TreeNode* rest=stk.top();
        stk.pop();
        while(!stk.empty())
        {
            rest=stk.top();
            stk.pop();
        }
        return rest;
    }
    //UNI# second largest node from BST
    TreeNode* SecLargeNode(TreeNode* root){
        TreeNode* cur=root;
        while (cur->right != nullptr) {
            root = cur;
            cur = cur->right;
        }//go to the rightest/largest
        if (cur->left == nullptr)
            return root; //parent of largest
        TreeNode* leftsub = cur->left;
        while (leftsub->right != nullptr) {
            leftsub = leftsub->right;
        } //go to the smallest left to cur
        return leftsub;
    }
    //UNI#http://www.ideserve.co.in/learn/remove-out-of-range-bst-nodes
    TreeNode* removeOutOfRangeNodes(TreeNode* root, int min, int max){
        if(root==nullptr)
            return nullptr;
        root->left= removeOutOfRangeNodes(root->left,min,max);
        root->right= removeOutOfRangeNodes(root->right,min,max);
        if(root->value<min)
            return root->right; //post-order traversal, left<min already be removed
        if(root->value>max)
            return root->left;
        return root;//keep if in range
    }
    TreeNode *buildTreeIterative(vector<int> &preorder, vector<int> &inorder) {
        if (preorder.empty()) {
            return NULL;
        }
        TreeNode *root = new TreeNode(preorder[0]);
        stack<TreeNode *> myStack;
        myStack.push(root);
        int j = 0;
        for (int i = 1; i < preorder.size(); i++) {
            TreeNode *curr = myStack.top();
            if (curr->value != inorder[j]) {
                curr->left = new TreeNode(preorder[i]);
                myStack.push(curr->left);
            } else {
                while (!myStack.empty() && myStack.top()->value == inorder[j]) {
                    curr = myStack.top();
                    myStack.pop();
                    j++;
                }
                if (j < inorder.size()) {
                    curr->right = new TreeNode(preorder[i]);
                    myStack.push(curr->right);
                }
            }
        }
        return root;
    }
    //remove node having single child
    void removeTreeNodeWithSingleChild(TreeNode* root){
        queue<TreeNode*> q;
        unordered_map<int,pair<TreeNode*,bool>> p;//store parent and from its left|right
        q.push(root);
        while(!q.empty()){
            TreeNode* n=q.front();
            q.pop();
            if(n->left!= nullptr) {
                q.push(n->left);
                p[n->left->value] = make_pair(root, true);
                if (n->right == nullptr) {//only left child
                    pair<TreeNode *, bool> pn = p[n->value];
                    if (pn.second)//from parent's left
                        pn.first->left = n->left;
                    else//from parent's right
                        pn.first->right = n->left;
                    delete n;
                }
            }
            if(n->right!= nullptr){
                q.push(n->right);
                p[n->right->value]=make_pair(root,false);
                if(n->left == nullptr){//only right child
                    pair<TreeNode*,bool> pn = p[n->value];
                    if(pn.second)//from parent's left
                        pn.first->left=n->right;
                    else//from parent's right
                        pn.first->right=n->right;
                    delete n;
                }
            }
        }
    }
    //recursive ref JAVA
    TreeNode* IterativeMirror(TreeNode*root)
    {
        if (root== nullptr)
            return nullptr;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty())
        {
            TreeNode* parent = q.front();
            q.pop();
            //swap(parent->left, parent->right);
            TreeNode* temp = parent->right;
            parent->right = parent->left;
            parent->left = temp;

            if (parent->left != nullptr)
                q.push(parent->left);
            if (parent->right != nullptr)
                q.push(parent->right);
        }
        return root;
    }
    TreeNode* createMirror(TreeNode* root){
        TreeNode* t=new TreeNode(root->value);
        t->left=createMirror(root->right);
        t->right=createMirror(root->left);
        return t;
    }
    //convert char[] '12' to int 12, a!=b not always atoi(a)!=atoi(b) because str has no limit but int has
    int atoi(const char *str)
    {
        int res = 0;
        for (int i = 0; str[i] != '\0'; ++i)
            res = res*10 + str[i] - '0';
        return res;
    }
    int stoi(string str) {
        char sign = '+';
        int i = 0;
        if (str[0] == '-') {
            sign = '-';
            i++;
        } else if (str[0] == '+') {
            i++;
        }
        double result = 0;
        while (str.length() > i && str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
            i++;
        }
        if (sign == '-')
            result = -result;
        if (result > INT_MAX)
            return INT_MAX;
        if (result < INT_MIN)
            return INT_MIN;
        return (int) result;
    }
    char* reverseStr(char str[], int len)//void ok str refer back
    {
        int start, end;
        char temp;
        for(start=0, end=len-1; start < end; start++, end--) {
            temp = *(str+start);
            *(str+start) = *(str+end);
            *(str+end) = temp;
        }
        return str;
    }
    char *itoa(int n, char *s, int b) {
        const char digits[] = "0123456789";
        int i=0; bool sign;
        if (n < 0) {
            n = -n;
            sign=false;
        }
        while ((n /= 10) > 0){
            s[i++] = digits[n % 10];
        }
        if (sign < 0)
            s[i++] = '-';
        s[i] = '\0';
        reverseStr(s,i);
        return s;
    }
    void reverse(string &s, int left, int right) {
        while (left < right) {
            char t = s[left];
            s[left] = s[right];
            s[right] = t;
            ++left; --right;
        }
    }
    void reverseWords(string &s) {//reverse each word then reverse whole
        int left = 0;
        for (int i = 0; i <= s.size(); ++i) {
            if (i == s.size() || s[i] == ' ') {
                reverse(s, left, i - 1);
                left = i + 1;//next to ' '
            }
        }
        reverse(s, 0, s.size() - 1);
    }
    //2|5:..64bit..0010|0101 bit(pos) 1|0 is the lowest bit set (1) count from right
    int lowestBitSet(int value) {
        if(!value)//0
            return -1;
        unsigned pos=0;
        while((value&1)==0){//shift till 1, optimize by hop multiple bits?
            value>>1;
            pos++;
        }
        return pos;
    }
    //count of 1s
    unsigned int countSetBits(int n)
    {
        unsigned int count = 0;
        while (n)//O(logn) better than thru each bit
        {
            n=n&(n-1);//unset the righmost set ( last significant ) bit XXX10000 -> XXX00000. so no of times loop = set bit count
            count++;
        }
        return count;
    }
    //C++
    bool isCylicGraph(GraphNode* root)
    {
        unordered_set<GraphNode*> hs;
        queue<GraphNode*> gq;
        gq.push(root);
        while(!gq.empty())
        {
            GraphNode* gn=gq.front();
            gq.pop();
            if(hs.find(gn)!=hs.end())
                return true;
            hs.insert(gn);
            for(auto nb : gn->directs)
            {
                gq.push(nb);
            }
        }
        return false;
    }
    bool isCyclicGraphDFS(GraphNode* root,unordered_set<GraphNode*>& hs){//~dfsFlight
        if(root==nullptr)
            return true;
        for(GraphNode* d:root->directs){
            if(hs.count(d))
                return false;
            hs.insert(d);
            return isCyclicGraphDFS(d,hs);
            hs.erase(d);
        }
        return true;
    }
    GraphNode *cloneGraph(GraphNode *node) {
        if(!node) return NULL;
        GraphNode *p1 = node;
        GraphNode *p2 = new GraphNode(node->value);
        unordered_map<GraphNode*, GraphNode*> ht;
        queue<GraphNode*> q;//DFS using stack
        q.push(node);
        ht[node] = p2;

        while(!q.empty()) {
            p1 = q.front();
            p2 = ht[p1];//must exist
            q.pop();
            for(int i=0; i<p1->directs.size(); i++) {
                GraphNode *nb = p1->directs[i];
                if(ht.count(nb)) {
                    p2->directs.push_back(ht[nb]);
                }
                else {
                    GraphNode *temp = new GraphNode(nb->value);
                    p2->directs.push_back(temp);
                    ht[nb] = temp;
                    q.push(nb);//only queue if not exist in ht
                }
            }
        }
        return ht[node];
    }
    //find path with layer < maxLayer and sum weight thru the same path > minWeight, O(maxLayer^avg size weightDirects)
    vector<string> findPathsMaxLayerMinWeight(GraphNode* source, int maxLayer, int minWeight){
        vector<GraphNode*> ret;
        unordered_map<GraphNode*,int> sumWeights;//store sum weight on current path on topof max possible weight from prev node
        priority_queue<pair<GraphNode*,int>> pq;//store max possible weight and ordered by max, omit comp
        pq.push(make_pair(source,0));
        int layers=0; //BFS increment over each layer
        while(!pq.empty() && layers<maxLayer){
            for(int i=0; i<pq.size(); ++i) {//pop up all nodes in current layer, only need size if counting layers
                pair<GraphNode*, int> cur = pq.top();
                pq.pop();
                if (cur.second > minWeight)
                    ret.push_back(cur.first);
                //priority_queue for muliple upstream node have same downstream node,
                //calc on better up node first minimize update on base of other upstream node
                for (pair<GraphNode*, int> dir : cur.first->weightDirects) {
                    if (sumWeights.count(dir.first)){//sumWeight from a different path
                        if (sumWeights[dir.first] < dir.second + cur.second){//updt if cur path > prev path to this node
                            sumWeights[dir.first] = dir.second + cur.second;
                        }
                    }else{
                        sumWeights[dir.first]=dir.second + cur.second;
                    }
                    //sumWeights[dir.first->value] has best weight across multiple path to this node so far
                    pq.push(make_pair(dir.first,sumWeights[dir.first]));
                }
            }
            layers++;
        }
    }
    //find a route that can go through all the edges in the graph
    //tickets = [["JFK","SFO"],["JFK","ATL"],["SFO","ATL"],["ATL","JFK"],["ATL","SFO"]]
    //Return ["JFK","ATL","JFK","SFO","ATL","SFO"]
    void dfsFlight(unordered_map<string, priority_queue<string>>& graph, vector<string>& result, string start)//multiset<string>>& graph
    {
        while(!graph[start].empty())
        {
            //string next = *graph[start].begin();//begin() is iterator, *iterator to get the value
            //graph[start].erase(graph[start].begin());
            string next = graph[start].top();
            graph[start].pop();
            dfsFlight(graph, result, next);
        }
        result.push_back(start);//first exhaust downstream is last dest ?? AB AC CB BD :  AC CB BD not AB BD
    }
    vector<string> findItineraryFlight(vector<pair<string, string>> tickets)
    {
        vector<string> result;
        unordered_map<string, priority_queue<string>> graph;//priority_queue<string> | multiset<string> may have same ticket more than one
        for (auto& p : tickets)
            graph[p.first].push(p.second);//insert(p.second); for multiset<string>
        dfsFlight(graph, result, "JFK");
        std::reverse(result.begin(), result.end());
        return result;
    }
    //comp(A BC,ZBC)>0 comp(A BC,ABC)=0
    int comp(const char* c1,const char* c2){
        while(c1!=nullptr && c2!=nullptr){
            if(*c1!=' ' && *c2!= ' '){
                if (*c1<*c2) {
                    return 1;
                }
                else if(*c1<*c2){
                    return -1;
                }
                else if(*c1==*c2){
                    c1++;
                    c2++;
                    continue;//every char need to be check if equal
                }
            }
            else if(*c1==' '){
                c1++;
            }
            else if(*c2==' '){
                c2++;
            }
        }
        if(c1==nullptr && c2==nullptr){//same length
            return 0;
        }
        else if(c1==nullptr){//c1 shorter
            return 1;
        }
        else if(c2==nullptr){//c2 shorter
            return -1;
        }
    }
    //try change each char to a-z, and return all dict contains  O(26*strlen)
    //better than try change each word in dict O(dictsize*strlen)
    vector<string> findNeighbors(string s, unordered_set<string> &dict) {
        vector<string> ret;
        for(int i=0; i<s.size(); i++) {
            char c = s[i];
            for(int j=0; j<25; j++) {
                if(c=='a'+j) //skip same
                    continue;
                s[i] = 'a'+j;//char int value
                if(dict.count(s)) {
                    ret.push_back(s);
                    dict.erase(s);//each word can be used once, avoid hog->hot->hog endless
                }
            }
            s[i] = c;//restore
        }
        return ret;
    }
    //"hit","hot","dot","dog","cog" change one char a time based on given dict
    int wordLadder(string start, string end, unordered_set<string> &dict) {
        //unordered_map<string,vector<string> >prePath; //if return path
        //unordered_set<string> visited; //avoid duplicate word per path, clear after find a path
        dict.insert(end);
        queue<pair<string,int>> q;
        q.push(make_pair(start,1));
        while(!q.empty()) {
            string s = q.front().first;
            int len = q.front().second;
            if(s==end)
                return len;//first is shortest
            q.pop();
            vector<string> neighbors = findNeighbors(s, dict);
            for(int i=0; i<neighbors.size(); i++)
                q.push(make_pair(neighbors[i],len+1));
        }
        return 0;
    }

    //if an element is 0, set its entire row and column to 0
    void setMatrixZeroeByRowCol(vector<vector<int> > &matrix) {
        if (matrix.empty() || matrix[0].empty()) return;
        int m = matrix.size(), n = matrix[0].size();
        bool rowZero = false, colZero = false;
        for (int i = 0; i < m; ++i)//prestore if  top row | left col has 0
            if (matrix[i][0] == 0) colZero = true;
        for (int i = 0; i < n; ++i)
            if (matrix[0][i] == 0) rowZero = true;
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (matrix[i][j] == 0) {//use top row | left col to store if col | row has 0
                    matrix[0][j] = 0;
                    matrix[i][0] = 0;
                }
            }
        }
        for (int i = 1; i < m; ++i) {
            for (int j = 1; j < n; ++j) {
                if (matrix[0][j] == 0 || matrix[i][0] == 0) {
                    matrix[i][j] = 0;//must on 2nd pass to
                }
            }
        }
        if (rowZero) {
            for (int i = 0; i < n; ++i) matrix[0][i] = 0;
        }
        if (colZero) {
            for (int i = 0; i < m; ++i) matrix[i][0] = 0;
        }
    }
    void spiralMatrix(int m, int n, int a[R][C]) {
        int i, k = 0, l = 0;
        while (k < m && l < n) {
            for (i = l; i < n; ++i) {
                printf("%d ", a[k][i]);//first row
            }
            k++;//increment starting row index because printed first row
            for (i = k; i < m; ++i) {
                printf("%d ", a[i][n - 1]);//last column
            }
            n--;//decrement ending column index because printed last column
            if (k < m) {//avoid dup, if k==m the row already printed by k
                for (i = n - 1; i >= l; --i) {
                    printf("%d ", a[m - 1][i]);//last row
                }
                m--;//decrement ending row index because printed last row
            }
            if (l < n) {//avoid dup
                for (i = m - 1; i >= k; --i) {
                    printf("%d ", a[i][l]);//first column
                }
                l++;//increment starting column index because printed first column
            }
        }
    }
    const int R=10,C=5;
    void spiralOrder(int x, int y, int m, int n,int a[R][C]){
        if(m<=0||n<=0)
            return;
        if(m==1&&n==1) {//only one element left
            printf("%d ",a[x][y]);
            return;
        }
        for(int i=0;i<n-1;i++){
            printf("%d ",a[x][y++]);//top - move right
        }
        for(int i=0;i<m-1;i++){
            printf("%d ",a[x++][y]);//right - move down
        }
        if(m>1){
            for(int i=0;i<n-1;i++){
                printf("%d ",a[x][y--]);//bottom - move left
            }
        }
        if(n>1){
            for(int i=0;i<m-1;i++){
                printf("%d ",a[x--][y]);//left - move up
            }
        }
        if(m==1||n==1)
            spiralOrder(x, y, 1, 1, a);
        else
            spiralOrder(x+1, y+1, m-2, n-2, a);
    }
    // (0, 0), then (0, 1) -> (1, 0) sums up to 1. Then (2, 0)->(1, 1)->(0, 2) sums up to 2.
    void ZigZag(int** matrix[R][C] ) {
            int i, j;
            int index = 0;
            for (int sum = 0; sum <= R + C - 2; sum++) {
                if (sum % 2 == 0) {
                    i = min(sum, R - 1);//[2,0]
                    j = sum - i;
                    while (i >= 0 && j < C) {
                        cout<<matrix[i--][j++];//to up-right
                    }
                } else {
                    j = min(sum,C - 1);//[0,1]
                    i = sum - j;
                    while (j >= 0 && i <R) {
                        cout<<matrix[i++][j--];//to down-left
                    }
                }
            }
        }
    /*
            10(0)
            /  \
         5(1)  3(2)
        /   \
     4(3)    1(4)
    */
    // To heapify a subtree rooted with node i, heapify childrean subtree first
    void heapify(int arr[], int n, int i)
    {
        int largest = i;  // Initialize largest as root
        int l = 2*i + 1;  // left = 2*parent + 1
        int r = 2*i + 2;  // right = 2*parent + 2
        //i=(r|l-1)/2
        if (l < n && arr[l] > arr[largest])
            largest = l;
        if (r < n && arr[r] > arr[largest])
            largest = r;//index at max(arr[l],arr[r])
        if (largest != i){
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);// Recursively heapify the affected sub-tree
        }
    }
    void heapSort(int arr[], int n)
    {
        for (int i = n / 2 - 1; i >= 0; i--)
            heapify(arr, n, i);
        for (int i=n-1; i>=0; i--){
            swap(arr[0], arr[i]);// Move current root to end
            heapify(arr, i, 0);// call max heapify on the reduced heap
        }
    }
};

struct ListNode {
    int value;
    ListNode *next;
    ListNode *random;//copyRandomList
    ListNode(int x) : value(x), next(nullptr) { }
};
struct TreeNode{
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val):value(val){}
};
struct GraphNode{
    string value;
    vector<GraphNode*> directs;
    vector<pair<GraphNode*,int>> weightDirects;
    GraphNode(string  val):value(val){}
};
struct Node{
    int data;
    struct Node *right;
    Node *left;
    Node *up;
    struct Node *down;
};
struct Interval {
    int Start,End;
    Interval(int start,int end):Start(start),End(end){};
};

struct customLess{
    bool operator()(const Interval& a, const Interval& b)
    {
        return a.Start < b.Start;
    }
};

class LRUCache{
private:

struct ListNode{
  string key;
  int value;
  ListNode* prev;
  ListNode* next;
  ListNode(string k, int v):key(k),value(v),prev(nullptr),next(nullptr){}
};

    int capacity;
    unordered_map<int, ListNode*> mp;
    ListNode* head= nullptr;
    ListNode* tail= nullptr;
public:
    LRUCache(int cap):capacity(cap){};
    int get(string key)
    {
        if(mp.find(key)!=mp.end())
        {
            auto np=mp[key];//ListNode* np=mp.at(key);
            remove(np);
            setHead(np);
            return np->value;
        }
        return -1;
    }

    void set(string key, int value)
    {
        if(mp.find(key)!=mp.end())
        {
            ListNode* op=mp.at(key);
            op->value=value;
            remove(op);
            setHead(op);
        }
        else
        {
            //auto cp=unique_ptr<ListNode>(new ListNode(key,value)); //auto delete 
            ListNode* cp=new ListNode(key,value);
            if(mp.size()>=capacity)
            {
                mp.erase(tail->key);
                ListNode* td=tail;
                remove(tail);
                delete td; //manual delete node that tail used to point before move
                setHead(cp);
            }
            else
            {
                setHead(cp);
            }
            mp.insert(make_pair(key,cp));//mp[cp->key]=cp;
        }
    }

      void remove(ListNode* np)
    {
        if(np->prev!= nullptr)
            np->prev->next=np->next;
        else //np is tail
            tail=np->next;

        if(np->next!= nullptr)
            np->next->prev=np->prev;
        else //np is head
            head=np->prev;
        //dont delet np maybe setHead to it later
    }
    void setHead(ListNode* np)
    {
        np->prev=head;
        np->next=nullptr;
        if(head!=nullptr)
            head->next=np;
        head=np;
        if(tail==nullptr) //1st node
            tail=head;
    }
};

struct HashEntry{
    int value;
    int key;
    HashEntry *next;//seperate chaining
    HashEntry(){}
    HashEntry(int k, int val):key(k), value(val){this->next = nullptr;}
};
static const int TABLE_SIZE = 128;//2^n
class HashMap {
private:
    HashEntry **table;//array is pointer underneath, JAVA HashEntry[] table;
public:
    HashMap() {
        table = new HashEntry*[TABLE_SIZE];
        for (int i = 0; i < TABLE_SIZE; i++)
            table[i] = nullptr;
    }
    int get(int key) {
        int hash = (key % TABLE_SIZE);
        //linear probe
        while (table[hash] != nullptr && table[hash]->key != key)
            hash = (hash + 1) % TABLE_SIZE;

        if (table[hash] == nullptr)
            return -1;
        else {
            //linear probe
            return table[hash]->value;
            //seperate chaining
            HashEntry *entry = table[hash];
            while (entry != nullptr && entry->key != key)
                entry = entry->next;
            if (entry == nullptr)
                return -1;
            else//entry->key != key
                return entry->value;
        }

    }
    void put(int key, int value) {
        int hash = (key % TABLE_SIZE);//string key hashcode ref JAVA

        //linear probe
        while (table[hash] != nullptr && table[hash]->key != key)
            hash = (hash + 1) % TABLE_SIZE;//hash collision linear probe try next slot
        if (table[hash] == nullptr)
            table[hash] = new HashEntry(key, value);
        else//table[hash]->key == key
            table[hash]->value = value;

        //seperate chaining
        HashEntry *entry = table[hash];
        while (entry != nullptr && entry->key != key)
            entry = entry->next;
        if (entry == nullptr)
            entry=new HashEntry(key, value);
        else//entry->key == key
            entry->value=value;
    }
    ~HashMap() {
        for (int i = 0; i < TABLE_SIZE; i++)
            if (table[i] != nullptr)
                //linear probe
                delete table[i];

                //seperate chaining
                //HashEntry *prevEntry = nullptr;
                //HashEntry *entry = table[i];
                //while (entry != nullptr) {
                //    prevEntry = entry;
                //    entry = entry->next;
                //    delete prevEntry;
                //}
        delete[] table;
    }
};
class TrieNode {
public:
    TrieNode *child[26];
    bool isLeaf;
    TrieNode() : isLeaf(false){
        for (auto &a : child)
            a = nullptr;
    }
};
class Trie {
public:
    Trie() {
        root = new TrieNode();
    }
    void insert(string s) {
        TrieNode *p = root;
        for (auto &a : s) {
            int i = a - 'a';
            if (!p->child[i])//index to char
                p->child[i] = new TrieNode();
            p = p->child[i];
        }
        p->isLeaf = true;
    }
    bool startsWith(string prefix) {
        TrieNode *p = root;
        for (auto &a : prefix) {
            int i = a - 'a';
            if (!p->child[i])
                return false;
            p = p->child[i];
        }
        return true;//return p->isLeaf; return p;
    }
private:
    TrieNode* root;
};

#endif //CAPP_ARRLSTSTK_H
