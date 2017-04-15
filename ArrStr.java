package com.bbg.test;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.*;


public class ArrStr {
		
	public static void main(String[] args) throws IOException {
//		BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
//	    String s;
//	    String[] sa = new String[20];
//	    while ((s = in.readLine()) != null) {
//	    	sa=s.split(" ");  
//		    int[] a=new int[sa.length];
//		    for(int i = 0; i < sa.length; i++){
//		    	a[i]=Integer.parseInt(sa[i]);
//		    }	
//		    System.out.println(maxSub(a));	    	 
//	    }
//	   	int[] a = {10, 7, -3, -10, 4, 2, 8, -2, 4, -5, -6};
//	   	System.out.println(maxSub(a));
		maxSum();
		return;
	}
	//max sum of any 2 elements out of array (find largest 2 numbers) 
	static void maxSum(){
		int[] a = {2,5,3,1,3,8,4,9};
		int max1 = Integer.MIN_VALUE;
		int max2 = Integer.MIN_VALUE;
		for (int i = 0; i < a.length; i++) {
			if (a[i]>max1){
				int tempMax1=max1;
				max1=a[i];
				if(tempMax1>max2){
					max2=tempMax1;
				}
			}else if(a[i]>max2)
				max2=a[i];
		}
		System.out.println(max1+max2);
	}
	//find 3 num produce max from array (+-)
	static int maxProd(int[] a) {
		int min1 = Integer.MAX_VALUE;
		int min2 = Integer.MAX_VALUE;
		int max1 = Integer.MIN_VALUE;
		int max2 = Integer.MIN_VALUE;
		int max3 = Integer.MIN_VALUE;

		for (int i = 0; i < a.length; i++) {
			//find 3 max num 1>2>3, or use max heap
			if (a[i] > max1) {//a[i] > max1 > max2 > max3
				int tempMax1 = max1;
				max1 = a[i];
				if (tempMax1 > max2) {
					int tempMax2 = max2;
					max2 = tempMax1;
					if (tempMax2 > max3) 
						max3 = tempMax2;//max1(a[i]) > max2(former max1) > max3(former max2)
				} else {//former max1 == max2
					if (tempMax1 > max3) 
						max3 = tempMax1;
				}
			} else if (a[i] > max2) {//max1 > a[i] > max2 > max3
				int tempMax2 = max2;
				max2 = a[i];
				if (tempMax2 > max3) //if former max2==max3 do nothing
					max3 = tempMax2;//max1 > max2(a[i]) > max3(former max2)
			} else if (a[i] > max3) {
				max3 = a[i];
			}
			/*simple
			if (a[i] > max1) {
				max3=max2;
				max2=max1;
				max1=a[i];
			}
			else if(a[i] > max2){
				max3=max2;
				max2=a[i];
			}
			else if(a[i] > max3){
				max3=a[i];
			}
			*/
			//find 2 min num 1<2
			if (a[i] < min1) {
				int tempMin1 = min1;
				min1 = a[i];
				if (tempMin1 < min2) {
					min2 = tempMin1;
				}
			} else if (a[i] < min2) {
				min2 = a[i];
			}
		}
		//if min1<min2<0, min1*min2 may >max1*max2
	    int prod1 = min1 * min2 * max1;
	    int prod2 = max1 * max2 * max3;
	    return Math.max(prod1,prod2);
	}
	//find a subarray where the sum of numbers is zero | n
    public ArrayList<Integer> subSumZero(int[] nums) {//REF C++ maxSubArray
        ArrayList<Integer> rst = new ArrayList<Integer>();
        if (nums == null || nums.length == 0) {
            return rst;
        }
        int sum = 0;
        HashMap<Integer, Integer> map = new HashMap<Integer, Integer>();
        map.put(0, -1);//[sum,index], if nums[0]=0 then rst{0,0}
        //if sum[0 ~ a] = sum[0 ~ b] then  sum[a+1 ~ b] = 0 
        for (int i = 0; i < nums.length; i++) {
            sum += nums[i];
            if (map.containsKey(sum)) {//map.containsKey(sum-n)
                rst.add(map.get(sum) + 1);//a
                rst.add(i);//b
                return rst;
            }
            map.put(sum, i);//save sum[0 ~ i] as key i as value
        }
        return rst;
    }
	//max contiguous subarray sum: Dynamic Programming 
	//ignore the sum of the previous n-1 elements if nth element is greater than the sum
	//{10 7 -3 -10 4 2 8 -2 4 -5 -6} 20 [0,8]
	static int maxSub(int[] A) {
		int newsum = A[0];
		int max = A[0];
		int start=0,end=0;
		for (int i = 1; i < A.length; i++) {
			//case if prev newsum<0 and A[i]>0 then newsum restart from A[i] 
			//newsum = Math.max(newsum + A[i], A[i]);//if not to get index of subarray
			if(newsum + A[i]>A[i]){
				newsum += A[i];
			}
			else{
				newsum = A[i];
				start=i;
			}
			//compare to previous max newsum
			//max = Math.max(max, newsum);	
			if(newsum>max){
				max=newsum;				
				end=i;
			}		
		}
		return max;
	}
	//if robbing 2 adjacent houses will alert police, find max amount given a list of house with money value
	static int robMax(int[] num) {
	    if(num==null || num.length==0)
	        return 0;
	 
	    int n = num.length;
	 
	    int[] dp = new int[n+1];
	    dp[0]=0;
	    dp[1]=num[0];//dp[i] : num[i-1], or add dummy num[0]=0 to map dp[i] to num[i], dp[0|1]=num[0|1];
	    //case 1: rob num[i-1] plus max amount from dp[i-2]:num[i-3], skip dp[i-1]:num[i-2] 
	    //case 2: not rob num[i-1] then inherit max amount dp[i-1]:num[i-2]
	    for (int i=2; i<n+1; i++){
	       dp[i] = Math.max(dp[i-1], dp[i-2]+num[i-1]); //dp[i]=Math.max(dp[i-1], dp{i-2]+num[i]);
	    }
	 
	    return dp[n];
	}
	//buy and sell 1 transaction to gain max profit on an array of stock prices
	int maxProfit(int[] p)
	{
		int min = p[0];
		int max = 0;
		for (int i = 1; i < p.length; i++)
		{
			if (min > p[i])
				min = p[i];
			else
			{//no need to recalc when min = p[i]
				if ((p[i] - min) > max)
					max = p[i] - min;
			}
		}
		return max;
	}
	//unlimited transactions but have to sell out before next buy
    int maxProfits(int[] prices) {
        if (prices.length < 2) return 0;
        
        int maxProfit = 0;
        for (int i = 1; i < prices.length; i++) {
            int diff = prices[i] - prices[i - 1]; //prices[i] - prices[i - 1] - fee if applicable
            if (diff > 0) //consecutive buy sell as long as price rise, same effect as buy|sell at begin|end of rise
                maxProfit += diff;
        }
        
        return maxProfit;
    }
	//with at most k transactions but have to sell out before next buy O(nk)
	int maxProfitsDP(int k, int[] prices) {
		int len = prices.length;
	 
		if (len < 2 || k <= 0)
			return 0;
		//The local array tracks maximum profit of j tran & the last tran sell[i] is ON i day. 
		int[][] local = new int[len][k + 1];
		//int[] local = new int[k + 1]; //1D version
		//The global array tracks the maximum profit of j tran TILL i day.
		int[][] global = new int[len][k + 1];
		//int[] global = new int[k + 1]; //1D version
	 
		for (int i = 1; i < len; i++) {
			int diff = prices[i] - prices[i - 1];//+/-
			for (int j = 1; j <= k; j++) {//keep buy sell per day as long as price rise, merge into single trans same effect as buy|sell at begin|end of rise
				//if prices[i-1]<[i], buy[i-1] and sell[i] can be merge with previous sell[i-1] as same tran j to save 1 tran: local[i][j]=local[i-1][j]+diff (>0)
				//if prices[i-1]>[i], local[i][j]=global[i-1][j-1]+diff (<=0)
				local[i][j] = Math.max(global[i - 1][j - 1], local[i - 1][j] + diff);
				//if prices[i-1]<[i], sell[i] and sync global[i][j]=local[i][j]
				//if prices[i-1]>[i], no sell[i] trans remain same j global[i][j]= global[i - 1][j]+diff (<=0)
				global[i][j] = Math.max(global[i - 1][j], local[i][j]);
			}
//			for (int j = k; j >= 1; j--) {//1D version : reverse order to use j-1 value from previous i
//				local[j] = Math.max(global[j - 1], local[j] + diff);
//				global[j] = Math.max(local[j], global[j]);
//			}
		}
	 
		return global[prices.length - 1][k];
		//return global[k]; //1D version
	}
	//return any peak position A[P] > A[P-1] && A[P] > A[P+1] in given array: A[i]!=A[i-1], A[0] < A[1] and A[n - 2] > A[n - 1]
	//Searching num in an array that is first decreasing and then increasing: O(logn) find peak then binary search on both side
    static int findPeak(int[] A) {
        int start = 1, end = A.length-2; 
        while(start + 1 <  end) {
            int mid = (start + end) / 2;
            if(A[mid] < A[mid - 1]) {
                end = mid-1;//search on left half, A[0]<A[1] A[mid] < A[mid - 1] so [1, mid - 1] must contain peak 
            } else if(A[mid] < A[mid + 1]) {
                start = mid+1;//search on right half, A[n - 2] > A[n - 1] A[mid] < A[mid + 1] so [mid + 1,n-2] must contain peak
            } else {
                return mid;//A[mid - 1] < A[mid] > A[mid + 1] 
            }
        }
        if(A[start] < A[end]) {
            return end;
        } else { 
            return start;
        }
    }
    //every element appears twice except for one
    static int singleNumberXOR(int[] A) {
    	int x = 0;
    	for (int a : A) {
    		x = x ^ a;
    	}
    	return x;
    }
    static int singleNumberHashset(int[] A) {
    	HashSet<Integer> set = new HashSet<Integer>();
    	for (int n : A) {
    		if (!set.add(n))//true if not exist 
    			set.remove(n);//or output duplicate
    	}
    	Iterator<Integer> it = set.iterator();
    	return it.next();//also as firstUniqueChar
    	//for (Integer i : set) {System.out.println(i);}
    }
	//same|diff XOR =0|1, Items and index are same 0~N except a duplicate or missing num
	//dup0^1^2^3^2(r) std^0^1^2^3=2
	static int findDuplicateNumber(int[] arr) {
	    int i = arr.length-2;  
	    int r = arr[arr.length-1]; 
	    while (i>=0) {
	        r ^= i^ arr[i];
	        i--;
	    }

	    return r;
	}
	//missing0^1^3 std^0^1^2^3(r) =2
	static int findMissingNumber(int[] arr) {
	    int r = arr.length;
	    int i = arr.length-1; 
	    while (i>=0) {
	        r ^= i^ arr[i];
	        i--;
	    }

	    return r;
	}
	//1002 numbers in the range of 1-1000 with two numbers duplicated
	//Sum(1-n) = n(n+1)/2 X + Y = S – n(n+1)/2 XY = P/n!, X – Y = sqrt((X+Y)^2 – 4*XY), 
	void printRepeating(int arr[])
	{ int[] count=new int[arr.length-2];
	  for(int i = 0; i < arr.length; i++)
	  {  
	    if(count[arr[i]] == 1)
	    	System.out.print(arr[i]);
	    else
	     count[arr[i]]++;
	  } 
	}  
	//given num a to find pair of elements(b,c) from the unsort array n=b+c.
	//sorted array:  i|j = 0|n  when a[i] + a[j] = k else if a[i] + a[j]>|< k j-- | i++ till i =j
	static void findPair(int[] a, int n) {
		Set<Integer> ht = new HashSet<Integer>();//HashMap<a[i],List<index>> to return index of multiple pair elements
		//given all a[i]>0, bool|int[0,n] need store mapping because 0< the pair <n
		//if a[i]<n {if (need[n-a[i]]!=0) {return a[i],n-a[i]|i,need[n-a[i]} else{need[a[i]]=true|i;} }
		for (int i = 0; i < n; ++i) {
			int need = n - a[i];
			if (ht.contains(need)) {//&& !ht.contains(a[i]) eliminate duplicate 
				System.out.format("%d:%d%n", a[i], need);
				//ht.add(a[i]); //if duplicate otherwise no need because no another item=need
			} else {
				ht.add(a[i]);
			}
		}
	}
	//numbers is sorted
	public int[] twoSumSortArr(int[] numbers, int target) {
		int i = 0;
		int j = numbers.length - 1;	 
		while (i < j) {
			int x = numbers[i] + numbers[j];
			if (x < target) {
				++i;
			} else if (x > target) {
				j--;
			} else {
				return new int[] { numbers[i], numbers[j] };//if duplicate add (i,j) to result
			}
		}	 
		return null;
	}
	//sorted array A has m elements but size is m+n
	//similar find same diff items: while (i<m && j<n){if (A[i] <|> B[j]){i|j++;diff.add(A[i]|B[j])} else{same.add(A[i])} } if(i<m|j<n){diff.add(rest A|B)} 
    public void mergeSortedArray(int[] A, int m, int[] B, int n) {
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
	//given [100, 4, 200, 1, 3, 2], the longest consecutive elements sequence [1, 2, 3, 4] length is 4.
	public static int longestConsecutive(int[] num) {
		// if array is empty, return 0
		if (num.length == 0) {
			return 0;
		}
	 
		Set<Integer> set = new HashSet<Integer>();
		int max = 1;
		//ArrayList<Integer> rec= new ArrayList<Integer>(); //return elements
	 
		for (int e : num)
			set.add(e);
	 
		for (int e : num) {
			int left = e - 1;
			int right = e + 1;
			int count = 1;
			//rec.add(e); 
	 
			while (set.contains(left)) {
				count++;
				//rec.add(left);
				set.remove(left);//1 num can be counted in only 1 consecutive range
				left--;
			}
	 
			while (set.contains(right)) {
				count++;
				//rec.add(right);
				set.remove(right);
				right++;
			}
	 
			max = Math.max(count, max);
			//if(count>max) update rec
				
		}
	 
		return max;
	}
	//Find intersection of a.size < b.size a[9, 1, 4, 2, 5] b[3, 1, 8, 7, 6, 5, 5] result 1 5 not 1 5 5
	public static List<Integer> getIntersectionO1Space(List<Integer> a, List<Integer> b)
	{
		Collections.sort(a); //mergeSort Time O(k log k), space O(1)
	    for ( Iterator<Integer> iterator = b.iterator(); iterator.hasNext(); ) //O(n log k)
	    {
	    	//vs hashset to store smaller array item comparing to bigger array time O(1) space O(n)
	        int r = Collections.binarySearch(a, iterator.next()); //O(log k)
	        if (r < 0) {
	            iterator.remove();
	        } else {
	            a.remove(r);//handle duplicates
	        }
	    }
	    return b;
	}
	//given [5, 7, 7, 8, 8, 10] and target value 8, return [3, 4] with O(log n)
	public static int[] findRange(int[] arr, int target, int left, int right) {
	    //parameter check
	    if(arr == null || arr.length == 0) return new int[]{-1, -1};

	    //boundary check
	    if(left > right) return new int[]{-1, -1};

	    int mid = (left+right)/2;
	    if(arr[mid] < target) return findRange(arr, target, mid+1, right);
	    else if(arr[mid] > target) return findRange(arr, target, left, mid-1);
	    else    {
	        int[] leftRange = findRange(arr, target, left, mid-1);
	        int[] rightRange = findRange(arr, target, mid+1, right);
	        int leftEnd = (leftRange[0] == -1 ? mid : leftRange[0]);
	        int rightEnd = (rightRange[1] == -1 ? mid : rightRange[1]);
	        return new int[]{leftEnd, rightEnd};
	    }
	}
	//histogram (bar chart) [2,1,4,6,5,3] 4X3=12
	public static int largestRectangle(int[] height) {
		if (height == null || height.length == 0) {
			return 0;
		}
	 
		Stack<Integer> stack = new Stack<Integer>();
		int max = 0;

        for (int i = 0; i <= height.length; i++) {
            int current = (i == height.length) ? -1 : height[i];//always calc the last bar -1 < height[length-1]
            //when the current height is less than the previous one, the previous height cant spread over current one
            //max area based on previous height is reached and can be calculated 
            while (!stack.empty() && current <= height[stack.peek()]) {
            	int p = stack.pop();
                int h = height[p];//while keep poping previous heights, i stay on current   
                //from previous height just being poped to the one right before current
                int w = i-p;//stack.empty() ? i : i - stack.peek() - 1;
                max = Math.max(max, w * h);
            }
            stack.push(i);//push index to stack when the current height is larger than the previous one
        }
        return max;
	}
	//max area|water between bars|container can hold
	static int maxArea(int[] height) {
		if (height == null || height.length < 2) {
			return 0;
		}
	 
		int max = 0;
		int left = 0;
		int right = height.length - 1;
		//scan from both sides. If leftHeight <|> rightHeight, move left|right and find a greater value 
		while (left < right) {
			max = Math.max(max, (right - left) * Math.min(height[left], height[right]));
			if (height[left] < height[right])
				left++;
			else
				right--;
		}
	 
		return max;
	}
	//rain can be trapped by bars, water on height[i] is decided by min(highest left,right height)
	//if min<|>height[i] water[i]=0|min-height[i]
    static int trapWater(int[] height) {
        if(height.length<3) return 0;
        int[] leftHeight= new int[height.length];
        int[] rightHeight= new int[height.length];
        int water=0;
        for (int i = 1; i < height.length; i++) 
        	leftHeight[i]=Math.max(leftHeight[i-1],height[i-1]);//highest left
        for (int i = height.length-2; i >=0; i--){ 
        	rightHeight[i]=Math.max(rightHeight[i+1],height[i+1]);
        	int minHeight=Math.min(leftHeight[i], rightHeight[i]);
        	if(minHeight>height[i])
        		water+=minHeight-height[i]; 
        }
        return water;
    }
    //how many three num can form triangle: sum(any of two)>the other 
    //if x< y < z then x+z>y and y+z>x by sort, then find x+y>z for each z O(n^2)
    public int triangleCount(int S[]) {
        int ans = 0;
        Arrays.sort(S);
        for(int i = S.length-1; i >2 ; i--) {
            int left = 0;
            int right = i - 1;//S[right] < S[i]
            while(left < right) {
                if(S[left] + S[right] > S[i]) {
                    ans = ans + (right - left);//all comb : current right with j (S[left]<S[j]<S[right])
                    right --;
                } else {
                    left ++;//bigger S[left] so S[left] + S[right] > S[i]
                }
            }
        }
        return ans;
    }
	//circular station i has gas[i], car costs cost[i] of gas to travel from station i to its next station (i+1)
	public int canCompleteCircuit(int[] gas, int[] cost) {
		int remaining = 0; //remaining left from previous
		int total = 0; // track total remaining
		int start = 0; 
	 
		for (int i = 0; i < gas.length; i++) {
			int balance = gas[i] - cost[i];//current 
	 
			//if sum remaining of (i-1) >= 0, continue 
			if (remaining >= 0) { 
				remaining += balance;
			//otherwise, reset start index to be current
			} else {
				remaining = balance;
				start = i;
			}
			total += balance;//balance of whole cycle
		}
		//as long as total >= 0, it can go thru whole circle, gas|cost[0] already included, same effect as circle from last to first 
		if (total >= 0){
			return start;
		}else{
			return -1;
		}
	}
	//how many jumps (positive right negative left) out if array a[0]=2->a[2]=3->a[5]... 
	public static int jumps(int[] a) {
		int i = 0;int c = 0;
		int n = a.length;
		while (i >= 0 && i < n) {
			if (a[i] == 1000001)
				return -1;//loop
			int j = a[i] + i;
			a[j] = 1000001;//mark as visited 
			i = j;
			c++;
		}
		return c;
	}
	//Each element array represents maximum jump from that position, if can reach the end [3,2,1,0,4] false
	static boolean canJump(int[] A) {
	    if(A.length <= 1)
	        return true;
	 
	    int reach = 0; //reach stands for the largest index that can be reached.
	 
	    for(int i=0; i<A.length; i++){
	    	reach=Math.max(reach, i + A[i]);//greedy
	        if(reach >= A.length-1) 
	            return true;	    	
	    	if(reach==i)	//all previous max reach can not pass current i and A[i] == 0 
	            return false;
	    }
	 
	    return false;    
	}
	//Each element array represents maximum jump from that position, minimum jumps to reach the last index
	// [2,3,1,1,4] 2 jumps:0-1-4  
	static int minJump(int[] nums) {
		if (nums == null || nums.length == 0)
			return 0;
	 
		int lastReach = 0;
		int reach = 0;
		int step = 0;
	 
		for (int i = 0; i <= reach && i < nums.length; i++) {
			//when last jump can not read current i, increase the step by 1
			if (i > lastReach) { 
				step++;
				lastReach = reach;
			}
			reach = Math.max(reach, nums[i] + i);
		}
	 
		if (reach < nums.length - 1)
			return 0; //can not jump out
	 
		return step;
	}
	//given [3, 30, 34, 5, 9], the largest formed number is 9534330
	public String largestNumber(int[] nums) {
	    String[] strs = new String[nums.length];
	    for(int i=0; i<nums.length; i++)
	        strs[i] = String.valueOf(nums[i]);
	    //sorting strings instead of sorting integer.
	    Arrays.sort(strs, new Comparator<String>(){//class CustomComparator implements Comparator<String> {
	        public int compare(String s1, String s2){
	        	//=0|>0|<0 if this string (s2 + s1) is lexicographically equal|greater|less than the argument string (s1 + s2)
	        	return (s2 + s1).compareTo(s1 + s2);	 
	        }
	    });
	 
	    StringBuilder sb = new StringBuilder();
	    for(String s: strs)
	        sb.append(s);
	 
	    while(sb.charAt(0)=='0' && sb.length()>1)
	        sb.deleteCharAt(0);
	 
	    return sb.toString();
	}
	//longest substring without repeating letters for "abcabcbb" is "abc",  the length is 3
	static int longestUniqueSubstring(String s) {
		HashMap<Character, Integer> map = new HashMap<Character, Integer>();
	    if (s == null || s.length() == 0) return 0;
	    if (s.length() == 1) return 1;
	    
	    int right = 0, left = right - 1, max = 0;
	    //String result;//store the longest substring 
	    while (right != s.length()) {
	         Integer prevPos = map.put(s.charAt(right), right);
	         //put returns the previous value associated with key, or null 
	         if (prevPos != null) {
	             left = Math.max(left, prevPos);//restart from the prev dup, abcdcbe ignore dup (2nd b) if less than current left (2nd c) 
	         }
	         if (max < right - left){//max init = 1
	        	 //result = s.substring(left, right + 1);
	        	 max=right - left;	        	 
	         }
	         right++;
	    }
	    return max;//return result; 
	}
	//reverse is the same, ref C++linkedlist
	public boolean isPalindrome(String s) {
		s = s.replaceAll("[^a-zA-Z0-9]", "").toLowerCase();
	 
		int len = s.length();
		if (len < 2)
			return true;
	 
		Stack<Character> stack = new Stack<Character>();
	 
		int index = 0;
		while (index < len / 2) {
			stack.push(s.charAt(index));
			index++;
		}
		//if len=5, index=2 at here
		if (len % 2 == 1)
			index++;
		//if len=5, index=3 at here
		while (index < len) {
			if (stack.empty())
				return false;
	 
			char temp = stack.pop();
			if (s.charAt(index) != temp)
				return false;
			else
				index++;
		}
	 
		return true;
	}
	//Parentheses match ref C++
	static boolean isValidBrackets(String s) {
		HashMap<Character, Character> map = new HashMap<Character, Character>();
		map.put('(', ')');
		map.put('[', ']');
		map.put('{', '}');
	 
		Stack<Character> stack = new Stack<Character>();
	 
		for (int i = 0; i < s.length(); i++) {
			char curr = s.charAt(i);
	 
			if (map.containsKey(curr)) {
				stack.push(curr);
			} else if (map.containsValue(curr)) {
				if (!stack.empty() && map.get(stack.peek()) == curr) {
					stack.pop();
				} else {
					return false;
				}
			}
		}
	 
		return stack.empty();
	}
	
	//number of occurance/duplication of a char in a given string
	//use node{char,next} to retain the insert order in linked list
	static void countChar(String s) {
		Map<Character, Integer> charMap = new HashMap<Character, Integer>();
		char[] arr = s.toCharArray();
		for (char value : arr) {
			if (charMap.containsKey(value)) {
				charMap.put(value, charMap.get(value) + 1);
			} else {
				charMap.put(value, 1);//charMap.put(node.char, 1);tail.next=node;tail=node;
			}
		}
	}

	static boolean isAnagram(String s1, String s2) {// MATTER, METATR, TERMAT
		int LETTERS_LEN = 256;// 26 if only letters
		if (s1.length() != s2.length())
			return false;
		int[] letters = new int[LETTERS_LEN];
		for (int i = 0; i < s1.length(); i++) {
			letters[s1.charAt(i)]++;// use char's int value as index
			letters[s2.charAt(i)]--;// count off
		}
		for (int i = 0; i < LETTERS_LEN; i++) {
			if (letters[i] != 0) {
				return false;
			}
		}
		return true;
	}
	static char firstUniqueChar(String s){
		int[] count=new int[26];
		int[] index=new int[26];
		Arrays.fill(index, -1);
		for (int i = 0; i < s.length(); i++) {
			count[s.charAt(i)-'A']++;
			if (index[s.charAt(i)-'A']==-1)
				index[s.charAt(i)-'A']=i;
		}
		int minIdx=Integer.MAX_VALUE;
		for(int i=0;i<26;i++){
			if (index[i]!=-1 && count[i]==1){
				//return i+'A';//need not index ref C++ findFirstUniqueNum
				minIdx=Math.min(minIdx, index[i]);
			}
		}
		return s.charAt(minIdx);
	}
	//each char of s1 first position in s2  
	static void firstPosChar(String s1, String s2){
		int[] index=new int[26];
		Arrays.fill(index, -1);
		for (int i = 0; i < s1.length(); i++) {
			int pos = s1.charAt(i)-'A';
			if (index[pos]==-1)
				index[pos]=s2.indexOf(s1.charAt(i));
		}
	}
	//aab a2b1, too long may overflow count
	public String compress(String str) {
	    char[] chars = str.toCharArray();
	    StringBuilder builder = new StringBuilder();
	    int count = 1;
	    char prev = chars[0];
	    for (int i = 1; i < chars.length; i++) {
	        char current = chars[i];
	        if (current == prev) {
	            count++;
	        } else {
	            builder.append(prev).append(count>2?count:"");//once or twice wont save the size
	            count = 1;
	            prev = current;
	        }	        
	    }
	    return builder.append(prev).append(count).toString();//last char
	}
	//if s can be segmented into one or more words in dictionary 
	public boolean wordBreak(String s, Set<String> dict) {
        if (s == null || dict.contains(s)) {
            return true;
        }

        String s2 = '#' + s;//dummy char treated as in dict so dp[0]=true as base for calc the next 
        int len = s2.length();
        boolean[] dp = new boolean[len];
        dp[0] = true;

        for(int i =1; i<len; i++) {//dp[i] if s[0,i] in dictionary 
        	for(int k=0; k<i; k++) {//i=3 k=0[# abc] k=1[#a bc] k=2[#ab c], no need for k=3[#abc ]
        		//if dp[k] == true and s[k+1,i] in dictionary 0<k<i
        		dp[i]=dp[k] && dict.contains(s2.substring(k+1, i));//length i-k
                if (dp[i])//as long as one segment is in dict
                    break;
            }
        }

        return dp[len-1];
    }
	//if not in place: split by ' ' and string builder append reversely
	public char[] ReverseWordsInPlace(char[] in_text){ 
	    int lindex = 0;
	    int rindex = in_text.length - 1;
	    if (rindex > 1)
	    {
	        in_text = ReverseString(in_text, 0, rindex);//reverse all phrases
	        for (rindex = 0; rindex <= in_text.length; rindex++)//reverse each word
	        {
	            if (rindex == in_text.length || in_text[rindex] == ' ')//aligned for rindex - 1
	            {
	                in_text = ReverseString(in_text, lindex, rindex - 1);
	                lindex = rindex + 1;
	                //handle multiple spaces
	                //while(rindex<in_text.length && in_text[rindex]==' ')
	                //	rindex++;
	                //lindex = rindex;

	            }
	        }
	    }
	    return in_text;
	}
	//char array pass by reference and have mutable elements
	public char[] ReverseString(char[] intext, int lindex, int rindex)
	{
	    char tempc;
	    while (lindex < rindex)
	    {
	        tempc = intext[lindex];
	        intext[lindex++] = intext[rindex];
	        intext[rindex--] = tempc;
	    }
	    return intext;
	}
    //set active = false for job of given id and any jobs that have parent_id = id or if their parent becomes inactive
    public void killJobs (List<job> jobs, int id){
        Map<Integer,List<job>> pc=new HashMap<Integer,List<job>>();//HashMap<id,list children jobs>
        for(job cur : jobs){
        	if (cur.id==id)
        		cur.active=false;//root set separately
            if(pc.containsKey(cur.parent_id))
            {
                List<job> cs= pc.get(cur.parent_id);
                cs.add(cur);
            }
            else
            {
                List<job> cs = new ArrayList<job>();
                cs.add(cur);
                pc.put(cur.parent_id, cs);
            }
        }
        killSubJobs(pc,id);//recursively kill all descendants
    }
    public void killSubJobs (Map<Integer,List<job>> pc, int id){
        List<job> cs=pc.get(id);
        //or BFS use queue 
        for(job cur:cs){
        	cur.active=false;
        	killSubJobs(pc, cur.id);
        }
    }
}

class job //similar to directory/file
{
    public int id;         // greater than 0
    public int parent_id;  // job either has a parent or parent_id == -1
    boolean active;
}
class FNode<T>{
    T data;//null if folder
    FNode<T> parent;
    List<FNode<T>> children;//null if file
    public FNode(T data) {
    	if (data!=null)
    		this.data = data;
    	else
    		this.children = new LinkedList<FNode<T>>();
    }
    public FNode<T> addChild(T child) {
    	FNode<T> childNode = new FNode<T>(child);
        childNode.parent = this;
        this.children.add(childNode);
        return childNode;
    }
}

