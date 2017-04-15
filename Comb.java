package com.bbg.test;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

import com.bbg.test.Sort;

public class Comb {
	private static final String[] KEYS = {"", "", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};
	
	public static void main(String[] args) throws IOException {
		String str = "abc";
		char[] cr = str.toCharArray();
		//combWithAll("",str);
		//combWithLen("",2,str);
		//combWithOrder("",str);
		//List<String> ret = combAllUnique(str);

//		List<String> strings = new ArrayList<String>();
//		strings.add("A");
//		strings.add("B");
//		strings.add("C");
//		List<List<String>> result = combWordsUnique(strings);

		
//        char[] string = {'a', 'b', 'c'};
//        char[] buff = new char[string.length];
//        for(int k=1;k<=string.length;k++) {
//            combAll(string,0,k,buff);
//        }
        
//		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
//		ArrayList<String> result = new ArrayList<String>();
//		ArrayList<Character> temp = new ArrayList<Character>();
//		String s;
//		while ((s = in.readLine()) != null) {
//			dailNumToStr(s, temp, result);
//			return;
//		}
		
		
		ArrayList<ArrayList<Integer>> result = new ArrayList<ArrayList<Integer>>();
		ArrayList<Integer> list = new ArrayList<Integer>();
		int[] ia={1,2,3};
		combNum(ia,list,result); 
		
//		ArrayList<ArrayList<Integer>> result =  subsets(new int[]{1,2,3});
		return;
	}
	
	//abc acb bac bca cab cba 
	static void combWithAll(String prefix, String letters) {
		if (letters.length() <= 1) {
			System.out.println(prefix + letters);
		} else {
			for (int i = 0; i < letters.length(); i++) {
				String newLetters = letters.substring(0, i) + letters.substring(i + 1);//take out the current char
				String newPrefix = prefix + letters.charAt(i);
				combWithAll(newPrefix, newLetters);
			}
		}
	}
	
	//ab ac ba bc ca cb
	static void combWithLen(String prefix, int length, String letters) {
		if (length == 0) {
			System.out.println(prefix);
			return;
		}
		for (int i = 0; i < letters.length(); i++) {
			String newLetters = letters.substring(0, i) + letters.substring(i + 1);
			String newPrefix = prefix + letters.charAt(i);
			combWithLen(newPrefix, length - 1, newLetters);//combWithLen(newPrefix, length - 1, letters); aa ab ac bb ba bc cc ca cb
		}
	}
	
	//000 =	001 = C	010 = B	011 = BC 100 = A 101 = AC 110 = AB	111 = ABC  2 (0/1)  power 3 (2^3) = 8 ways
//	static void combWithOrder(char[] str) {
//		int n = str.length;
//		int num = 1<<n;//1<<3 left shift 1(0001) by 3 bit position = 8(1000) (int)Math.pow(2,3)
//		for(int i =1; i< num ; i++){ // i= 001 to 111, exclude 0000 and 1000
//		    int j, pos;
//		    StringBuilder sb = new StringBuilder();
//		    for (j=1, pos=1; j < num; j<<=1, pos++){ //Finds the bits that are set
//		        if (i & j){
//	        		sb.append(str[pos-1]);  //You can print s[n-pos] to print according to bit position
//		        }
//		    }   
//		    System.out.println(sb);
//		}
//	}	
	//a	ab	abc	ac	b	bc	c
    static void combWithOrder(String prefix, String s) {
    	System.out.println(prefix);
    	//i(0) a ab	abc ac i(1) b bc i(2) c
        for (int i = 0; i < s.length(); i++)
        	combWithOrder(prefix + s.charAt(i), s.substring(i + 1));
    }  
    //"abc" -> {"a", "b", "c", "ab", "bc", "abc"}
    static void combIncrement(String s) {
    	int step =0;
    	while(++step<s.length()){
        for (int i = 0; i < s.length(); i=i+step)
        	System.out.println(s.substring(i,i+step));
    	}
    }
	
	//a, ba, cba, ca, bca, b, ab, cab, cb, acb, c, ac, bac, bc, abc
	static List<String> combAllUnique(String inStr) 
	{
	  if (inStr.length() > 1) 
	  {
	    List<String> result = new ArrayList<String>();
	    for (int i=0; i<inStr.length(); i++)
	    {
	      char str = inStr.charAt(i);
	      String subStr = inStr.substring(0, i) + inStr.substring(i+1);
	      result.add(Character.toString(str));
	      for (String combinations : combAllUnique(subStr)) 
	      {
	        combinations= combinations + str;
	        result.add(combinations);
	      }
	    }
	    return result;
	  }
	  else 
	  {
	    List<String> result = new ArrayList<String>();
	    result.add(inStr);
	    return result;
	  }
	}
	
    static List<List<String>> combWordsUnique(List<String> strings) 
    {
      if (strings.size() > 1) 
      {
        List<List<String>> result = new ArrayList<List<String>>();

        for (String str : strings) 
        {
          List<String> subStrings = new ArrayList<String>(strings);
          subStrings.remove(str);

          result.add(new ArrayList<String>(Arrays.asList(str)));

          for (List<String> combinations : combWordsUnique(subStrings)) 
          {
            combinations.add(str);
            result.add(combinations);
          }
        }

        return result;
      }
      else 
      {
        List<List<String>> result = new ArrayList<List<String>>();
        result.add(new ArrayList<String>(strings));
        return result;
      }
    }
	
	//a b c aa ab ac ba bb bc 
	public static void combAll(char[] s, int i, int k, char[] buff) {
		if (i < k) {
			for (int j = 0; j < s.length; j++) {
				buff[i] = s[j];
				combAll(s, i + 1, k, buff);
			}
		} else {
			System.out.println(String.valueOf(buff));
		}
	}
	//depth-first search : dial 23 returns [ad, ae, af, bd, be, bf, cd, ce, cf] 
	//[add validate against dictionary/hashset, plus put words in trie to check against prefix before recurse further] 
	static void dailNumToStr(String digits, ArrayList<Character> list, ArrayList<String> result){
	    if(digits.length() == 0){
	        char[] arr = new char[list.size()];
	        for(int i=0; i<list.size(); i++){
	            arr[i] = list.get(i);
	        }
	        result.add(String.valueOf(arr));//concat all items: {a,d} to ad,[if hashset.contains(ad) result.add(ad)]
	        return;//otherwise run out of end digits
	    }
	 
	    Integer curr = Integer.valueOf(digits.substring(0,1));//use 1st char as prefix
	    String letters = KEYS[curr];//fxp:abc[2] bef[3]
	    for(int i=0; i<letters.length(); i++){//go thru all possible values for current level 
	        list.add(letters.charAt(i));
	        //1st recursion: dailNumToStr("3",[a],[]), 2nd recursion: 1st loop dailNumToStr("",[a,d],[]) 2nd loop dailNumToStr("",[a,e],[])
	        //each recursion move 1 char forward, substring is immutable so isolated in recursion: 23 to 3), but list and result pass same instance
	        //[if (trie contains word of list) then recursive call]
	        dailNumToStr(digits.substring(1), list, result); 
	        list.remove(list.size()-1);//remove the last/current item to loop in the next items at current level: a+d>e>f
	    }
	}
	//combSum:find all unique combinations (subset) in num where the candidate numbers sums to target
	//combNum:find all permutations without marking visited 11 12 21 22
	//combNum:find all unique permutations with marking visited [1, 2, 3], [1, 3, 2], [2, 1, 3], [2, 3, 1], [3, 1, 2], [3, 2, 1]
	//static void combSum(int[] num, int start, int target, ArrayList<Integer> list, ArrayList<ArrayList<Integer>> result){
	static void combNum(int[] num, ArrayList<Integer> list, ArrayList<ArrayList<Integer>> result){	
	    //if(target == 0){//sum=target [&& list.size() == m to find m items sum up to the target
	    if (list.size() == num.length){ 
	        ArrayList<Integer> temp = new ArrayList<Integer>(list);//snapshot current items in list, which is constantly repopulated
	        result.add(temp);
	        return;
	    }
	 
//	    for(int i=start; i<num.length; i++){//go thru all possible values from rest of array for current level
//	        if(target < num[i])//skip current num if sum pass the target 
//	            continue;
	    for(int i=0; i<num.length; i++){
	    	//if (list.isEmpty() || !list.contains(num[i]))
	    		if (num[i]==-1)//skip the num already visited
	    			continue;
	    		list.add(num[i]);
	    		int preval=num[i];
	    		num[i]=-1;//mark as visited
	        //combSum(num, i+1, target-num[i], list, result);//each recursion to next num out because each num only need to be counted once  
	        combNum(num, list, result);
	        num[i]=preval;//restore
	        list.remove(list.size()-1);//remove the last/current num to loop in the next num at current level
	    }
	}
	//http://www.programcreek.com/2013/02/leetcode-permutations-java/
	//[1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1]
	static void permute(int[] num, int start) {
		if (start >= num.length) {
			//output num
		}
		for (int j = start; j < num.length; j++) {
			com.bbg.test.Sort.swap(num, start, j);//Swap cur 1(0) with each element 2(1),3(2) after it A[1,2,3] B[2,1,3] C[3,1,2]
			//j = start no change to keep the originals from previous, fxp  A[1,2,3] A'[1,2,3]
			permute(num, start + 1);//move cur to next (1) and further swap from above result to A'[1,2,3][1,3,2] B'[2,1,3][2,3,1] C'[3,1,2][3,2,1]
			com.bbg.test.Sort.swap(num, start, j);//restore before next loop
		}
	}
	//S[1,2,3] returns [3],[1],[2],[1,2,3],[1,3], [2,3], [1,2],[]
	static ArrayList<ArrayList<Integer>> subsets(int[] S) {
		if (S == null)
			return null;
	 
		Arrays.sort(S);
	 
		ArrayList<ArrayList<Integer>> result = new ArrayList<ArrayList<Integer>>();
	 
		for (int i = 0; i < S.length; i++) {
			ArrayList<ArrayList<Integer>> temp = new ArrayList<ArrayList<Integer>>();
			//deep copy from result to separate change to temp 
			for (ArrayList<Integer> a : result) {
				temp.add(new ArrayList<Integer>(a));
			}
			//add S[i] 3[2] to existing sets from previous 2[1]: [[1], [1, 2], [2]]
			//[[1, 3], [1, 2, 3], [2, 3]]
			for (ArrayList<Integer> a : temp) {
				a.add(S[i]);
			}
			//add a set [3] for S[i] only [[1, 3], [1, 2, 3], [2, 3], [3]]
			ArrayList<Integer> single = new ArrayList<Integer>();
			single.add(S[i]);
			temp.add(single);
			//add i temp [[1, 3], [1, 2, 3], [2, 3], [3]] to previous i-1 result [[1], [1, 2], [2]]
			result.addAll(temp);
		}
		//add empty set[]
		result.add(new ArrayList<Integer>());
	 
		return result;
	}
	//Count the number of expressions containing n pairs of parentheses which are correctly matched. For n = 3, ((())), ()(()), ()()(), (())(), (()()).
	//Count the number of possible Binary Search Trees with n keys 
	static int catalan(int n) {//n = 0, 1, 2, 3 are 1, 1, 2, 5 
        int res = 0;
        // Base case
        if (n <= 1) {
            return 1;
        }
        for (int i = 0; i < n; i++) {
            res += catalan(i) * catalan(n - i - 1);//C(n+1) = SUM(C(i)*C(n-i)) OR C(n) = SUM(C(i)*C(n-i-1))
        }
        return res;
    }
	//Given n pieces of wood with length L[i], cut into >= k pieces with the same length. What is the longest length 
    public int woodCut(int[] L, int k) {
        if (L == null || L.length == 0 || k < 0) {
            return 0;
        } 
        if (L.length == 1) {
            return L[0] / k;
        }

        int start = 0,mid = 0,max = 0;
        for (int i = 0; i < L.length; i++) 
        	max = Math.max(max, L[i]);//using length of longest existing wood because k pcs may be cut from only it self  
        int end=max-1;
        while (start + 1 < end) {
            mid = start + (end - start) / 2;//binary search
            
            int count = 0;
            for (int i : L) 
                count += i / mid;//how many pcs can be cut out by length mid from all given wood
            
            if (count >= k) {//meet the req, increase length by half mid above mid to see if still can get k pcs 
                start = mid;
                max = mid;//reuse max to keep prev value after mid recalc                   
            } else {
                end = mid;//less than the req, decrease length by half mid to see if still can get k pcs
            }
        }
        return max;
    }
}
