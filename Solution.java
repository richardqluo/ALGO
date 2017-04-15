package com.hack.test;

import java.io.IOException;
import java.util.*;

public class Solution {

	public static void main(String[] args) throws IOException {
		// BufferedReader in = new BufferedReader(new
		// InputStreamReader(System.in));
		// String s;
		// String[] sa = new String[20];
		// while ((s = in.readLine()) != null) {
		// sa = s.split(" ");
		// int[] a = new int[sa.length];
		// for (int i = 0; i < sa.length; i++) {
		// a[i] = Integer.parseInt(sa[i]);
		// }
		// // int[] a = {10, 7, -3, -10, 4, 2, 8, -2, 4, -5, -6}
		// System.out.println(sum(a));
		// }
		// String[] friends = {"YYNNN","YYNNN","NNYNN","NNNYN","NNNNY"};
		// String[] friends = {"YYNN","YYYN","NYYN","NNNY"};//2
		String[] friends = { "YNYY", "NYYN", "YYYN", "YNNY" };// 1
		// String[] friends = {"YNNN","NYNN","NNYN","NNNY"};//4

		System.out.println(friendCircles(friends));
		return;

	}

	// http://yuancrackcode.com/tag/two-sigma-oa/
	// disjoint set: If M[i][j]=Y, i and j are friends, find how many groups, one without any friend also consider as a group 
	public static int friendCircles(String[] friends) {
		if (friends == null || friends.length == 0 || friends.length > 300)
			return 0;

		int circles = 0, n = friends.length;
		boolean[] visited = new boolean[n];
		Queue<Integer> q = new LinkedList<Integer>();
		q.offer(0);// 0 is the index of first person in friends
		visited[0] = true;
		while (!q.isEmpty()) {
			int curr = q.poll();
			char[] currFriends = friends[curr].toCharArray();
			for (int i = 0; i < currFriends.length; i++) {
				if (currFriends[i] == 'Y' && i != curr && !visited[i]) {
					q.offer(i);// i belongs the the circle with curr
					visited[i] = true;
				}
			}
			if (q.isEmpty()) {// find another component/circle
				circles++;
				for (int i = 0; i < n; i++) {// offer next unvisited friend to the queue
					if (!visited[i]) {
						q.offer(i);// create a new component
						visited[i] = true;
						break;// continue as q has 1
					}
				}
			}
		}
		return circles;
	}

	public static int longestchain(String[] words) {
		HashSet<String> dict = new HashSet<String>();
		HashMap<String, Integer> map = new HashMap<String, Integer>();
		// use map to store the longest len for string, save time
		for (String s : words) {
			dict.add(s);
		}
		int longest = 0;
		for (String s : words) {
			if (s.length() <= longest) {
				continue;// no need to process s, since its longest length can
							// be at most longest
			}
			int len = helper(s, dict, map) + 1;// s itself is already in the
												// dict, so + 1
			map.put(s, len);
			longest = Math.max(len, longest);
		}
		return longest;
	}

	public static int helper(String s, HashSet<String> dict, HashMap<String, Integer> map) {
		int result = 0;
		for (int i = 0; i < s.length(); i++) {
			String newStr = s.substring(0, i) + s.substring(i + 1);
			// create a new string by removing a char
			if (dict.contains(newStr)) {
				if (map.containsKey(newStr)) {
					result = Math.max(result, map.get(newStr));
				} else {
					result = Math.max(result, helper(newStr, dict, map) + 1);
				}

			}
		}
		return result;
	}

	public static boolean isRectOverLap(rect a, rect b) {
		if (a.l.x > b.r.x || b.l.x > a.r.x)// horizontal aside  l:lowerleft corner r:upper right corner
			return false;
		if (a.l.y > b.r.y || b.l.y > a.r.y)// vertical aside
			return false;
		return true;
	}	

    public static String addBinary(String a, String b) {
        int la = a.length();
        int lb = b.length();
        
        int max = Math.max(la, lb);
        
        StringBuilder sum = new StringBuilder("");
        int carry = 0;
        
        for(int i = 0; i < max; i++){
            int m = getBit(a, la - i - 1);
            int n = getBit(b, lb - i - 1);
            int add = m + n + carry;
            sum.append(add % 2);
            carry = add / 2;
        }
        
        if(carry == 1)
            sum.append("1");
        
        return sum.reverse().toString().replaceAll("^0*", "");
        
    }
    
    public static int getBit(String s, int index){
        if(index < 0 || index >= s.length())
            return 0;
        
        if(s.charAt(index) == '0')
            return 0;
        else
            return 1;
        
    }
    //0 is room, B->obstacle, G-> Guard
    //0 0 0 
    //B G G 
    //B 0 0
    //calculate the steps from a room to nearest Guard 
    //2 1 1 
    //B G G 
    //B 1 1
    //BFS O(n^2) from each guard and update least distance to each room
    public static void  stepToGuard(int[][] matrix) {
    	int rows = (int)matrix.length;
    	int cols = (int)matrix[0].length;
        int i, j;
        for (i = 0; i < rows; ++i) {
            for (j = 0; j < cols; ++j) { 
                if (matrix[i][j] == -1) { // Guard: -1  Block:-2                    
                    doBFS(matrix, i, j); 
                } 
            } 
        }
    }
    static int[][] dd = new int[][]{{-1, 0}, {+1, 0}, {0, -1}, {0, +1}};
	private static void doBFS(int[][] matrix, int r, int c) {
		Queue<Integer> q = new LinkedList<Integer>();
		int tmp;
		int rr, cc;
		int i;
		int dist;
		int rows = (int) matrix.length;
		int cols = (int) matrix[0].length;
		q.offer(r * cols + c);// serialize coordinates, only init push is guard, subseq are all rooms
		while (q.size() > 0) {
			tmp = (int) q.poll();
			r = tmp / cols;// deserialize coordinates
			c = tmp % cols;
			//dist of room init to 0 or assign to existing value, dist init guard(matrix[r][c]<0) to itself =0
			dist = matrix[r][c] > 0 ? matrix[r][c] : 0;
			for (i = 0; i < 4; ++i) {// go thru all 4 neighbors
				rr = r + dd[i][0];
				cc = c + dd[i][1];
				if (!(rr >= 0 && rr <= rows - 1 && cc >= 0 && cc <= cols - 1) || // out of boundary
						matrix[rr][cc] < 0 || //is the neighbor a guard or a blockade
						(matrix[rr][cc] > 0 && matrix[rr][cc] <= dist + 1)) { // the distance is no shorter
					continue;
				} 
				matrix[rr][cc] = dist + 1;//one more hop above distance of room node off the queue
				q.offer(rr * cols + cc);//enqueue only the neighbor that is room and has > dist+1 
			}
		}
	}
	//common item from n sorted arrays O(mn) ref  BETTER  C++ intersectKsortedArray
	public static void printCommonElements(int[][] arrays) {
		if (arrays.length < 2) {
			System.out.println("Too few arrays");
			return;
		}
		int baseIndex = 0;
		int[] indices = new int[arrays.length - 1];
		int totalMatchFound;
		boolean smallestArrayTraversalComplete = false;

		while ((baseIndex < arrays[0].length) && (!smallestArrayTraversalComplete)) {
			totalMatchFound = 0;
			for (int i = 1; i < arrays.length; i++) {
				int currIndex = indices[i - 1];//arrays[i] resume from index left from previous compare with arrays[0] at baseIndex
				while ((currIndex < arrays[i].length) && (arrays[i][currIndex] < arrays[0][baseIndex])) {
					currIndex += 1;//increment till > bench mark array item
				}
				if (currIndex < arrays[i].length) {
					if ((arrays[i][currIndex] == arrays[0][baseIndex])) 
						totalMatchFound += 1;
				}
				else {
					smallestArrayTraversalComplete = true;//stop if any array (shortest) complete
				}
				indices[i - 1] = currIndex;//indices[0] store prev index of array[1]
			}
			if (totalMatchFound == arrays.length - 1) {//find the item in all arrays
				System.out.println(arrays[0][baseIndex]);
			}
			baseIndex += 1;//index of bench mark arrays[0] item to be compared 
		}
	}
	//sorted array is rotated at some pivot (0 1 2 4 5 6 7 > 4 5 6 7 0 1 2)
	public int findRotated(int[] nums, int target) {
	    int left = 0;
	    int right= nums.length-1;
	 
	    while(left<=right){
	        int mid = left + (right-left)/2;
	        if(target==nums[mid])
	            return mid;	 
	        if(nums[left]<=nums[mid]){//rotate pos is after mid, nums[left] > nums[pos~right]
	            if(nums[left]<=target && target<nums[mid]){//target in [left,mid], ordered section
	                right=mid-1;
	            }else{
	                left=mid+1;//start of unordered section where continue to split to order n unorder
	            }
	        }else{
	            if(nums[mid]<target && target<=nums[right]){//target in [mid,right], ordered section
	                left=mid+1;
	            }else{
	                right=mid-1;//end of unordered section
	            }
	        }    
	    }	 
	    return -1;
	}
	//dp[i][j] denote whether a substring from i to j is palindrome, Time O(n^2) Space O(n^2)
	//dp[i+1][j-1] == true && s.charAt(i) == s.charAt(j)=>dp[i][j] == true 
	//nondp Time O(n^2) Space O(1)
	public String longestPalindrome(String s) {
		if (s.isEmpty()) 
			return null;	 
		if (s.length() == 1) 
			return s;
	 
		String longest = s.substring(0, 1);
		for (int i = 0; i < s.length(); i++) {			
			String tmp = helper(s, i, i);// get longest palindrome with center of i
			if (tmp.length() > longest.length()) {
				longest = tmp;
			}
			
			tmp = helper(s, i, i + 1);
			if (tmp.length() > longest.length()) {// get longest palindrome with center of i, i+1
				longest = tmp;
			}
		}	 
		return longest;
	}	 
	// Find longest palindrome from a center, either one letter or two letter, 
	public String helper(String s, int begin, int end) {
		while (begin >= 0 && end <= s.length() - 1 && s.charAt(begin) == s.charAt(end)) {
			//store s.substring(begin, end)
			begin--;
			end++;
		}
		return s.substring(begin + 1, end);
	}
	//given array of string, group anagrams together
    public List<List<String>> groupAnagrams(String[] strs) {
        List<List<String>> rst = new ArrayList<List<String>>();
        HashMap<String, List<String>> map = new HashMap<String, List<String>>();
        Arrays.sort(strs);//return list alphbat order, //C++ std::sort(strs, strs+size);
        for(int i = 0; i < strs.length; i++) {
            char[] strChar = strs[i].toCharArray();
            Arrays.sort(strChar);//C++ std::sort(str.begin(), str.end());
            String str = new String(strChar);//anagram if equal after alphabatic sort 
            if(map.containsKey(str)) {//key is sorted string, value is list of its anagrams found from array 
                map.get(str).add(strs[i]);
            }
            else {
                List<String> list = new ArrayList<String>();
                list.add(strs[i]);
                map.put(str, list);
            }
        }
        for(List<String> val : map.values()) 
            rst.add(val);

        return rst;
    }
}

class point {
	public int x, y;

	public point(int x, int y) {
		this.x = x;
		this.y = y;
	}
}

class rect {
	public point l, r;

	public rect(int x, int y, int lx, int ly) {
		l = new point(x, y);
		r = new point(x + lx, y + ly);
	}
}
